all:
	clang++ src/*.cpp -lncurses -o jpedit

clean:
	rm jpedit
