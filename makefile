run: main.o src.o
	g++-11 -o run main.o src.o

main.o: main.cpp
	g++-11 -c main.cpp

src.o: src.cpp
	g++-11 -c src.cpp

clean:
	rm run *.o