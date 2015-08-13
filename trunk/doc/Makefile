.PHONY:	version.tex fixate	all
.SECONDARY:


FIX_TARGETS=	$(addsuffix .fix,$(basename $(wildcard *.tex) $(wildcard parts/*.tex)))
EPS_TARGETS=	$(addsuffix .eps,$(basename $(wildcard dia/*.dia)))
FIG_TARGETS=	$(addsuffix .fig,$(basename $(wildcard dia/*.dia)))

#PDF_TARGETS=	
PDF_TARGETS=	nw.pdf	nw.pres-10.pdf

current:	$(FIX_TARGETS)	nw.pdf
#current:	$(FIX_TARGETS)	nw.pres-11.pdf

all:	version.tex	$(EPS_TARGETS)	$(FIX_TARGETS)	$(PDF_TARGETS)
#nw.pdf nw.pres-10.pdf
#$(FIG_TARGETS)

version.tex:
	echo '\newcommand{\REVISION}{'`svnversion`'}' >version.tex

nw.pres-10.pdf:	nw.pres-10.tex
	pdflatex $^

nw.pres-11.pdf:	nw.pres-11.tex
	pdflatex $^

%.pdf:	%.tex
#	pdflatex $*.tex
	latex	$*.tex
	dvips	$*.dvi
	ps2pdf14	$*.ps

%.fix:
	./fix-fly.bash $*.tex

%.eps:	%.dia	Makefile
#	dia --export=$@ --size=1280x $<
	dia -t eps-builtin --export=$@ $< 
	epstopdf $@

%.fig:	%.dia
	dia --export=$@ --size=1280x $<
#	dia --export=$@ $<



sync:
	rsync -aCcrvvz --progress --rsh ssh nagyz@pandora.inf.elte.hu:docs/./ docs/

pic/%.jpg:	pic0/%.png
	convert $^ -negate -gamma 0.3 -crop 10000x10000+0+64 $@
