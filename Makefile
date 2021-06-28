#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := mpu6050.cpp

# header files in this project
HEADERS := mpu6050.hpp Class_xyz.hpp

# other places to look for files for this project
SEARCH  :=

# set RELATIVE to the next higher directory 
# and defer to the Makefile.* there
RELATIVE := ..
include $(RELATIVE)/Makefile.due

CFLAGS="-Wno-error=format-truncation"

