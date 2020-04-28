sph.x: main.cpp simulation.cpp boundary.cpp
	g++ -o sph.x main.cpp simulation.cpp boundary.cpp -larmadillo

clean:
	rm *.x *.o
