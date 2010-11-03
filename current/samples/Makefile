# Makefile for c/urg/samples
# $Id$

# Compile options
SRCDIR = ../src
INCLUDEDIR = ../include
CC = gcc
CFLAGS = -g -O0 -Wall -Werror -W $(INCLUDES)
INCLUDES = -I$(INCLUDEDIR)
LDFLAGS =
LDLIBS = -lm `/bin/sh ld_wsock.sh` `/bin/sh ld_rt.sh` `/bin/sh ld_setupapi.sh`

# Target
TARGET = \
	get_distance \
	get_distance_intensity \
	get_multiecho \
	get_multiecho_intensity \
	calculate_xy \
	sync_time_stamp \
	sensor_parameter \
	find_port \

all : $(TARGET)

clean :
	$(RM) *.o $(TARGET)

depend :
	makedepend -Y -- $(INCLUDES) -- $(wildcard *.h *.c)

.PHONY : all depend clean
######################################################################
REQUIRE_LIB = $(SRCDIR)/liburg_c.a
$(REQUIRE_LIB) : $(wildcard $(SRCDIR)/*.[ch])
	cd $(@D)/ && $(MAKE) $(@F)

get_distance get_distance_intensity get_multiecho get_multiecho_intensity calculate_xy sync_time_stamp sensor_parameter find_port : $(REQUIRE_LIB)

# DO NOT DELETE

calculate_xy.o: ../include/urg_sensor.h ../include/urg_connection.h
calculate_xy.o: ../include/urg_serial.h ../include/urg_detect_os.h
calculate_xy.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
calculate_xy.o: ../include/urg_utils.h
get_distance.o: ../include/urg_sensor.h ../include/urg_connection.h
get_distance.o: ../include/urg_serial.h ../include/urg_detect_os.h
get_distance.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
get_distance.o: ../include/urg_utils.h
get_distance_intensity.o: ../include/urg_sensor.h ../include/urg_connection.h
get_distance_intensity.o: ../include/urg_serial.h ../include/urg_detect_os.h
get_distance_intensity.o: ../include/urg_ring_buffer.h
get_distance_intensity.o: ../include/urg_tcpclient.h ../include/urg_utils.h
get_multiecho.o: ../include/urg_sensor.h ../include/urg_connection.h
get_multiecho.o: ../include/urg_serial.h ../include/urg_detect_os.h
get_multiecho.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
get_multiecho.o: ../include/urg_utils.h
get_multiecho_intensity.o: ../include/urg_sensor.h
get_multiecho_intensity.o: ../include/urg_connection.h
get_multiecho_intensity.o: ../include/urg_serial.h ../include/urg_detect_os.h
get_multiecho_intensity.o: ../include/urg_ring_buffer.h
get_multiecho_intensity.o: ../include/urg_tcpclient.h ../include/urg_utils.h
sensor_parameter.o: ../include/urg_sensor.h ../include/urg_connection.h
sensor_parameter.o: ../include/urg_serial.h ../include/urg_detect_os.h
sensor_parameter.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
sensor_parameter.o: ../include/urg_utils.h
sync_time_stamp.o: ../include/urg_sensor.h ../include/urg_connection.h
sync_time_stamp.o: ../include/urg_serial.h ../include/urg_detect_os.h
sync_time_stamp.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
sync_time_stamp.o: ../include/urg_utils.h
