# Makefile for urgwidget

all :
	cd src/ && $(MAKE)
	cd samples/ && $(MAKE)

clean :
	$(RM) -rf html html_jp
	cd src/ && $(MAKE) clean
	cd samples/ && $(MAKE) clean
	-cd viewers/ && $(MAKE) clean

html :
	doxygen Doxyfile
	doxygen Doxyfile_jp

.PHONY : all clean html
######################################################################
