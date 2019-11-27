cmake ..
echo $?
if [ $? -eq 0 ];then
	make
else
	echo "cmake fail"
fi
