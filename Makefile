emulator: emulator.c
	gcc emulator.c -o emulator -g -Iinclude
hello: hello.asm
	ca65 hello.asm -o hello.o
	ld65 hello.o -o hello -C config.cfg
run: hello emulator
	./emulator hello
