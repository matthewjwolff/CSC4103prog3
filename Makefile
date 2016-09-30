all :
	make pi
	make dot

dot:
	gcc dot.c -o dot -pthread

pi:
	gcc pi.c -o pi -pthread
