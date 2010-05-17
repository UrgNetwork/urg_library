# Makefile for libs/c
# $Id$

# Compile options
CFLAGS = -g -O0 -Wall -Werror -W $(INCLUDES)
INCLUDES =
LDFLAGS =
LDLIBS =

# Target
URG_C_LIB = urg_c.a
TARGET = $(URG_C_LIB)

all : $(TARGET)

clean :
	$(RM) *.o $(TARGET)

depend :
	makedepend -Y -- $(INCLUDES) -- $(wildcard *.h *.c)

.PHONY : all clean depend
######################################################################
$(URG_C_LIB) : \
	$(URG_C_LIB)(urg_sensor.o) \
	$(URG_C_LIB)(urg_utils.o) \

# DO NOT DELETE

urg_utils.o: urg_sensor.h
urg_sensor.o: urg_sensor.h
urg_utils.o: urg_utils.h urg_sensor.h
