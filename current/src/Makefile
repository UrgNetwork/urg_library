# Makefile for libs/c
# $Id$

# Compile options
CC = gcc
#CFLAGS = -O2 -Wall -Werror -W $(INCLUDES)
CFLAGS = -g -O0 -Wall -Werror -W $(INCLUDES)
INCLUDES = -I../include
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
	$(URG_C_LIB)(urg_tcpclient.o) \
	$(URG_C_LIB)(urg_ring_buffer.o) \

urg_serial_utils.o : urg_serial_utils_windows.c urg_serial_utils_linux.c

# DO NOT DELETE

urg_connection.o: ../include/urg_connection.h ../include/urg_serial.h
urg_connection.o: ../include/urg_detect_os.h ../include/urg_ring_buffer.h
urg_connection.o: ../include/urg_tcpclient.h
urg_ring_buffer.o: ../include/urg_ring_buffer.h
urg_sensor.o: ../include/urg_sensor.h ../include/urg_connection.h
urg_sensor.o: ../include/urg_serial.h ../include/urg_detect_os.h
urg_sensor.o: ../include/urg_ring_buffer.h ../include/urg_tcpclient.h
urg_sensor.o: ../include/urg_errno.h
urg_serial.o: ../include/urg_serial.h ../include/urg_detect_os.h
urg_serial.o: ../include/urg_ring_buffer.h urg_serial_linux.c
urg_serial_utils.o: ../include/urg_serial_utils.h urg_serial_utils_linux.c
urg_serial_utils_linux.o: ../include/urg_serial_utils.h
urg_tcpclient.o: ../include/urg_detect_os.h ../include/urg_tcpclient.h
urg_tcpclient.o: ../include/urg_ring_buffer.h
urg_utils.o: ../include/urg_utils.h ../include/urg_sensor.h
urg_utils.o: ../include/urg_connection.h ../include/urg_serial.h
urg_utils.o: ../include/urg_detect_os.h ../include/urg_ring_buffer.h
urg_utils.o: ../include/urg_tcpclient.h ../include/urg_errno.h
