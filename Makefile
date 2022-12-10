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

experiment: $(deps)
falsepositiveexperiment: $(deps)
setapplicationexperiment: $(deps)
bitcounttest: $(deps)

bloomfilter.tex : bloomfilter.nw
		./myweave.sh $< > $@

datagen:

files: $(files)
$(files): % : bloomfilter.nw
	notangle -R$* $^ | cpif $@

clean:
	latexmk -pdf -shell-escape bloomfilter -CA
	rm -r -f $(files) $(deps) $(progs) *~ *.o *.tex