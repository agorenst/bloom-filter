SHELL=/bin/bash -o pipefail
CC=clang
CXX=clang++
CXXFLAGS=-Wall -O2 -std=c++17 -g -flto

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

density.output: driver bitdensity.sh
	chmod u+x bitdensity.sh
	./bitdensity.sh

files: $(files)
$(files): % : bloomfilter.nw
	notangle -R$* $^ | cpif $@

clean:
	rm -r -f $(files) $(deps) $(progs) *~ *.o *.tex
	latexmk -pdf -shell-escape bloomfilter.pdf -CA
