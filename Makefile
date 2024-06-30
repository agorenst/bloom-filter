SHELL=/bin/bash -o pipefail
CC=clang
CXX=clang++
CXXFLAGS=-Wall -O2 -std=c++17 -g -flto

files=$(shell for r in `noroots bloomfilter.nw | grep -v " "`; do echo $${r:2:-2}; done)
figure_scripts=$(filter %.sh, $(files))
figures=$(figure_scripts:.sh=.fig.tex)

# string_generator:

bloomfilter.pdf : bloomfilter.tex library.bib $(files) $(figures)
		latexmk -pdf -shell-escape $(basename $<)

files: $(files)
$(files): % : bloomfilter.nw
	notangle -R$* $^ | sed -e '/% HIDDEN/d' | cpif $@

deps=murmur/MurmurHash3.o

driver: $(deps)

bloomfilter.tex : bloomfilter.nw
		./myweave.sh $< > $@

# datagen:

%.fig.tex: %.sh driver
	chmod u+x $<
	./$<

%.fig.pdf: %.fig.tex
	pdflatex -interaction=nonstopmode -jobname=$(basename $@) "\documentclass{standalone}\usepackage{tikz}\usepackage{gnuplot-lua-tikz}\begin{document}\input{$<}\end{document}"


clean:
	rm -r -f $(files) $(deps) driver *~ *.o *.tex *.output *.fig.*
	latexmk -pdf -shell-escape bloomfilter.pdf -CA -f
	rm -r -f *.bbl *.run.xml _minted-bloomfilter
