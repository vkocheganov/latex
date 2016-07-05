FILE = `cat NAME`


all:
	pdflatex $(FILE)
	pdflatex $(FILE)
	-bibtex $(FILE)
	pdflatex $(FILE)
	pdflatex $(FILE)
	mkdir -p out
	mv $(FILE).pdf out

clean:
	-rm -f *.{log,toc,tac,aux,dvi,ps,bbl,blg,tmp,nav,out,snm,vrb,rel,thm,rel,*~} $(FILE).pdf *eps-converted-to.pdf *.synctex.gz
	-rm -f split*
	rm -rf out auto
	for i in $(ALLSUBDIRS); do \
	    (cd $$i; make clean) || exit 1; \
	done
