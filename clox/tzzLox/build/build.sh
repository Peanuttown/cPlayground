cmake ..
echo $?
if [ $? -eq 0 ];then
	make
else
	echo "cmake fail"
	exit 1
fi
