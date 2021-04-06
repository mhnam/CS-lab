OBJECTS = main.o string.o
SRC = main.cpp string.cpp

CC = g++
CFLAGS = -c

TARGET = a.out

$(TARGET): $(OBJECTS) String.h
	$(CC) -o $(TARGET) $(OBJECTS)

clean:
	rm $(OBJECTS) $(TARGET)
