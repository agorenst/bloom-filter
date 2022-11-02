CC=clang
CXX=clang++
CXXFLAGS=-Wall -O2
bloomfilter.pdf : bloomfilter.tex
		pdflatex $(basename $<)
experiment: datagen.o murmur/MurmurHash3.o
bloom_filter: datagen.o murmur/MurmurHash3.o

bloomfilter.tex : bloomfilter.nw
		./myweave.sh $< > $@

datagen:

clean:
	rm -r -f bloom_filter datagen *~ *.o murmur/*.o