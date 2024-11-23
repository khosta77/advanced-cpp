W=-Wall -Werror -Wextra

all: clean main launch

main:
	g++ -std=c++20 -O2 $(W) main.cpp -o main.out

launch:
	./main.out

clean:
	rm -rf main.out
