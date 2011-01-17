REM Compile URG library

cl.exe -c -MD -I../include ../src/urg_sensor.c 
cl.exe -c -MD -I../include ../src/urg_utils.c 
cl.exe -c -MD -I../include ../src/urg_connection.c 
cl.exe -c -MD -I../include ../src/urg_serial.c 
cl.exe -c -MD -I../include ../src/urg_serial_utils.c 
cl.exe -c -MD -I../include ../src/urg_tcpclient.c 
cl.exe -c -MD -I../include ../src/urg_ring_buffer.c

REM Compile sample utility

cl.exe -c -MD -I../include ../samples/open_urg_sensor.c

REM Compile samples linking with ws2_32.lib setupapi.lib with /MD option.

cl.exe /MD -I../include ../samples/sensor_parameter.c open_urg_sensor.obj urg_sensor.obj urg_utils.obj urg_connection.obj urg_serial.obj urg_serial_utils.obj urg_tcpclient.obj urg_ring_buffer.obj ws2_32.lib setupapi.lib

cl.exe /MD -I../include ../samples/calculate_xy.c open_urg_sensor.obj urg_sensor.obj urg_utils.obj urg_connection.obj urg_serial.obj urg_serial_utils.obj urg_tcpclient.obj urg_ring_buffer.obj ws2_32.lib setupapi.lib

cl.exe /MD -I../include ../samples/get_multiecho_intensity.c open_urg_sensor.obj urg_sensor.obj urg_utils.obj urg_connection.obj urg_serial.obj urg_serial_utils.obj urg_tcpclient.obj urg_ring_buffer.obj ws2_32.lib setupapi.lib

cl.exe /MD -I../include ../samples/find_port.c open_urg_sensor.obj urg_sensor.obj urg_utils.obj urg_connection.obj urg_serial.obj urg_serial_utils.obj urg_tcpclient.obj urg_ring_buffer.obj ws2_32.lib setupapi.lib

cl.exe /MD -I../include ../samples/get_distance.c open_urg_sensor.obj urg_sensor.obj urg_utils.obj urg_connection.obj urg_serial.obj urg_serial_utils.obj urg_tcpclient.obj urg_ring_buffer.obj ws2_32.lib setupapi.lib

cl.exe /MD -I../include ../samples/get_distance_intensity.c open_urg_sensor.obj urg_sensor.obj urg_utils.obj urg_connection.obj urg_serial.obj urg_serial_utils.obj urg_tcpclient.obj urg_ring_buffer.obj ws2_32.lib setupapi.lib

cl.exe /MD -I../include ../samples/sync_time_stamp.c open_urg_sensor.obj urg_sensor.obj urg_utils.obj urg_connection.obj urg_serial.obj urg_serial_utils.obj urg_tcpclient.obj urg_ring_buffer.obj ws2_32.lib setupapi.lib

cl.exe /MD -I../include ../samples/get_multiecho.c open_urg_sensor.obj urg_sensor.obj urg_utils.obj urg_connection.obj urg_serial.obj urg_serial_utils.obj urg_tcpclient.obj urg_ring_buffer.obj ws2_32.lib setupapi.lib

