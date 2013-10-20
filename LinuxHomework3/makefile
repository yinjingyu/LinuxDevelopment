a.out:CClient.o CThread.o CStatus.o CExecutiveObject.o CClientBusinessForExecObj.o
	g++ -g CClient.o CThread.o CStatus.o CExecutiveObject.o CClientBusinessForExecObj.o -lpthread
	rm -rf *.o
CClient.o : CClient.cpp
	g++ -g -c CClient.cpp
CThread.o : cpp/CThread.cpp
	g++ -g -c cpp/CThread.cpp
CStatus.o : cpp/CStatus.cpp
	g++ -g -c cpp/CStatus.cpp
CExecutiveObject.o : cpp/CExecutiveObject.cpp
	g++ -g -c cpp/CExecutiveObject.cpp
CClientBusinessForExecObj.o : cpp/CClientBusinessForExecObj.cpp
	g++ -g -c cpp/CClientBusinessForExecObj.cpp
clean:
	rm -rf *.o *.out
