# Makefile for c/urg/samples
# $Id: Makefile,v ef6863dc48c6 2010/12/02 06:08:48 Satofumi $

# Compile options
SRCDIR = ../src
INCLUDEDIR = ../include
CC = gcc
CFLAGS = -g -O0 -Wall -Werror -W $(INCLUDES)
INCLUDES = -I$(INCLUDEDIR)
LDFLAGS =
LDLIBS = -lm `/bin/sh ld_wsock.sh` `/bin/sh ld_setupapi.sh`

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
	timeout_test \
	reboot_test \
	angle_convert_test \

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

get_distance get_distance_intensity get_multiecho get_multiecho_intensity calculate_xy sync_time_stamp sensor_parameter timeout_test reboot_test angle_convert_test : open_urg_sensor.o $(REQUIRE_LIB)
find_port : $(REQUIRE_LIB)

# DO NOT DELETE

open_urg_sensor.o: ../include/urg_sensor.h ../include/urg_connection.h
open_urg_sensor.o: ../include/urg_serial.h ../include/urg_detect_os.h
open_urg_sensor.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
calculate_xy.o: ../include/urg_sensor.h ../include/urg_connection.h
calculate_xy.o: ../include/urg_serial.h ../include/urg_detect_os.h
calculate_xy.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
calculate_xy.o: ../include/urg_utils.h open_urg_sensor.h
find_port.o: ../include/urg_serial_utils.h
get_distance.o: ../include/urg_sensor.h ../include/urg_connection.h
get_distance.o: ../include/urg_serial.h ../include/urg_detect_os.h
get_distance.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
get_distance.o: ../include/urg_utils.h open_urg_sensor.h
get_distance_intensity.o: ../include/urg_sensor.h ../include/urg_connection.h
get_distance_intensity.o: ../include/urg_serial.h ../include/urg_detect_os.h
get_distance_intensity.o: ../include/urg_ring_buffer.h
get_distance_intensity.o: ../include/urg_tcpclient.h ../include/urg_utils.h
get_distance_intensity.o: open_urg_sensor.h
get_multiecho.o: ../include/urg_sensor.h ../include/urg_connection.h
get_multiecho.o: ../include/urg_serial.h ../include/urg_detect_os.h
get_multiecho.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
get_multiecho.o: ../include/urg_utils.h open_urg_sensor.h
get_multiecho_intensity.o: ../include/urg_sensor.h
get_multiecho_intensity.o: ../include/urg_connection.h
get_multiecho_intensity.o: ../include/urg_serial.h ../include/urg_detect_os.h
get_multiecho_intensity.o: ../include/urg_ring_buffer.h
get_multiecho_intensity.o: ../include/urg_tcpclient.h ../include/urg_utils.h
get_multiecho_intensity.o: open_urg_sensor.h
open_urg_sensor.o: open_urg_sensor.h ../include/urg_sensor.h
open_urg_sensor.o: ../include/urg_connection.h ../include/urg_serial.h
open_urg_sensor.o: ../include/urg_detect_os.h ../include/urg_ring_buffer.h
open_urg_sensor.o: ../include/urg_tcpclient.h ../include/urg_utils.h
reboot_test.o: ../include/urg_sensor.h ../include/urg_connection.h
reboot_test.o: ../include/urg_serial.h ../include/urg_detect_os.h
reboot_test.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
reboot_test.o: ../include/urg_utils.h open_urg_sensor.h
sensor_parameter.o: ../include/urg_sensor.h ../include/urg_connection.h
sensor_parameter.o: ../include/urg_serial.h ../include/urg_detect_os.h
sensor_parameter.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
sensor_parameter.o: ../include/urg_utils.h open_urg_sensor.h
sync_time_stamp.o: ../include/urg_sensor.h ../include/urg_connection.h
sync_time_stamp.o: ../include/urg_serial.h ../include/urg_detect_os.h
sync_time_stamp.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
sync_time_stamp.o: ../include/urg_utils.h open_urg_sensor.h
timeout_test.o: ../include/urg_sensor.h ../include/urg_connection.h
timeout_test.o: ../include/urg_serial.h ../include/urg_detect_os.h
timeout_test.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
timeout_test.o: ../include/urg_utils.h ../include/urg_debug.h
timeout_test.o: open_urg_sensor.h
