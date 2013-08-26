# Makefile for c/urg/samples
# $Id$

# Compile options
SRCDIR = ../../src
INCLUDEDIR = ../../include/cpp
CC = gcc
CXX = g++
CFLAGS = -g -O0 -Wall -Werror -W $(INCLUDES)
CXXFLAGS = $(CFLAGS)
INCLUDES = -I$(INCLUDEDIR)
LDFLAGS =
LDLIBS = -lm $(shell if test x$(MSYSTEM) = x"MINGW32"; then echo "-lwsock32 -lsetupapi"; else echo "-lrt"; fi) -L$(SRCDIR)

# Target
TARGET = \
	get_distance \
	get_distance_intensity \
	get_multiecho \
	get_multiecho_intensity \
	sync_time_stamp \
	sensor_parameter \

all : $(TARGET)

clean :
	$(RM) *.o $(TARGET) *.exe

depend :
	makedepend -Y -- $(INCLUDES) -- $(wildcard *.h *.c)

.PHONY : all depend clean

######################################################################
REQUIRE_LIB = $(SRCDIR)/liburg_cpp.a
$(REQUIRE_LIB) : $(wildcard $(SRCDIR)/*.[ch])
	cd $(@D)/ && $(MAKE) $(@F)

get_distance get_distance_intensity get_multiecho get_multiecho_intensity sync_time_stamp sensor_parameter : Connection_information.o $(REQUIRE_LIB)
