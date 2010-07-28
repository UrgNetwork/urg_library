# Makefile for libs/c
# $Id$

# Compile options
CC = gcc
CFLAGS = -g -O0 -Wall -Werror -W $(INCLUDES)
INCLUDES =
LDFLAGS =
LDLIBS =

# Target
URG_C_LIB = liburg_c.a
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
	$(URG_C_LIB)(urg_connection.o) \
	$(URG_C_LIB)(urg_serial.o) \
	$(URG_C_LIB)(urg_serial_utils.o) \
	$(URG_C_LIB)(urg_ethernet_linux.o) \
	$(URG_C_LIB)(urg_ethernet.o) \
	$(URG_C_LIB)(urg_ring_buffer.o) \

# DO NOT DELETE

urg_connection.o: urg_serial.h urg_detect_os.h urg_ring_buffer.h
urg_connection.o: urg_ethernet.h
urg_sensor.o: urg_connection.h urg_serial.h urg_detect_os.h urg_ring_buffer.h
urg_sensor.o: urg_ethernet.h
urg_serial.o: urg_detect_os.h urg_ring_buffer.h
urg_utils.o: urg_sensor.h urg_connection.h urg_serial.h urg_detect_os.h
urg_utils.o: urg_ring_buffer.h urg_ethernet.h
urg_connection.o: urg_connection.h urg_serial.h urg_detect_os.h
urg_connection.o: urg_ring_buffer.h urg_ethernet.h
urg_ethernet.o: urg_ethernet_linux.c urg_ethernet.h
urg_ethernet_linux.o: urg_ethernet.h
urg_ethernet_windows.o: urg_ethernet.h
urg_ring_buffer.o: urg_ring_buffer.h
urg_sensor.o: urg_sensor.h urg_connection.h urg_serial.h urg_detect_os.h
urg_sensor.o: urg_ring_buffer.h urg_ethernet.h urg_errno.h
urg_serial.o: urg_serial.h urg_detect_os.h urg_ring_buffer.h
urg_serial.o: urg_serial_linux.c
urg_serial_utils.o: urg_serial_utils.h urg_serial_utils_linux.c
urg_serial_utils_linux.o: urg_serial_utils.h
urg_utils.o: urg_utils.h urg_sensor.h urg_connection.h urg_serial.h
urg_utils.o: urg_detect_os.h urg_ring_buffer.h urg_ethernet.h urg_errno.h
