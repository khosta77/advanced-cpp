TARGET=./main
CC=g++
STD=-std=c++20
W=-Wall -Werror -Wextra
SANITIZE=-fsanitize=address

all: clean hashtable intrusive_list lru $(TARGET)

hashtable:
	$(CC) -O0 $(STD) $(W) test_hashtable.cpp -o test_hashtable
	./test_hashtable
	rm ./test_hashtable

intrusive_list:
	$(CC) -O0 $(STD) $(W) test_intrusive_list.cpp -o test_intrusive_list
	./test_intrusive_list
	rm ./test_intrusive_list

lru:
	$(CC) -O0 $(STD) $(W) test_lru.cpp -o test_lru
	./test_lru
	rm ./test_lru

$(TARGET):
	$(CC) $(STD) -O2 $(W) main.cpp -o $(TARGET)

clean:
	#clang-format --dry-run --Werror --style=file *.cpp *.hpp
	rm -rf test_lru main test_hashtable test_intrusive_list
