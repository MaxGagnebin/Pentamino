CXX=g++
CXX_FLAGS= -O3


All: main.o Plateau.o Config.o 
	${CXX} ${CXX_FLAGS} -o Game main.o Plateau.o Config.o 


Plateau.o: Plateau.cpp
	${CXX} ${CXX_FLAGS} -c Plateau.cpp

main.o: main.cpp 
	${CXX} ${CXX_FLAGS} -c main.cpp
	
	
Config.o: Config.cpp 
	${CXX} ${CXX_FLAGS} -c Config.cpp
	
clean:
	rm *.o 
