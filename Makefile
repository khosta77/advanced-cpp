TARGET=./main.out
CC=g++
STD=-std=c++20
W=-Wall -Werror -Wextra

all: clean $(TARGET)

$(TARGET):
	$(CC) $(STD) -O2 $(W) main.cpp -o $(TARGET)

clean:
	rm -rf main.out
