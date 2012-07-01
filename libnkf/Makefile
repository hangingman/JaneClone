################################################################################
# libnkfビルド用Makefile 
# Contributor:
#	Hiroyuki Nagata <newserver002@gmail.com>
################################################################################

# target and sources
TARGET  = libnkf.a
SOURCES = utf8tbl.c libnkf.c
OBJECTS = $(SOURCES:.c=.o)

# basic command
CC		:= gcc
CCFLAGS	:= -Wall
LDFLAGS := -static
AR      := ar
ARFLAG	:= crsv
RM		:= rm
VERSION := 2.1.2
MKDIR	:= mkdir

# make all
all:	$(TARGET) $(OBJECTS)
# suffix rule
.c.o:
		$(CC) $(CCFLAGS) -c $<
# build library
$(TARGET): $(OBJECTS) $(SOURCES)
		$(AR) $(ARFLAG) $(TARGET) $(OBJECTS)
# clean
.PHONY: clean
clean:
		$(RM) -f *.o $(TARGET)