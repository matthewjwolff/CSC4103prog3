all :
	make pi
	make dot
	make bar
	make scan

clean:
	rm dot
	rm pi
	rm bar.o
	rm scan

dot:
	gcc dot.c -o dot -pthread

pi:
	gcc pi.c -o pi -pthread

bar:
	gcc bar.c -c -o bar.o

scan:
	gcc scan.c -o scan
