OBJECTS = main.o string.o
SRC = main.cpp string.cpp

CC = g++
CFLAGS = -c

TARGET = test

$(TARGET): $(OBJECTS) String.h
	$(CC) -o $(TARGET) $(OBJECTS)

clean:
	rm $(OBJECTS) $(TARGET)
