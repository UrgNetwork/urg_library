# viewers/viewer_sdl/

# Compile options
SRCDIR = ../../src
CC = gcc
CFLAGS = -g -O0 -Wall -Werror -W $(INCLUDE) `sdl-config --cflags`
INCLUDE = -I$(SRCDIR)
LDFLAGS = -L/usr/X11/lib
LDLIBS = `sdl-config --libs` -lGL

TARGET = viewer_sdl

all : $(TARGET)

clean :
	$(RM) $(TARGET) *.o

depend :
	makedepend -Y -- $(INCLUDES) -- $(wildcard *.h *.c)

.PHONY : all clean depend
######################################################################
REQUIRE_LIB = $(SRCDIR)/liburg_c.a
$(REQUIRE_LIB) : $(wildcard $(SRCDIR)/*.[ch])
	cd $(@D)/ && $(MAKE) $(@F)

viewer_sdl : plotter_sdl.o $(REQUIRE_LIB)

# DO NOT DELETE

plotter_sdl.o: plotter_sdl.h
viewer_sdl.o: plotter_sdl.h
