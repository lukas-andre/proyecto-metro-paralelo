CXX = mpic++ -g3 -Wall -Wextra
CXXFLAGS = 

 compile: metrored.cpp dataloader.cpp main.cpp
	$(CXX) -c dataloader.cpp metrored.cpp
	$(CXX) main.cpp dataloader.o metrored.o -o programa

# all: program 

# debug: make DEBUG=TRUE

# dataloader.o: clean dataloader.cpp dataloader.h
# 	$(CXX) dataloader.cpp -o dataloader.o

# metrored.o: metrored.cpp dataloader.h
# 	$(CXX) metrored.cpp  -o metrored.o

# program: dataloader.o metrored.o 
# 	$(CXX) -o programa main.cpp metrored.o dataloader.o

run: 
	mpirun --allow-run-as-root -np 6 ./programa -f LH CA

# clean:
# 	rm -fr *.o *.out programa
