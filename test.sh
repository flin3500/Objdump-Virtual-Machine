#!/bin/bash

# Trigger all your test cases with this script
export ASAN_OPTIONS=verify_asan_link_order=0
for file in tests/*.asm
do
	file_name=${file##*/}
	./vm_x2017 ./tests/${file_name%.*}.x2017 > result
	if cmp -s result tests/${file_name%.*}.asm;
	then
		#echo -e "${file_name%.*} : \033[32m [PASS] \033[0m"
		echo -e "\033[32m [PASS] \033[0m : ${file_name%.*}" 
	else
		#echo -e "${file_name%.*} : \033[31m [FAIL] \033[0m"
		echo -e "\033[31m [FAIL] \033[0m : ${file_name%.*}" 
	fi
done 
