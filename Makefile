CXXFLAGS=-Wall -O2
bloom_filter: datagen.o murmur/MurmurHash3.o

datagen:

clean:
	rm -r -f bloom_filter datagen *~ *.o murmur/*.o
