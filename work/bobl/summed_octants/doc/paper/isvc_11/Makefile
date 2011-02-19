DOCNAME = paper

PRINT = lpr
PRINTOPTS =

PDF_PREVIEW = evince

default: $(DOCNAME).pdf

clean:
	rm -rf auto/ *.bbl *.blg *.dvi *.aux *.log *.dlog $(DOCNAME).ps $(GENFILES) *~

immaculate: clean
	rm -f $(DOCNAME).pdf

preview: $(DOCNAME).pdf
	$(PDF_PREVIEW) $<

print: $(DOCNAME).pdf
	$(PRINT) $(PRINTOPTS) $<

$(DOCNAME).pdf: $(DOCNAME).tex
# if there's a biblography, enable these:
	pdflatex $<
	bibtex $(subst .tex,,$<)
	pdflatex $<
	pdflatex $<
