all: usage

compile:
	cd build && cmake .. && make && cd ..

run:
	./bin/test_engine

usage:
	echo "Usage : make compile, make run"
