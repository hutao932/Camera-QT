
if [ $# -eq 0 ];then
    echo "please input branch"
    exit 1
fi
pro_file=ci.yml

branch=$1
sub_string="      command : sh -x god.sh yanhongbin baidu-idl-xteam-xeye-35 "
new_string=$sub_string$branch
# echo $new_string
# cat $pro_file | grep "$sub_string" |awk '{$8="'"$branch"'";print $8}'
sed "s/${sub_string}.*/${new_string}/" $pro_file >> $pro_file"_tmp"
mv $pro_file"_tmp" $pro_file
git add $pro_file
git commit -m "add ci.yml"
git push -u origin HEAD:refs/for/$branch