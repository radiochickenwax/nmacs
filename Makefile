nmacs-glib: buffer.c
	gcc -std=c99 -g buffer.c   -o nmacs-glib -lncurses `pkg-config --cflags --libs glib-2.0`


clean: 
	rm  nmacs-glib
