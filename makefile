all: sph.x

sph.x: ./src/*.cpp
	g++ -o sph.x ./src/*.cpp -larmadillo -I ./include

clean:
	rm *.x *.o
