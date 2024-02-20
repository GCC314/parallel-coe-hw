CFLAGS = -std=c++11
CXX = g++

main:
	@ echo "Compiling target 'main'..."
	@ $(CXX) *.cpp -o main

test: main
	@ echo "Testing benchmark...\n"
	@ ./main

debugtest:
	@ echo "Compiling target 'main' in debug mode..."
	@ $(CXX) *.cpp -o main -DDEBUG
	@ echo "Testing benchmark...\n"
	@ ./main

clean:
	@ echo "Removing executable file 'main'..."
	@ rm ./main