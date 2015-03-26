CC = g++
CHIMU_api_dir = ./CHIMUAPI
ros_project = /home/sensor/ros_workspace/robot_mover/lib
lib_name = CHIMU_device_shared_lib.so

CFLAGS = \
	-fpermissive \
	-shared \
	-c \
	-fPIC

LDLIBS = \
       -lpthread

SRCS = \
	$(CHIMU_api_dir)/CHIMUAPI.c\
	$(CHIMU_api_dir)/util.c \
	$(CHIMU_api_dir)/crc.c \
	$(CHIMU_api_dir)/endian_functions.c \
	$(CHIMU_api_dir)/globals.c \
	$(CHIMU_api_dir)/CHIMUParserClass.c \
	CHIMU_shared_lib.cpp \

all:
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) $(LDFLAGS) $(LDLIBS)
	g++ -shared -WL,-soname,$(lib_name) -o $(lib_name) *.o
	cp $(lib_name) $(ros_project)
	rm *.o
	rm -rf *~
	rm -rf $(CHIMU_api_dir)/*~

clean:
	rm -rf $(lib_name)
	rm -rf *.o *.so $(ros_project)/$(lib_name).so  

