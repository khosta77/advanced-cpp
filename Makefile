TARGET=./main
CC=g++
STD=-std=c++20
W=-Wall -Werror -Wextra
SANITIZE=-fsanitize=address

all: clean hashtable intrusive_list lru $(TARGET)

hashtable:
	$(CC) --coverage -g -O0 $(SANITIZE) $(STD) $(W) test_hashtable.cpp -o test_hashtable
	./test_hashtable
	@echo "---------------------"
	@gcov test_hashtable-test_hashtable.cpp | grep -A 1 "File 'hashtable.hpp'"
	@gcov test_hashtable-test_hashtable.cpp | grep -A 1 "File 'test_hashtable.cpp'"
	@echo "---------------------"
	@rm ./test_hashtable
	@rm *.gcov *.gcda *.gcno
	@rm -rf *.dSYM

intrusive_list:
	$(CC) --coverage -g -O0 $(SANITIZE) $(STD) $(W) test_intrusive_list.cpp -o test_intrusive_list
	./test_intrusive_list
	@echo "---------------------"
	@gcov test_intrusive_list-test_intrusive_list.cpp | grep -A 1 "File 'intrusive_list.hpp'"
	@gcov test_intrusive_list-test_intrusive_list.cpp | grep -A 1 "File 'test_intrusive_list.cpp'"
	@echo "---------------------"
	@rm ./test_intrusive_list
	@rm *.gcov *.gcda *.gcno
	@rm -rf *.dSYM

lru:
	$(CC) --coverage -g -O0 $(SANITIZE) $(STD) $(W) test_lru.cpp -o test_lru
	./test_lru test_lru-test_lru.
	@echo "---------------------"
	@gcov test_lru-test_lru.cpp | grep -A 1 "File 'lru.hpp'"
	@gcov test_lru-test_lru.cpp | grep -A 1 "File 'test_lru.cpp'"
	@echo "---------------------"
	@rm ./test_lru
	@rm *.gcov *.gcda *.gcno
	@rm -rf *.dSYM

$(TARGET):
	$(CC) $(STD) -O2 $(W) main.cpp -o $(TARGET)

clean:
	clang-format --dry-run --Werror --style=file *.cpp *.hpp
	rm -rf test_lru main test_hashtable test_intrusive_list
