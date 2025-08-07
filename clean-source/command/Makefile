########################################################################### 
# Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,              #
# the Japan Society for the Promotion of Science (JSPS)                   #
# Copyright (C) 2006-2007 ADVENTURE Project                               #
# All Rights Reserved                                                     #
########################################################################### 

#
#  ADVENTURE CAD (advcad)
#

# WH library directory
WH_HEADER_DIR = ..
WH_LIBRARY_DIR = ../WH

INCLUDES = -I$(WH_HEADER_DIR)
LIBS = -L$(WH_LIBRARY_DIR) -lWH -lm

CC = g++
#CFLAGS = $(INCLUDES) -g -Wall
# Next line modified in Ver. 0.11b
CFLAGS = $(INCLUDES) -O -DWH_PRE_ONLY -DNDEBUG -DWH_INLINE_ENABLED
LDFLAGS = $(LIBS)

.SUFFIXES: .cc

.cc.o:
	$(CC) -c $< $(CFLAGS)



all : command



COMMAND = advcad
command : $(COMMAND)

advcad.o   : 
advcad     : advcad.o 
	$(CC) -o $@ $^ $(LDFLAGS)



clean :
	rm -f *.o *.exe $(COMMAND)
