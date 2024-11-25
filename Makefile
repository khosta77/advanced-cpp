W=-Wall -Werror -Wextra

all: clean first second launch

first:
	g++ -std=c++20 -O2 $(W) first.cpp -o first.out

second:
	g++ -std=c++20 -O2 $(W) second.cpp -o second.out

launch:
	./first.out
	./second.out

clean:
	rm -rf *.out
