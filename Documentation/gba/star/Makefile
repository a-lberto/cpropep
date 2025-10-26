SITE = antoine@rocketworkbench.sourceforge.net:/home/groups/rocketworkbench/htdocs/grain/star/
DOCSITE = antoine@rocketworkbench.sourceforge.net:/home/groups/rocketworkbench/htdocs/grain/doc/
DOC = star
PS  = $(DOC).ps
PDF = $(DOC).pdf

TEX = $(DOC).tex

all: $(PS) $(PDF)


$(PS):
	latex $(TEX)
	latex $(TEX)
	dvips -f < $(DOC).dvi > $(PS)

$(PDF): $(PS)
	ps2pdf $(PS)

html:
	latex2html -white -image_type gif -no_navigation -split 0 -dir html -mkdir $(TEX)

upload:
	scp html/* $(SITE)
	scp $(PS) $(PDF) $(DOCSITE)

clean:
	rm -f *~ *.{aux,log,toc,dvi} $(PS) $(PDF)
