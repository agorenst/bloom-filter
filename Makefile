SHELL=/bin/bash -o pipefail
CC=clang
CXX=clang++
CXXFLAGS=-Wall -O2 -std=c++17 -g

files=$(shell for r in `noroots bloomfilter.nw | grep -v " "`; do echo $${r:2:-2}; done)

bloomfilter.pdf : bloomfilter.tex $(files)
		pdflatex -shell-escape $(basename $<)
		biber $(basename $<)
		pdflatex -shell-escape $(basename $<)
		pdflatex -shell-escape $(basename $<)
experiment: datagen.o murmur/MurmurHash3.o
bloom_filter: datagen.o murmur/MurmurHash3.o

bloomfilter.tex : bloomfilter.nw
		./myweave.sh $< > $@

datagen:

files: $(files)
$(files): % : bloomfilter.nw
	notangle -R$* $^ | cpif $@

clean:
	rm -r -f bloom_filter experiment datagen *~ *.o murmur/*.o *.pdf *.bcf *.aux *.bbl *.log *.blg