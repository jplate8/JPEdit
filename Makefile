all:
	clang++ -std=c++11 src/*.cpp -lncurses -o jpedit

clean:
	rm jpedit
