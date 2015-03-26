CC = g++
ocean_device_api_dir = ./OceanDeviceAPI
ros_project = /home/sensor/ros_workspace/robot_mover/lib
lib_name = ocean_device_shared_lib.so

CFLAGS = \
	-fpermissive\
	-shared \
	-c \
	-fPIC

LDLIBS = \
       -lpthread

SRCS = \
	$(ocean_device_api_dir)/OceanDeviceAPI.c\
	ocean_shared_lib.cpp
all:
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) $(LDFLAGS) $(LDLIBS)
	g++ -shared -WL,-soname,$(lib_name) -o $(lib_name) *.o
	cp $(lib_name) $(ros_project)
	rm *.o
	rm -rf *~
	rm -rf $(ocean_device_api_dir)/*~

clean:
	rm -rf $(ocean_device_api_dir)/$(lib_name)
	rm -rf *.o *.so $(ros_project)/$(lib_name)  

