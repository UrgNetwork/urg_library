# Makefile for urgwidget

all :
	cd libs/ && $(MAKE)
	cd doxes/ && $(MAKE)

clean :
	cd libs/ && $(MAKE) clean
	cd doxes/ && $(MAKE) clean

.PHONY : all clean
######################################################################
