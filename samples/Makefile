# Makefile for c/urg/samples
# $Id$

# Compile options
CFLAGS = -g -O0 -Wall -Werror -W $(INCLUDES)
SRCDIR = ../src
INCLUDES = -I$(SRCDIR)
LDFLAGS =
LDLIBS = -lm

# Target
TARGET = \
	get_distance \
	get_distance_intensity \
	get_multiecho \
	get_multiecho_intensity \
	calculate_xy \
	sync_time_stamp \
	sensor_parameter \

all : $(TARGET)

clean :
	$(RM) *.o $(TARGET)

depend :
	makedepend -Y -- $(INCLUDES) -- $(wildcard *.h *.c)

.PHONY : all depend clean
######################################################################
REQUIRE_LIB = $(SRCDIR)/liburg_c.a
$(REQUIRE_LIB) :
	cd $(@D)/ && $(MAKE) $(@F)

get_distance get_distance_intensity get_multiecho get_multiecho_intensity calculate_xy sync_time_stamp sensor_parameter : $(REQUIRE_LIB)

# DO NOT DELETE

calculate_xy.o: $(SRCDIR)/urg_sensor.h $(SRCDIR)/urg_connection.h $(SRCDIR)/urg_serial.h
calculate_xy.o: $(SRCDIR)/urg_detect_os.h $(SRCDIR)/urg_ring_buffer.h $(SRCDIR)/urg_ethernet.h
calculate_xy.o: $(SRCDIR)/urg_utils.h
get_distance.o: $(SRCDIR)/urg_sensor.h $(SRCDIR)/urg_connection.h $(SRCDIR)/urg_serial.h
get_distance.o: $(SRCDIR)/urg_detect_os.h $(SRCDIR)/urg_ring_buffer.h $(SRCDIR)/urg_ethernet.h
get_distance.o: $(SRCDIR)/urg_utils.h
sensor_parameter.o: $(SRCDIR)/urg_sensor.h $(SRCDIR)/urg_connection.h $(SRCDIR)/urg_serial.h
sensor_parameter.o: $(SRCDIR)/urg_detect_os.h $(SRCDIR)/urg_ring_buffer.h $(SRCDIR)/urg_ethernet.h
sensor_parameter.o: $(SRCDIR)/urg_utils.h
sync_time_stamp.o: $(SRCDIR)/urg_sensor.h $(SRCDIR)/urg_connection.h $(SRCDIR)/urg_serial.h
sync_time_stamp.o: $(SRCDIR)/urg_detect_os.h $(SRCDIR)/urg_ring_buffer.h $(SRCDIR)/urg_ethernet.h
sync_time_stamp.o: $(SRCDIR)/urg_utils.h
