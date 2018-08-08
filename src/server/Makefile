firstTest : test1.o test2.o
	g++ test1.o test2.o -o firstTest
test1.o : student.cpp student.h
	g++ -c student.cpp -o test1.o
test2.o : main.cpp student.h
	g++ -c main.cpp -o test2.o
clean:
	rm -rf *.o firstTest
