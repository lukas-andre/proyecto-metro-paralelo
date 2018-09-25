CXX = mpic++ -g3 -Wall
CXXFLAGS = -np 5

all: program 

debug: make DEBUG=TRUE

program:
	$(CXX) main.cpp metrored.cpp dataloader.cpp -o programa

run: 
	mpirun --allow-run-as-root -np 5 ./programa -f VVA CA