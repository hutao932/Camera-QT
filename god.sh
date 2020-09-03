#!/bin/bash
CURRENT=wall-e
readonly CURRENT
OS=$(uname)
readonly OS
libfile=./godlib.sh
readonly libfile
if [ -f $libfile ];then
	source $libfile
fi
function synclib(){
    # wget -r -nH --level=0 --cut-dirs=9 getprod@buildprod.scm.baidu.com:/temp/data/prod-64/baidu/hiai/lib/CITASK/r9aafc44a35f11fba73b5deddc7d4856735c4e8a7-8823087/  --user getprod --password getprod --preserve-permissions
    # $1应该是"1.0.0.1"形式的四位版本号
    ver=$1
    ver=$(echo $ver|sed 's/\./-/g')
    [[ ! $ver ]] || ver=1-0-0-2
    if [ -d ../lib ];then
        if [ "$2" != "" ];then
            rm -rf ../lib
        else
            return
        fi
    fi
    [[ -d ./output ]] && rm -rf ./output
    wget -r -nH --level=0 --cut-dirs=7 getprod@product.scm.baidu.com:/data/prod-64/baidu/hiai/lib/lib_${ver}_PD_BL/  --user getprod --password getprod --preserve-permissions
    if [ $? -eq 0 ]; then
        mv ./output ../lib
        log_success "baidu/hiai/lib [$ver] have been installed successfully"
    else
        log_error "command [wget] seems do work incorrectly, pls make it work"
        log_warning "or download lib from icode by git, [git clone ssh://zouxun01@icode.baidu.com:8235/baidu/hiai/lib]"
        log_warning "and make sure src be saved in path [$GOAPTH/src/baidu/hiai/lib]"
    fi
}
function fsynclib(){
    synclib "$1" "-f"
}
function publish(){
    go_publish testshop/asset/asset.go asset testshop/data/...
    # go_publish testshop/asset/asset.go asset warehouse/cache/...
    # go_publish testshop/asset/asset.go asset conf/...

    log_success "building..."
    build
    if [ $? -ne 0 ];then
        cd ${GOPATH}/src && rm $CURRENT
        go build -v &> tmp && cat tmp && rm tmp
        exit 1
    else
        GOOS=linux GOARCH=amd64 time go build -v -o ${CURRENT}.linux
        GOOS=darwin GOARCH=amd64 time go build -v -o ${CURRENT}.darwin
        GOOS=windows GOARCH=amd64 time go build -v -o ${CURRENT}.exe

        [[ -d output ]] && rm -rf output/*
        [[ -d output ]] || mkdir -p output
        cp -r README.md $CURRENT* conf warehouse doc ./output
        zip -r output.zip output
        ls -alh
        #mkdir -p output/src && cp -r lib testshop tools *.go ./output/src

        cd ${GOPATH}/src && rm $CURRENT
        exit 0
    fi
}
function cases(){
    git status| grep cases.go$ | awk '{print $2}' | while read line; do
        git show HEAD:$line > tmp
        diff tmp $line | grep '>' | awk -F'(' '{print $2}'| awk -F')' '{print $1}'| awk -F'_' '{print "go run wall-e.go drun -d "$NF" -url \"http://127.0.0.1\" || echo "$NF" >> result"}' >> ids.sh
        rm tmp
    done
    cat ids.sh && sh ids.sh && rm ids.sh
    cat result && rm result
}
function check(){
    pwd
    errorNum=0
    msg=""
    [[ -d log ]] || mkdir log

    log_notice "check: NewCase method had been abandoned"
    grep -wrn 'NewCase' testshop/baidu/*|grep -w 'New'
    if [ $? -eq 0 ];then
        errorNum=$[errorNum+1]
        log_warning "NewCase method can NOT be implemented any more"
        m=$(grep -wrn 'NewCase' testshop/baidu/*|grep -w 'New')
        # echo $m
        msg="NewCase method can NOT be implemented any more: $(echo $m | tr -d ["\n"])"
    fi

    grep 'super.Testing' -wrn testshop/baidu/*|grep -w 'func' |awk '{print $2}' | awk -F'(' '{print $1}'|awk -F'_' '{print $NF" "$1" "NF" "$0}'> log/tmp_cases
    log_notice "check: case struct naming format"
    cat log/tmp_cases |awk '{if($3<2){print $0}}'|sort|uniq -c|sort -rn > log/tmp_cases_id
    errName=`cat log/tmp_cases |awk 'BEGIN{n=0}{if($3<2){n++}}END{print n}'`
    if [ $errName -gt 1 ];then
        errorNum=$[errorNum+1]
        log_warning "$errName case(s) bad naming: it should be in format of: Feature_ID"

        m=$(cat log/tmp_cases |awk '{if($3<2){print $NF"; "}}')
        echo $m
        msg="$msg\n bad naming-it should be in format of Feature_ID: $(echo $m | tr -d ["\n"])"
    fi

    log_notice "check: forbidden standard lib/methods using (time.Sleep/fmt.Printf/fmt.Println)"
    errID=`grep 'fmt\.Print' -rn testshop/testshop.go testshop/baidu/*|wc -l`
    if [ $errID -ne 0 ];then
        errorNum=$[errorNum+1]
        log_warning "$errName case(s) bad using: forbidden standard lib/methods using: fmt.Print"

        m=$(grep fmt.Print -rn -c testshop/testshop.go testshop/baidu/*|awk -F':' '{if($2!=0){print $1": "$2"; "}}')
        echo $m
        msg="$msg\n forbidden standard methods using-fmt.Print: $(echo $m | tr -d ["\n"])"
    fi
    errID=`grep 'time\.Sleep' -rn testshop/testshop.go testshop/baidu/*|wc -l`
    if [ $errID -ne 0 ];then
        errorNum=$[errorNum+1]
        log_warning "$errName case(s) bad using: forbidden standard lib/methods using: time.Sleep"

        m=$(grep time.Sleep -rn -c testshop/testshop.go testshop/baidu/*|awk -F':' '{if($2!=0){print $1": "$2"; "}}')
        echo $m
        msg="$msg\n forbidden standard methods using-time.Sleep: $(echo $m | tr -d ["\n"])"
    fi
    errID=`grep 'log\.' -rn testshop/testshop.go testshop/baidu/*|wc -l`
    if [ $errID -ne 0 ];then
        errorNum=$[errorNum+1]
        log_warning "$errName case(s) bad using: forbidden standard lib/methods/package using: log"

        m=$(grep 'log\.' -rn -c testshop/testshop.go testshop/baidu/*|awk -F':' '{if($2!=0){print $1": "$2"; "}}')
        echo $m
        msg="$msg\n forbidden standard methods using-log lib: $(echo $m | tr -d ["\n"])"
    fi


    log_notice "check: case struct naming (containing case ID)"
    errID=`cat log/tmp_cases |awk 'BEGIN{n=0}{if($1 !~ /^[0-9]+$/){n++}}END{print n}'`
    if [ $errID -gt 1 ];then
        errorNum=$[errorNum+1]
        log_warning "$errName case(s) bad naming: it should be ended with id of its icafe card"

        m=$(cat log/tmp_cases |awk '{if($1 !~ /^[0-9]+$/){print $NF}}')
        echo $m
        msg="$msg\n bad naming-it should be ended with id of its icafe card: $(echo $m | tr -d ["\n"])"
    fi
    log_notice "check: duplicated case ID existence"
    #id feature split name
    cat log/tmp_cases |awk '{print $1}'|sort|uniq -c|sort -rn > log/tmp_cases_id
    biggest=`head -1 log/tmp_cases_id| awk '{print $1}'`
    if [ $biggest -gt 1 ];then
        errorNum=$[errorNum+1]
        log_warning "duplicated case ID:"
        m=$(cat log/tmp_cases_id| awk '{if($1>1){print $1"-"$2"; "}}')
        echo $m
        msg="$msg\n duplicated case ID: $(echo $m | tr -d ["\n"])"
    fi
    rm -f log/tmp_cases_id

    log_notice "check: forbidden usage of case ID existence: beginning with 0"
    count=`cat log/tmp_cases| awk '{if($1 ~ /^0/){print $NF}}'|wc -l `
    if [ $count -gt 1 ];then
        errorNum=$[errorNum+1]
        log_warning "forbidden usage of case ID:"
        m=$(cat log/tmp_cases| awk '{if($1 ~ /^0/){print $NF"; "}}')
        echo $m
        msg="$msg\n forbidden usage of case ID-beginning with 0: $(echo $m | tr -d ["\n"])"
    fi

    go build && ./wall-e case -c -o $1
    errorNum=$[errorNum+$?]


    if [ $errorNum -ne 0 ];then
        log_error "$errorNum bad styles, please check and fix them"
        # msg=$(echo $msg | tr -d ["\n"])
        tfile="./log/case"
        if [ -f $tfile ]; then
            msg="$msg\n $(cat $tfile | tr -d ["\n"])"
            # cat $tfile | while read line; do
            #     msg="$msg\n $(echo $line | tr -d ["\n"])"
            # done
            rm $tfile
        fi
        # echo $msg
    fi
    return $errorNum
}
function style(){
    check -b
    if [ $? -ne 0 ]; then
        # robot 1585897 "==bad styles codes check-in, please fix them==\n\n--run command: sh god.sh style--\n--Details-- \n$msg"
        robot 1535490 "==bad styles codes check-in, please fix them==\n\n--run command: sh god.sh style--\n--Details-- \n$msg"
        # robot 1614528 "==bad styles codes check-in, please fix them==\n\n--run command: sh god.sh style--\n--Details-- \n$msg" # debug
    fi
}
function commit(){
    style || exit $?
    log_success "publish test asset"
    publish
    # prebuild
    build
    if [ $? -ne 0 ];then
        log_error "build failed, please check your modifications and make sure No build failure"
        exit 1
    fi
    log_success "build OK"
    # delete build output
    [[ -f ${CURRENT} ]] && rm ${CURRENT}
    [[ -f ${CURRENT}.exe ]] && rm ${CURRENT}.exe
    [[ -d log ]] && rm -rf log
    git pull
    git status
    if [ "A$1" = "A" ]; then
        read -p "Please input your comments: " comments
        if [ "A$comments" = "A" ]; then
            #log_error "please input commit comments!"
            #exit 1
        comments="default update or bugs fixing @zouxun01"
        fi
    else
        comments=$1
    fi
    # commit to git
    if [ "A$2" = "A" ]; then
        read -p "Please input objects that will be committed: " objects
        if [ "A$objects" = "A" ];then
            log_warning "\"conf lib testshop tools ${CURRENT}.go\" will be committed"
            #git add conf lib testshop tools ${CURRENT}.go
            git diff --stat | grep '|' | awk '{print $1}' | while read line; do git add $line; done
        else
            log_success "\"$objects\" will be committed"
            git add $objects
        fi
    else
        objects=$2
        log_success "\"$objects\" will be committed"
        git add $objects
    fi
    git commit -m "$comments"
    if [ $? -eq 0 ];then
        git push -u origin HEAD:refs/for/master
    else
        log_error "no changes added to commit"
    fi
    #build
}
function commit_xeye(){
    AGILE_COMPILE_BRANCH=$3
    userName=`git log -n1 |grep "Author" |awk -F[:] '{print $2}' |awk '{print $1}'`
    echo $userName
    # userCount=`git config --list |grep "user.email=yanhongbin@baidu.com" |wc -l`
    # if [ $userCount -lt 1 ];then
    echo "set user.name/user.email"
    git config --global user.name $userName
    git config --global user.email $userName"@baidu.com"
    # fi

    if [ "A$1" = "A" ]; then
        read -p "Please input your comments: " comments
        if [ "A$comments" = "A" ]; then
            comments="default update or bugs fixing @zouxun01"
        fi
    else
        comments=$1
    fi
    if [ "A$2" = "A" ]; then
        read -p "Please input objects that will be committed: " objects
        if [ "A$objects" = "A" ];then
            log_warning "\"conf lib testshop tools ${CURRENT}.go\" will be committed"
            git diff --stat | grep '|' | awk '{print $1}' | while read line; do git add $line; done
        else
            log_success "\"$objects\" will be committed"
            git add $objects
        fi
    else
        objects=$2
        log_success "\"$objects\" will be committed"
        git add $objects
    fi
    metadatafile=$2
    versionNew=`cat $metadatafile |grep "version" |awk -F[=] '{print $2}'`
    git commit -m "$comments Automatic version update to $versionNew by Agile [ci skip]"
    if [ $? -eq 0 ];then
        git branch
        br=`git branch | grep "*"`
        branchName=`echo ${br/* /}`
        echo $AGILE_COMPILE_BRANCH
        git push -u origin HEAD:refs/for/$AGILE_COMPILE_BRANCH
    else
        log_error "no changes added to commit"
    fi
}


function git_pull(){
    git pull
    git status
}
function changeMetaData(){
    metadatafile=$1
    if [ -f $metadatafile ];then
        versionOld=`cat $metadatafile |grep "version" |awk -F[=] '{print $2}'`
        versionNew="${versionOld%.*}.$((${versionOld##*.}+1))"
        CommitID=`git rev-parse HEAD`
        cat $metadatafile |grep "version" |awk '{printf "version=%s\ncommit-id=%s", "'"$versionNew"'", "'"$CommitID"'"}' > $metadatafile"_tmp" && mv $metadatafile"_tmp" $metadatafile
        # echo $versionNew
    else
        echo $metadatafile"is not exist!"
        exit 1
    fi
    # [[ -f $metadatafile ]] && cat $metadatafile | awk -F[=.] '{printf "version=0.0.%d\n",$4 + 1}' > $metadatafile"_tmp" && mv $metadatafile"_tmp" $metadatafile
    # [[ -f $metadatafile ]] || exit 1
}
function bcloud_go(){
    export color=1
    style || exit $?
    # /home/scmbuild/workspaces_cluster.debug/baidu.faceqa.${CURRENT}.20170713.153129
    #cd /home/scmtools/buildkit/go
    #ls
    #go-1.3  go-1.4  go-1.5  go-1.6  go_1.7  go_1.8.1
    cd ..
    mkdir -p go/src
    gorootpath="/home/scmtools/buildkit/go"
    echo "go versions: `ls ${gorootpath}`"
    export GOROOT="${gorootpath}/$(ls ${gorootpath}|tail -1)"
    [[ -d $GOROOT ]] || export GOROOT='/home/scmtools/buildkit/go/go_1.8.1'
    #export GOROOT='/home/scmtools/buildkit/go/go_1.8.1'
    export PATH="${GOROOT}/bin:${PATH}"
    export GOPATH="$(pwd)/go"
    go env
    cd ${GOPATH}/src && ln -s ../../wall-e wall-e
    cd wall-e

    publish
}
function bcloud_makefile(){
    rootPath="$(pwd)"
    echo $rootPath
    echo "ubuntu version:"`cat /etc/issue`
    echo `ls -l /bin/sh`
    echo `hostname`
    [[ -d output ]] && rm -rf output/*
    [[ -d output ]] || mkdir -p output

    cd myriad
    echo `pwd`
    #source config_environment.sh
    echo "Configure environment variables for XEYE"
    export MV_TOOLS_DIR=$PWD/xos/compile_tools/
    echo "Configure MV_TOOLS_DIR: $MV_TOOLS_DIR"
    cd xcore/platform/xeye_app
    echo `pwd`
    make clean
    echo "make..."
    make -j
    if [ $? -ne 0 ];then
        echo "make fail!"
        exit 1
    fi

    cp output/xeye_app.* $rootPath/output
    cd $rootPath

}
function judgeDiff(){
    metadataName=$1
    count_all=`git diff --stat | wc -l`
    count_metadata=`git diff --stat |grep "$metadataName" |wc -l`
    echo $count_all
    echo $count_metadata
    if [ $count_metadata -eq 1 ];then
        if [ $count_all -gt 2 ];then
            echo "do not modify metadata manually!"
            exit 1
        elif [ $count_all -eq 2 ];then
            echo "metadata update only"
            exit 0
        else
            echo "git diff error"
            exit 2
        fi
    fi
}
function judgeAuthorAndCommit(){
    Author=$1
    Commit=$2
    if [ `git log -n1 |grep $Commit |wc -l ` -eq 1 ];then
        echo "CI skip!"
        exit 0
    fi
}
function copyFile(){
    cp BCLOUD output
}
function bcloud(){
    # bcloud_makefile
    copyFile
    judgeAuthorAndCommit $Author $Commit

    metadataName=metadata
    metadatafile=./$metadataName

    # judgeDiff $metadataName

    changeMetaData $metadatafile
    commit_xeye $Commit $metadatafile $Branch
#    cp BCLOUD no_dir
    
}
function xx_cache(){
    set -x
    while ((1));do
        git pull
        build ucache
        if [ $? -eq 0 ];then
            dc=`git diff --stat warehouse/cache | grep -c '.json'`
            if [ $dc -ne 0 ];then
                git add warehouse/cache
                git commit -m "[cache service] commit new icafe cache"
                # git push -u origin HEAD:refs/for/master
                git push
            fi
        fi
        sleep 1m
    done
}
function pb(){
    [ $# -lt 3 ] && exit 1
    file=$2
    module=$3
    export PATH=$(pwd)/warehouse/tools/bin/protobuf:$PATH
    chmod +x ./warehouse/tools/bin/protobuf/*
    pname=$(grep '^package ' $file |awk '{print $2}'|awk -F ';' '{print $1}')
    dir="./testshop/baidu/face/$module/$pname"
    [[ $module = $pname ]] && dir="./testshop/baidu/face/$module"
    [[ -d $dir ]] || mkdir -p $dir; cp $file $dir
    cd $dir
    protoc --go_out=. *.proto

    sed -i 's/github.com/wall-e\/lib\/3rd\/github.com/g' *.pb.go
    cd -
}
function case_list(){
    grep 'NewCase' -wrn testshop/baidu/*|grep -w 'New' |awk -F')' '{print $1" "$3}'|awk -F'(' '{print $1" "$2" "$3}'|awk -F'/' '{print $1"/"$2"/"$3"/"$4" "$5}'|awk '{print $1""$4"*"$7}'|awk -F'*' '{print $1" "$2" "$3}' > log/caselist
}
function change(){
    log_success "prepare cases list"
    case_list
    log_success "change case type name"
    cat log/caselist | while read line; do
        pkg=`echo $line|awk '{print $1}'`
        name=`echo $line|awk '{print $2}'`
        feature=`echo $name|awk -F'_' '{print $1}'`
        id=`echo $line|awk '{print $3}'`

        #newname="${feature}_${id}"
        newname="${name}_${id}"
        #c=`grep "type ${name}" testshop/baidu/* -rn|wc -l`
        #if [ $c -ne 1 ];then
        #    k=`grep "type ${name}" testshop/baidu/* -rn|awk -F':' '{print $1}'|awk -F'/' '{print $4}'|sort|uniq|wc -l`
        #    if [ $k -eq 1 ];then
        #        log_warning "$name duplicated: $c"
        #        continue
        #    fi
        #fi
        #continue

        log_success "changing: $name -> ${newname}"
        grep -rnw $name $pkg/* |awk -F':' '{print $1}'|sort|uniq|while read file; do
            grep ' NewCase()' $file|grep -q '.New(' && sed -i '/NewCase()/d' $file
            sed -i s"/\<${name}\>/${newname}/g" $file
            #sed -i s"type /${name} struct/type ${name}_${id} struct/g" $file
        done
    done

    #log_success "remove NewCase() function"
    #grep 'NewCase()' -rn testshop/baidu/*|grep '.New(' | awk -F':' '{print $1}'|sort|uniq|while read fi; do
    #    sed -i '/NewCase()/d' $fi
    #done
}
function duplicated(){
    case_list
    cat log/caselist |awk '{print $3}'|sort|uniq -c|sort -n
}
function bns_run(){
    MODEL=$1
    CASES=$2
    URLS=($3)
    OFFSET=$4
    INTERVAL=$5
    RESULT_PATH=$6

    for((i=0;i<${INTERVAL};i++)); do
        if [[ ! -z "${URLS[$i]}" ]]; then
            build drun $MODEL $CASES --url=${URLS[$i]} > ${RESULT_PATH}/"bns_"${OFFSET}"_"${i} 2>&1
        fi
        #nohup build drun $MODEL $CASES --url=${machine_arr[${offset}]} > ${result_path}/${offset} 2>&1 &
    done
}
function bns(){
    BNS=$1
    MODEL=$2
    CASES=$3
    RESULT_PATH="./log/bns"

    #for i in $m
    #  do
    #       echo "------------------------"
    #       #sh god.sh run $2 $3 --url=$i
    #       echo "------------------------"
    #  done

    [[ -d ${RESULT_PATH} ]] || mkdir -p ${RESULT_PATH}

    offset=0
    interval=10
    len=$(get_instance_by_service ${BNS} | wc -l)
    machine_arr=($(get_instance_by_service ${BNS} -p | awk '{print "http://"$1".baidu.com:"$2}'))
    while ((${offset} < ${len})); do
        sub_url=()
        for((i=0;i<${interval};i++)); do
            if [[ ${offset} -ge ${len} ]]; then
                break
            fi
            sub_url[${i}]=${machine_arr[${offset}]}
            let offset=${offset}+1
        done
        bns_run ${MODEL} ${CASES} "${sub_url[*]}" ${offset} ${interval} ${RESULT_PATH} &
    done
}
function model(){
    if [ $# != 2 ] ; then
        echo "para error"
        echo "eg: sh god.sh model old-url new-url"
        exit 1
    fi

    #based-on face-api total 45
    build run -d 11063,11064,11065,11066,11067,11068,11069,11070,11071,11072,11073,11074,11075,11076,11077,11078,11079,11080,11081,11082,11083,11084,11085,11086,11087,11088,11089,11090,11091,11092,11093,11094,11095,11096,11097,11098,11099,11100,11103,11104,11105,11106,11107,11108,11109 --url=${1}","${2} -tolerance *:0.01 -ignore log_id,timestamp,face_token

    #based-on feature-frame total 2
    # build run -d 11101,11102 --url=${1}","${2} -tolerance *:0.01 -ignore log_id,timestamp,face_token
}
function mn(){
    ta=`cat xx | awk '{a+=$2;b+=$3;c+=$4;d+=$5;e+=$6;f+=$7}END{print a" "b" "c" "d" "e" "f}'`
    tb=`cat xx | awk '{print $8"/"$9}'|awk -F'/' '{x+=$1;y+=$2;a+=$3;b+=$4}END{print x"/"y" "a"/"b}'`
    log_success "total: $ta $tb"
}

function usage(){
    func_usage

    echo "\033[32msh god.sh update                           \033[0m# pull the latest source code"
    echo "\033[32msh god.sh pb file module                   \033[0m# auto generate proto interface source code"
    echo "\033[32msh god.sh build                            \033[0m# used in BCLOUD"
    echo "\033[32msh god.sh comments files                   \033[0m# commit changed files"
    echo "\033[32msh god.sh ci                               \033[0m# CI"
    echo "\033[32msh god.sh cache                            \033[0m# update icafe cache"
    echo "\033[32msh god.sh model old-url new-url            \033[0m# run model regression test"
}

[[ -d output ]] && rm -rf output/*
[[ -d output ]] || mkdir -p output

# Author="yanhongbin"
# Commit="baidu-idl-xteam-xeye-35"
# Branch="develop-qatest"
Author=$1
Commit=$2
Branch=$3
bcloud $Author $Commit $Branch
