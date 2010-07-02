# Makefile c/test
# $Id$

# Compile options
CFLAGS = -g -O0 -Wall -Werror -W $(INCLUDES)
INCLUDES = -I..
LDFLAGS =
LDLIBS =

# Target
TARGET = urg_sensor_test serial_test

all : $(TARGET)

clean :
	$(RM) *.o $(TARGET)

test : all
	./urg_sensor_test

depend :
	makedepend -Y -- $(INCLUDES) -- $(wildcard *.h *.c)

.PHONY : all clean test depend
######################################################################
$(TARGET) : tiny_cunit.o ../urg_communication.o ../urg_serial.o ../urg_ethernet.o ../urg_ring_buffer.o

# DO NOT DELETE

serial_test.o: ../urg_serial.h ../urg_detect_os.h ../urg_ring_buffer.h
urg_sensor_test.o: ../urg_sensor.c ../urg_sensor.h ../urg_communication.h
urg_sensor_test.o: ../urg_serial.h ../urg_detect_os.h ../urg_ring_buffer.h
urg_sensor_test.o: ../urg_errno.h tiny_cunit.h
