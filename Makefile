CC=gcc
CFLAGS=-fsanitize=address -Wvla -Wall -Werror -g -std=gnu11 -lasan

# fill in all your make rules

vm_x2017: ./src/vm_x2017.c
	$(CC) $(CFLAGS) $^ -o $@

objdump_x2017: ./src/objdump_x2017.c
	$(CC) $(CFLAGS) $^ -o $@

tests: 
	bash test.sh
	rm -f result

run_tests:
	make vm_x2017
	make objdump_x2017
	bash test.sh
	make clean

clean:
	rm -f objdump_x2017
	rm -f vm_x2017
	rm -f result


