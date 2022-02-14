emitter: emitter.cpp emitter.h
	g++ -I/usr/local/include/msgpack -I/usr/include/jsoncpp -fpic -c emitter.cpp -o emitter.o -ljsoncpp -lhiredis -lboost_any
	g++ -shared -o emitter.so emitter.o

test: test.cpp emitter
	LD_LIBRARY_PATH=. g++ test.cpp emitter.cpp -I/usr/local/include/msgpack -I/usr/include/jsoncpp -L. -ljsoncpp -lhiredis
