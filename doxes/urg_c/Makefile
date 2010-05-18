# Makefile for doxes/urg_c/

HTML_DIR = generated_html

all : $(HTML_DIR)/index.html

clean :
	$(RM) -rf $(HTML_DIR)

.PHONY : all clean
######################################################################
$(HTML_DIR)/index.html : $(wildcard *.dox ../../libs/c/urg/*.[ch] ../../libs/c/urg/samples/*.[ch]) Doxyfile
	mkdir -p $(HTML_DIR)
	cp img/icon_*.png $(HTML_DIR)
	doxygen
