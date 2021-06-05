all: sph.x

sph.x: ./src/*.cpp
	g++ -std=c++17 -O4 -o sph.x ./src/*.cpp -larmadillo -I ./include

clean:
	rm *.x *.o
