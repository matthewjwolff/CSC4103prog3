all :
	make pi
	make dot
	make bar
	make scan

dot:
	gcc dot.c -o dot -pthread

pi:
	gcc pi.c -o pi -pthread

bar:
	gcc bar.c -c -o bar.o

scan:
	gcc scan.c -o scan
