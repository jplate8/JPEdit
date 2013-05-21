logfile="jpedit.log"
raw_exec=".jpedit"
exec="jpedit"

all: debug

debug:
	@ clang++ -std=c++11 src/*.cpp -lncurses -o $(raw_exec)
	@ echo "#!/bin/sh" > $(exec)
	@ echo "" >> $(exec)
	@ echo "exec ./$(raw_exec) 2> $(logfile)" >> $(exec)
	@ chmod +x $(exec)

release:
	@ clang++ -std=c++11 -D NDEBUG src/*.cpp -lncurses -o $(raw_exec)
	@ echo "#!/bin/sh" > $(exec)
	@ echo "" >> $(exec)
	@ echo "exec ./$(raw_exec)" >> $(exec)
	@ chmod +x $(exec)

clean:
	@ rm $(raw_exec) $(exec)
