CFLAGS = -std=c++17 -O3 -Wall 
OBJS = source.o 

all: ${OBJS}
	g++ ${CFLAGS} ${OBJS} -o migSim.model

main.o: ./source.cpp
	g++ ${CFLAGS} -c .source.cpp

clean:
	rm -f migSim.model ${OBJS}
