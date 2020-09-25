all:
	cc ./fmenu.c -o fmenu.bin -lncursesw -lmenuw && ./fmenu.bin

clean:
	rm ./fmenu.bin
