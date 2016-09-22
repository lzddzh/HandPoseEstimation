# Know at debug time, we just use -O1, later we can use -O2,
# which will bring faster speed on running.
all: run

LoadData.o: common.h LoadData.h LoadData.cpp
	g++ -c -O1 LoadData.h LoadData.cpp -std=c++11

Node.o: common.h Node.h Node.cpp
	g++ -c -O1 Node.h Node.cpp -std=c++11

Tree.o: common.h Node.h Tree.h Tree.cpp
	g++ -c -O1 Tree.h Tree.cpp -std=c++11

RandomForest.o: common.h LoadData.h Tree.h RandomForest.h RandomForest.cpp
	g++ -c -O1 RandomForest.h RandomForest.cpp -fopenmp -larmadillo -std=c++11

main.o: main.cpp
	g++ -c -O1 main.cpp -std=c++11

run: Tree.o RandomForest.o Node.o LoadData.o main.o
	g++ -o run -O1 Tree.o RandomForest.o Node.o LoadData.o main.o -larmadillo -fopenmp -std=c++11

clean:
	rm *.gch *.o run
