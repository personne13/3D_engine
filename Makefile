all: usage

compile:
	cd build && cmake .. && make && cd ..

run:
	./bin/test_engine

clean:
	rm -rf bin/* build/*

usage:
	echo "Usage : make compile, make run"
