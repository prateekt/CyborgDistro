CC = g++
Ultrasonic_api_dir = ./UltrasonicAPI
lib_name = Ultrasonic_device_shared_lib.so

CFLAGS = \
	-fpermissive \
	-shared \
	-c \
	-fPIC

LDLIBS = \
       -lpthread

SRCS = \
	$(Ultrasonic_api_dir)/UltrasonicAPI.c\
	Ultrasonic_shared_lib.cpp \

all:
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) $(LDFLAGS) $(LDLIBS)
	g++ -shared -WL,-soname,$(lib_name) -o $(lib_name) *.o
	rm *.o
	rm -rf *~
	rm -rf $(Ultrasonic_api_dir)/*~

clean:
	rm -rf $(lib_name)

