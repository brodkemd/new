run: main.o
	g++-11 -o run main.o

main.o: main.cpp
	g++-11 -c main.cpp