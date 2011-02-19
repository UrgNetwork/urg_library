# current/dox

CFLAGS = -g -O0 -Wall -Werror -W `urg_c-config --cflags`
LDFLAGS =
LDLIBS = `urg_c-config --libs` -lm


TARGET = \
	connect_and_get \

IMAGE_TARGET = \
	multiecho_image.png \
	sensor_angle_image.png \
	sensor_index_image.png \
	sensor_step_image.png \
	skip_scan_image.png \

all : $(TARGET) img
	cd ../ && $(MAKE) html

img : $(IMAGE_TARGET)

clean :
	$(RM) $(TARGET) $(IMAGE_TARGET)

.PHONY : all clean
######################################################################
.SUFFIXES: .png .svg
.svg.png:
	convert $< $@
