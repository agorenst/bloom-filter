SHELL=/bin/bash -o pipefail
CC=clang
CXX=clang++
CXXFLAGS=-Wall -O2 -std=c++17 -g

files=$(shell for r in `noroots bloomfilter.nw | grep -v " "`; do echo $${r:2:-2}; done)

bloomfilter.pdf : bloomfilter.tex $(files)
		latexmk -pdf -shell-escape $(basename $<)

deps=datagen.o murmur/MurmurHash3.o
progs=experiment falsepositiveexperiment setapplicationexperiment bitcounttest
experiments: $(files) $(progs)

driver: $(deps)

experiment: $(deps)
falsepositiveexperiment: $(deps)
setapplicationexperiment: $(deps)
bitcounttest: $(deps)

bloomfilter.tex : bloomfilter.nw
		./myweave.sh $< > $@

datagen:

density.output: driver
	./driver -d -e 0.01 -n 10000 -l 24 > density.output
	./driver -d -e 0.01 -n 10000 -l 24 >> density.output
	./driver -d -e 0.01 -n 10000 -l 24 >> density.output
	./driver -d -e 0.1 -n 10000 -l 24 >> density.output
	./driver -d -e 0.1 -n 10000 -l 24 >> density.output
	./driver -d -e 0.1 -n 10000 -l 24 >> density.output

files: $(files)
$(files): % : bloomfilter.nw
	notangle -R$* $^ | cpif $@

clean:
	rm -r -f $(files) $(deps) $(progs) *~ *.o *.tex
	latexmk -pdf -shell-escape bloomfilter -CA