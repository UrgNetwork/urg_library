URG_C_LIB = liburg_c.a
URG_CPP_LIB = liburg_cpp.a
TARGET = $(URG_C_LIB) $(URG_CPP_LIB)

include ../build_rule.mk

CFLAGS = -g -O2 $(INCLUDES) -I../include/c -fPIC
CXXFLAGS = $(CFLAGS) -I../include/cpp

all : $(TARGET)

clean :
	$(RM) *.o $(TARGET)

$(URG_C_LIB) : \
	$(URG_C_LIB)(urg_sensor.o) \
	$(URG_C_LIB)(urg_utils.o) \
	$(URG_C_LIB)(urg_debug.o) \
	$(URG_C_LIB)(urg_connection.o) \
	$(URG_C_LIB)(urg_ring_buffer.o) \
	$(URG_C_LIB)(urg_serial.o) \
	$(URG_C_LIB)(urg_serial_utils.o) \
	$(URG_C_LIB)(urg_tcpclient.o) \

$(URG_CPP_LIB) : \
	$(URG_CPP_LIB)(ticks.o) \
	$(URG_CPP_LIB)(Urg_driver.o) \

