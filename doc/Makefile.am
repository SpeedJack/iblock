LATEXMKOPTS = -pdf -cd -use-make -shell-escape -pdflatex="pdflatex -interaction=nonstopmode %O %S"

EXTRA_DIST = $(srcdir)/*.tex $(srcdir)/*.sty $(srcdir)/**/*.tex \
	     $(srcdir)/**/**/*.tex $(srcdir)/**/*.eps $(srcdir)/**/*.pdf \
	     $(srcdir)/listings/

CLEANFILES = chapters/*.aux thesis.bbl thesis.run.xml \
	     presentation.bbl presentation.run.xml texput.fls **/*-converted-to.* \
	     *.xmpi *.xmpdata

# doc_DATA = thesis.pdf presentation.pdf
doc_DATA = thesis.pdf

.PHONY: FORCE_MAKE

thesis.pdf: $(srcdir)/thesis.tex FORCE_MAKE
	$(LATEXMK) $(LATEXMKOPTS) $<

# presentation.pdf: $(srcdir)/presentation.tex FORCE_MAKE
# 	$(LATEXMK) $(LATEXMKOPTS) $<

clean-local:
	$(LATEXMK) -C
