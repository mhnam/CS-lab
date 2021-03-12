cc=gcc
cflags = -W -g
target = main

objects = main.o turtle.o blackcow.o dog.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<

main.o turtle.o blackcow.o dog.o : animal.h

.PHONY : clean
clean :
	rm $(target) $(objects)
