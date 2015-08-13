.PHONY:	version.tex fixate	all


FIX_TARGETS=	$(addsuffix .fix,$(basename $(wildcard *.tex) $(wildcard parts/*.tex)))
EPS_TARGETS=	$(addsuffix .eps,$(basename $(wildcard dia/*.dia)))
FIG_TARGETS=	$(addsuffix .fig,$(basename $(wildcard dia/*.dia)))

all:	version.tex	$(EPS_TARGETS)	$(FIX_TARGETS)	freeprio.pdf
#$(FIG_TARGETS)

version.tex:
	echo '\newcommand{\REVISION}{'`svnversion`'}' >version.tex

%.pdf:	%.tex
	latex	$*.tex
	dvips	$*.dvi
	ps2pdf	$*.ps

%.fix:
	./fix-fly.bash $*.tex

%.eps:	%.dia	Makefile
#	dia --export=$@ --size=1280x $<
	dia -t eps-builtin --export=$@ $< 
	epstopdf $@

%.fig:	%.dia
	dia --export=$@ --size=1280x $<
#	dia --export=$@ $<

