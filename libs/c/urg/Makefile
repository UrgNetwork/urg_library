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
	$(URG_C_LIB)(urg_communication.o) \
	$(URG_C_LIB)(urg_serial_linux.o) \
	$(URG_C_LIB)(urg_serial_utils.o) \
	$(URG_C_LIB)(urg_ethernet_linux.o) \
	$(URG_C_LIB)(urg_ethernet.o) \

# DO NOT DELETE

urg_communication.o: urg_serial_t.h urg_ethernet_t.h
urg_ethernet.o: urg_ethernet_t.h
urg_sensor.o: urg_communication.h urg_serial_t.h urg_ethernet_t.h
urg_serial.o: urg_serial_t.h
urg_utils.o: urg_sensor.h urg_communication.h urg_serial_t.h urg_ethernet_t.h
urg_communication.o: urg_communication.h urg_serial_t.h urg_ethernet_t.h
urg_communication.o: urg_serial.h urg_ethernet.h
urg_sensor.o: urg_sensor.h urg_communication.h urg_serial_t.h
urg_sensor.o: urg_ethernet_t.h urg_errno.h
urg_serial_utils.o: urg_serial_utils_linux.c urg_serial_utils.h
urg_serial_utils_linux.o: urg_serial_utils.h
urg_utils.o: urg_utils.h urg_sensor.h urg_communication.h urg_serial_t.h
urg_utils.o: urg_ethernet_t.h urg_errno.h
