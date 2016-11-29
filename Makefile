all:
	g++ ucoroutine.cpp -g -c
	g++ testcase.cpp -g -o testcase ucoroutine.o
clean:
	rm -f ucoroutine.o testcase
