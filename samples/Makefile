# Makefile for c/urg/samples
# $Id$

# Compile options
CC = gcc
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

calculate_xy.o: ../src/urg_sensor.h ../src/urg_connection.h
calculate_xy.o: ../src/urg_serial.h ../src/urg_detect_os.h
calculate_xy.o: ../src/urg_ring_buffer.h ../src/urg_ethernet.h
calculate_xy.o: ../src/urg_utils.h
get_distance.o: ../src/urg_sensor.h ../src/urg_connection.h
get_distance.o: ../src/urg_serial.h ../src/urg_detect_os.h
get_distance.o: ../src/urg_ring_buffer.h ../src/urg_ethernet.h
get_distance.o: ../src/urg_utils.h
get_distance_intensity.o: ../src/urg_sensor.h ../src/urg_connection.h
get_distance_intensity.o: ../src/urg_serial.h ../src/urg_detect_os.h
get_distance_intensity.o: ../src/urg_ring_buffer.h ../src/urg_ethernet.h
get_distance_intensity.o: ../src/urg_utils.h
get_multiecho.o: ../src/urg_sensor.h ../src/urg_connection.h
get_multiecho.o: ../src/urg_serial.h ../src/urg_detect_os.h
get_multiecho.o: ../src/urg_ring_buffer.h ../src/urg_ethernet.h
get_multiecho.o: ../src/urg_utils.h
get_multiecho_intensity.o: ../src/urg_sensor.h ../src/urg_connection.h
get_multiecho_intensity.o: ../src/urg_serial.h ../src/urg_detect_os.h
get_multiecho_intensity.o: ../src/urg_ring_buffer.h ../src/urg_ethernet.h
get_multiecho_intensity.o: ../src/urg_utils.h
sensor_parameter.o: ../src/urg_sensor.h ../src/urg_connection.h
sensor_parameter.o: ../src/urg_serial.h ../src/urg_detect_os.h
sensor_parameter.o: ../src/urg_ring_buffer.h ../src/urg_ethernet.h
sensor_parameter.o: ../src/urg_utils.h
sync_time_stamp.o: ../src/urg_sensor.h ../src/urg_connection.h
sync_time_stamp.o: ../src/urg_serial.h ../src/urg_detect_os.h
sync_time_stamp.o: ../src/urg_ring_buffer.h ../src/urg_ethernet.h
sync_time_stamp.o: ../src/urg_utils.h
