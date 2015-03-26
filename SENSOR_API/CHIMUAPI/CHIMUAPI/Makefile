CC = g++

SRCS = \
	compass_test_case.c \
	util.c \
	crc.c \
	endian_functions.c \
	globals.c \
	CHIMUParserClass.c \
	CHIMUAPI.c

OUTPUT = runner

all:
	$(CC) -fpermissive $(CFLAGS) $(INCLUDE) $(SRCS) $(LDFLAGS) -lpthread -o $(OUTPUT)

clean:
	rm -rf *.o runner 
	rm $(OUTPUT)
