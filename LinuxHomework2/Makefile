a.out : Client.o FileManipulator.o CStatus.o
	g++ Client.o FileManipulator.o CStatus.o
	rm *.o 	
Client.o : Client.cpp
	g++ -c Client.cpp 
FileManipulator.o : FileManipulator.cpp 
	g++ -c FileManipulator.cpp
CStatus.o : CStatus.cpp
	g++ -c CStatus.cpp
clean:
	rm -rf *.o *.out 
