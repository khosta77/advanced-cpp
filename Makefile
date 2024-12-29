TARGET=./main
CC=g++
STD=-std=c++20
W=-Wall -Werror -Wextra
SANITIZE=-fsanitize=address
CXXFLAGS=-I/opt/homebrew/opt/boost/include

all: clean test clean $(TARGET)

install:
	git clone git@github.com:nlohmann/json.git

test:
	$(CC) $(CXXFLAGS) --coverage -g -O0 $(SANITIZE) $(STD) $(W) main.cpp -o test_main
	./test_main
	@echo "---------------------"
	@gcov test_main-main.cpp | grep -A 1 "File 'serialize.hpp'"
	@gcov test_main-main.cpp | grep -A 1 "File 'deserialize.hpp'"
	@echo "---------------------"
	@rm -rf main *.out test_main *.gcov *.gcda *.gcno *.dSYM

$(TARGET):
	$(CC) $(CXXFLAGS) $(STD) -O2 $(W) main.cpp -o $(TARGET)

clean:
	rm -rf main *.out test_main *.gcov *.gcda *.gcno *.dSYM
#	clang-format --dry-run --Werror --style=file units/*.cpp units/*.hpp
