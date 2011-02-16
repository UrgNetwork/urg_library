# current/dox

CFLAGS = -g -O0 -Wall -Werror -W `urg_c-config --cflags`
LDFLAGS =
LDLIBS = `urg_c-config --libs` -lm


TARGET = connect_and_get

all : $(TARGET)
	cd ../ && $(MAKE) html

clean :
