SHELL=/bin/bash -o pipefail
CC=clang
CXX=clang++
CXXFLAGS=-Wall -O2 -std=c++17 -g -flto

files=$(shell for r in `noroots bloomfilter.nw | grep -v " "`; do echo $${r:2:-2}; done)
figure_scripts=$(filter %.sh, $(files))
figures=$(figure_scripts:.sh=.fig.tex)


bloomfilter.pdf : bloomfilter.tex $(files) $(figures)
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

%.fig.tex: %.sh driver
	chmod u+x $<
	./$<

%.fig.pdf: %.fig.tex
	pdflatex -interaction=nonstopmode -jobname=$(basename $@) "\documentclass{standalone}\usepackage{tikz}\usepackage{gnuplot-lua-tikz}\begin{document}\input{$<}\end{document}"

files: $(files)
$(files): % : bloomfilter.nw
	notangle -R$* $^ | cpif $@

clean:
	rm -r -f $(files) $(deps) $(progs) *~ *.o *.tex *.output *.fig.*
	latexmk -pdf -shell-escape bloomfilter.pdf -CA -f
