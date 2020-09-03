#!/bin/bash
# [[ -f ~/.bash_profile ]] && source ~/.bash_profile
# [[ -f ~/.bashrc ]] && source ~/.bashrc

notsrcsuffix=".jpg.png.jpeg.bmp.gif.pdf.log.zip.7z.rar.gz."
srcsuffix=".go.py.java.c.cpp.js.html.css.php.h.m.pch.plist.xib"
readonly notsrcsuffix
readonly srcsuffix
tmp_file=.codeLine.tmp
go_install_package=go1.9.1.linux-amd64.tar.gz

function log_error(){
    if [ "$color" != "1" ];then
        echo "\033[31m[error]   \033[0m$*"
    else
        echo "[error]   $*"
    fi
}

function log_success(){
    if [ "$color" != "1" ];then
        echo "\033[32m[success] \033[0m$*"
    else
        echo "[success] $*"
    fi
}
function log_notice(){
    echo "[notice]  $*"
}
function log_warning(){
    if [ "$color" != "1" ];then
        echo "\033[35m[warning] \033[0m$*"
    else
        echo "[warning] $*"
    fi
}

function robot(){
    curl -X POST http://qyin.im.baidu.com/msgt/api/sendMsgToGroup?access_token=cd5db6ea6bee8cd6bb21d4d79ba50022 -d "{\"msg_type\":\"text\", \"access_token\":\"cd5db6ea6bee8cd6bb21d4d79ba50022\", \"to\":$1, \"content\": \"$2\"}"  -H "Content-Type: application/json"
}
function to_upper(){
    echo $1 | tr '[a-z]' '[A-Z]'
}
function to_lower(){
    echo $1 | tr '[A-Z]' '[a-z]'
}
function filenotpuretext(){
    echo $notsrcsuffix | grep -c ".$(to_lower $1)"
}
function filesuffix(){
    echo ${1##*.}
}

function install_go(){
    log_success "installing go..."
    jumbo install go &>/dev/null
    echo -e 'export GOPATH=~/.jumbo/lib/go' >> ~/.bash_profile && source ~/.bash_profile
    #[[ -f ~/go1.7.linux-386.tar.gz ]] || cp warehouse/tools/go1.7.linux-386.tar.gz ~
    #cd ~
    #tar zxvf go1.7.linux-386.tar.gz &>/dev/null
    #rm go1.7.linux-386.tar.gz
    #echo -e 'export PATH=~/go/bin:$PATH' >> ~/.bash_profile && source ~/.bash_profile
    #echo -e 'export GOROOT=~/go' >> ~/.bash_profile && source ~/.bash_profile
    #cd -
}
function install(){
    go_install_package=./warehouse/packs/${go_install_package}
    [[ -f ${go_install_package} ]] || exit 1
    tar zxvf ${go_install_package}

    echo $PATH  |grep -q `pwd`/go/bin || echo -e "export PATH=`pwd`/go/bin:$PATH" >> ~/.bash_profile
    echo $GOROOT|grep -q `pwd`/go     || echo -e "export GOROOT=`pwd`/go" >> ~/.bash_profile

    [[ -d code/src ]] || mkdir -p code/src
    echo $GOPATH|grep -q `pwd`/code   || echo -e "export GOPATH=`pwd`/code" >> ~/.bash_profile

    source ~/.bash_profile
}
function install_git(){
    log_success "installing git..."
    jumbo install git &>/dev/null
}
function install_package(){
    cd src/$1
    
    go install
    if [ $? -eq 0 ];then
        log_success "[PASS] $1"
    else
        log_error "[FAIL] $1"
    fi
    
    cd - &>/dev/null
}
function goinit(){
    cd warehouse/tools/script/goTools

    root=$GOPATH
    export GOPATH=$(pwd)
    echo $(pwd)

    [[ -f src.zip ]] && unzip src.zip 

    #install_package github.com/ramya-rao-a/go-outline
    install_package github.com/nsf/gocode
    install_package github.com/rogpeppe/godef
    install_package github.com/zmb3/gogetdoc
    install_package github.com/golang/lint/golint
    install_package github.com/ramya-rao-a/go-outline
    install_package sourcegraph.com/sqs/goreturns
    install_package golang.org/x/tools/cmd/gorename
    install_package github.com/tpng/gopkgs
    install_package github.com/acroca/go-symbols
    install_package golang.org/x/tools/cmd/guru
    install_package github.com/cweill/gotests/gotests
    install_package golang.org/x/tools/cmd/godoc
    install_package github.com/fatih/gomodifytags
    install_package github.com/josharian/impl
    #install_package github.com/derekparker/delve

    [[ -d bin ]] && mv bin/* $root/bin
    rm -rf bin pkg src
}
function gitinit(){
    git config --global user.email $1@baidu.com    ## 把xxxx@badiu.com改为你的邮箱
    git config --global user.name $1               ## 把xxxx改为你的账号
    git config --global remote.origin.push 'refs/heads/*:refs/for/*'
}
function gitclean(){
    ls $1 | while read line
    do
        full=$1/$line
        test -f $full
        if [ $? -ne 0 ]; then
            if [ "A$line" == 'A.git' ]; then
                rm -rf $full
            else
                list "$full"
            fi
        fi
    done
}
function init(){
     # install jumbo
    jumbo &>/dev/null || bash -c "$( curl http://jumbo.baidu.com/install_jumbo.sh )"; source ~/.bashrc;

    # install golang
    go version &>/dev/null || install_go

    # install git
    git version &>/dev/null || install_git

    # config git
    grep -q '@baidu.com' ~/.gitconfig
    if [ $? -ne 0 ];then
        read -p "config git, please input the prefix of your email address, such as zouxun01: " name
        echo '[user]' >> ~/.gitconfig
        echo "        name  = ${name}" >> ~/.gitconfig
        echo "        email = ${name}@baidu.com" >> ~/.gitconfig
    fi
    grep -q 'refs/heads/*:refs/for/*' ~/.gitconfig
    if [ $? -ne 0 ];then
        echo '[remote "origin"]' >> ~/.gitconfig
        echo '        push = refs/heads/*:refs/for/*' >> ~/.gitconfig
    fi

    # create syslink to ${CURRENT}
    goroot=`go env > goenvtmp; source goenvtmp; rm goenvtmp; echo $GOROOT`
    if [ -e ${goroot}/src/${CURRENT} ];then
        if [ A`ls -al ${goroot}/src/${CURRENT}|awk '{print $11}'` != A ];then
            rm ${goroot}/src/${CURRENT}
        else
            [[ `pwd` != ${goroot}/src/${CURRENT} ]] &&  mv ${goroot}/src/${CURRENT} ${goroot}/src/${CURRENT}.$(date +'%H:%M:%S %Y-%m-%d')
        fi
    fi
    ln -s $(pwd) ${goroot}/src/${CURRENT}
    if [ A$GOROOT = A ];then
        echo -e "export GOROOT=${goroot}" >> ~/.bash_profile
        source ~/.bash_profile
    fi
    which wl &>/dev/null || echo -e "alias wl='cd $(pwd)'" >> ~/.bashrc
    which wd &>/dev/null || echo -e "alias wd='cd $(pwd)/warehouse/deploy'" >> ~/.bashrc
    which wr &>/dev/null || echo -e "alias wr='cd $(pwd)/warehouse/record'" >> ~/.bashrc
    source ~/.bashrc

    build && log_success "env init success, and ${CURRENT} build PASS"
    exit 0


    #jumbo &>/dev/null || bash -c "$( curl http://jumbo.baidu.com/install_jumbo.sh )"
    #source ~/.bashrc
    #[[ A$1 == ABCLOUD ]] || git pull || jumbo install git && git pull
    go version || jumbo install go
    [[ -e ./${CURRENT} ]] && rm -rf ./${CURRENT}
    go build ${CURRENT}.go || ln -s $(pwd) ~/.jumbo/lib/go/src/${CURRENT} && go build ${CURRENT}.go
    [[ -e ./output ]] && rm -rf ./output
    
    mkdir -p output/warehouse && cp -r *.sh ./${CURRENT} ./conf ./tools ./output && cp -r ./warehouse/tools ./output/warehouse
    exit 0
}

function go_publish(){
    log_success "publish test asset"
    # go get -u github.com/jteeuwen/go-bindata/...
    # GOOS=linux GOARCH=amd64 go build -o go-bindata.linux
    # GOOS=darwin GOARCH=amd64 go build -o go-bindata.darwin
    # GOOS=windows GOARCH=amd64 go build -o go-bindata.exe
    texe="go-bindata.exe"
    if [ $OS = Linux ];then
        texe="go-bindata.linux"
    elif [ $OS = Darwin ];then
        texe="go-bindata.darwin"
    fi
    ls ./warehouse/tools/bin/bindata/
    fullexe="./warehouse/tools/bin/bindata/$texe"
    
    log_success "bind test data"
    [[ -f $fullexe ]] && $fullexe -o=$1 -pkg=$2 $3
}

function list(){
    ls $1 | while read line
    do
        full=$1/$line
        test -f $full
        if [ $? -eq 0 ]; then
            #echo "$2 $full"
            wc -l $full
        else
            #list "$full" "$2"
            list "$full"
        fi
    done
}

function reset(){
    git fetch --all && git reset --hard origin/master
}

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
function build(){
    if [ ! $GOPATH ]; then
        cd ../../               # 如果根目录位于src下
        # cd ../../../../       #如果根目录如baidu/hiai/bingo
        export GOPATH="$(pwd)"
        cd -
    fi

    start=`date +%s`
    #log_success "build start\t"$start
    time go install -v
    ret=$?

    end=`date +%s`

    if [ $ret -eq 0 ];then
        log_success "build cost\t"$[$end-$start]
        name="${CURRENT}"
        [[ -f $GOPATH/bin/$name ]] || name="${CURRENT}.exe"
        cp $GOPATH/bin/$name  .

        if [ $# -ne 0 ];then
            ./$name $*
            #code=$?
            #rm -f ./$name
            return $?
        fi
    else
        log_error "build cost\t"$[$end-$start]
    fi
    return $ret
}
function update(){
    git pull &> git_pull_log
    grep 'Already up-to-date' git_pull_log || build    
    [[ -f git_pull_log ]] && cat git_pull_log && rm git_pull_log
}

function fmt(){
    ls $1 | while read file; do
        file=$1/$file
        if [ -f $file ]; then
            go fmt $file
        else
            fmt $file
        fi
    done
}

function code_check(){
    codeLine=$2
    [[ A$2 = A ]] && codeLine=$(cat $tmp_file)
    #log_warning $codeLine $1 $2

    ls $1 | while read file; do
        file=$1/$file
        #file=$(pwd)/$1/$file
        #log_notice $file
        #wc -l $file|awk '{print $1}'
        #test $file
        if [ -f $file ]; then
            sf=$(filesuffix $file)
            if [ $(filenotpuretext $sf) -ne 0 ];then
                log_warning "$file is not source code file"
            else
                line=`wc -l $file|awk '{print $1}'`
                #codeLine=$line
                [[ -e $tmp_file ]] && codeLine=$[$(cat $tmp_file)+$line]
                echo $codeLine > $tmp_file
                log_notice "file: $file; line: $line; total: $codeLine"
            fi
        else
            code_check $file
            log_warning "dir: $file"
        fi
    done
}
function code(){
    code_check $1 0
    log_success "code line: $(cat $tmp_file)"
    [[ -f $tmp_file ]] && rm $tmp_file
}

function md5_sum(){
    if [ "$OS" = "Linux" ];then
        md5sum $1|awk '{print $1}'
    elif [ "$OS" = "Darwin" ];then
        #alias md5sum="md5"
        md5 $1|awk '{print $4}'
    fi
}
function md5_check(){
    if [ -f $1 ]; then
        echo "$(md5_sum $1)\t$1"
    else
        ls $1 | while read file; do
            file=$1/$file
            if [ -f $file ]; then
                echo "$(md5_sum $file)\t$file"
            else
                md5_check $file
            fi
        done
    fi
}
function MD5(){
    md5_file=md5.log
    md5_check $1|sort > $md5_file
    awk '{print $1}' $md5_file | uniq -d | sort -rn | while read line; do
        grep $line $md5_file | awk -v var=$line '{print $2}END{print "---- "var"  "NR" ----\n"}'
    done
    #log_success "code line: $(cat $tmp_file)"
    exit
}

function func_usage(){
    echo -e "\033[32msh god.sh synclib version     \033[0m# sync lib with version like: 1.0.0.2"
    echo -e "\033[32msh god.sh fsynclib version    \033[0m# sync lib with version like: 1.0.0.2 by force"
    echo -e "\033[32msh god.sh reset               \033[0m# sync source code with remote by force"
    echo -e "\033[32msh god.sh goinit              \033[0m# install golang related tools"
    echo -e "\033[32msh god.sh code path           \033[0m# calculate code line"
    echo -e "\033[32msh god.sh MD5 path            \033[0m# check duplicated file"
    echo -e "\033[32msh god.sh gitinit username    \033[0m# init git config"
}