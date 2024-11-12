TARGET=./main
CC=g++
STD=-std=c++20
W=-Wall -Werror -Wextra
SANITIZE=-fsanitize=address

all: clean $(TARGET)

$(TARGET):
	$(CC) $(STD) -O2 units/main.cpp -o $(TARGET)

build:
	$(TARGET)

clean:
	rm -rf $(TARGET)
