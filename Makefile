OBJECTS = main.o
SRC = main.cpp

CC = g++
CFLAGS = -c

TARGET = test

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

clean: 
	rm $(OBJECTS) $(TARGET)
	
