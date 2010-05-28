# Makefile for c/urg/samples
# $Id$

# Compile options
CFLAGS = -g -O0 -Wall -Werror -W $(INCLUDES)
INCLUDES = -I..
LDFLAGS =
LDLIBS = -lm

# Target
TARGET = \
	get_distance \
	get_distance_intensity \
	get_multiecho \
	get_subrange_distance \
	calculate_xy \
	sync_timestamp \
	sensor_parameter \

all : $(TARGET)

clean :
	$(RM) *.o $(TARGET)

depend :
	makedepend -Y -- $(INCLUDES) -- $(wildcard *.h *.c)

.PHONY : all depend clean
######################################################################
REQUIRE_LIB = ../urg_c.a
$(REQUIRE_LIB) :
	cd $(@D)/ && $(MAKE) $(@F)

get_distance normal_measurement intensity_measurement multiecho_measurement calculate_xy sync_timestamp sensor_parameter : $(REQUIRE_LIB)

# DO NOT DELETE

calculate_xy.o: ../urg_sensor.h ../urg_utils.h
get_distance.o: ../urg_sensor.h ../urg_utils.h
sensor_parameter.o: ../urg_sensor.h ../urg_utils.h
sync_timestamp.o: ../urg_sensor.h ../urg_utils.h
