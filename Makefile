TARGET=./main
CC=g++
STD=-std=c++20
W=-Wall -Werror -Wextra
SANITIZE=-fsanitize=address

all: clean crew_members passangers plane $(TARGET)

crew_members:
	$(CC) --coverage -g -O0 $(SANITIZE) $(STD) $(W) units/crew_members.cpp -o test_crew_members
	./test_crew_members
	@echo "---------------------"
	@gcov test_crew_members-crew_members.cpp | grep -A 1 "File 'units/crew_members.hpp'"
	@gcov test_crew_members-crew_members.cpp | grep -A 1 "File 'units/crew_members.cpp'"
	@echo "---------------------"
	@rm ./test_crew_members
	@rm *.gcov *.gcda *.gcno
	@rm -rf *.dSYM

passangers:
	$(CC) --coverage -g -O0 $(SANITIZE) $(STD) $(W) units/passengers.cpp -o test_passengers
	./test_passengers
	@echo "---------------------"
	@gcov test_passengers-passengers.cpp | grep -A 1 "File 'units/passengers.hpp'"
	@gcov test_passengers-passengers.cpp | grep -A 1 "File 'units/passengers.cpp'"
	@echo "---------------------"
	@rm ./test_passengers
	@rm *.gcov *.gcda *.gcno
	@rm -rf *.dSYM

plane:
	$(CC) --coverage -g -O0 $(SANITIZE) $(STD) $(W) units/plane.cpp -o test_plane
	./test_plane
	@echo "---------------------"
	@gcov test_plane-plane.cpp | grep -A 1 "File 'units/plane.hpp'"
	@gcov test_plane-plane.cpp | grep -A 1 "File 'units/plane.cpp'"
	@echo "---------------------"
	@rm ./test_plane
	@rm *.gcov *.gcda *.gcno
	@rm -rf *.dSYM

$(TARGET):
	$(CC) $(STD) -O2 $(W) units/main.cpp -o $(TARGET)

clean:
	rm -rf main test_crew_members test_passengers test_plane
#	clang-format --dry-run --Werror --style=file units/*.cpp units/*.hpp

