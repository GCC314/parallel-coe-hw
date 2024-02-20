CFLAGS = -std=c++11

all: test main

test: main
	@ echo "Testing file 'a.in'...\n"
	@ ./main a.in
	@ echo "Testing file 'b.in'...\n"
	@ ./main b.in

.PHONY: main

main:
	@ echo "Compiling target 'main'..."
	@ $(CXX) input.cpp mat_demo.cpp main.cpp -o main

clean:
	@ echo "Removing executable file 'main'..."
	@ rm ./main

clean_log:
	@ echo "Cleaning logs in the directory..."
	@ rm ./log-*.log