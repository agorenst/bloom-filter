bloom_filter: bloom_filter.cpp
	clang++ -std=c++11 -Wall simple_bit_set.cpp -o bloom_filter

clean:
	rm -r -f bloom_filter *~ *.o
