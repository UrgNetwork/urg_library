# Makefile for libs/c
# $Id: Makefile,v 4537e5141a86 2011/02/17 07:10:04 Satofumi $

# Compile options
CC = gcc
CXX = g++
#CFLAGS = -O2 -Wall -Werror -W $(INCLUDES)
CFLAGS = -g -O0 -Wall -Werror -W -I../include/c $(INCLUDES)
CXXFLAGS = $(CFLAGS) -I../include/cpp
INCLUDES =
LDFLAGS =
LDLIBS =

# Target
URG_C_LIB = liburg_c.a
URG_CPP_LIB = liburg_cpp.a
TARGET = $(URG_C_LIB) $(URG_CPP_LIB)

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
	$(URG_C_LIB)(urg_debug.o) \
	$(URG_C_LIB)(urg_connection.o) \
	$(URG_C_LIB)(urg_serial.o) \
	$(URG_C_LIB)(urg_serial_utils.o) \
	$(URG_C_LIB)(urg_tcpclient.o) \
	$(URG_C_LIB)(urg_ring_buffer.o) \

$(URG_CPP_LIB) : \
	$(URG_CPP_LIB)(ticks.o) \
	$(URG_CPP_LIB)(Urg_driver.o) \

urg_serial_utils.o : urg_serial_utils_windows.c urg_serial_utils_linux.c
