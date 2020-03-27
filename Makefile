#
# This makefile implements configuration specific macros and targets.
#

MKDIR=mkdir
CP=cp
GREP=grep
RM=rm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros

CND_DLIB_FILE=liberror_debug.so
CND_DISTDIR=dist

# Object Files
OBJECTFILES= \
	error_debug.o \

# Sources Files
SOURCES= \
    error_debug.cpp \

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS= -std=c++11 -fPIC -O2 -shared -fvisibility=hidden -DLIB_BUILDING_SHARED
CXXFLAGS= -std=c++11 -fPIC -O2 -shared -fvisibility=hidden -DLIB_BUILDING_SHARED

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
DEPEND_LIB=
LDLIBSOPTIONS=-L${DEPEND_LIB} -Wl,--exclude-libs,ALL

# Include Options
INCLUDEOPTIONS=

PHONY += all

# Build Targets
${CND_DISTDIR}/${CND_DLIB_FILE}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}
	${LINK.cc} -o $@ $^ ${LDLIBSOPTIONS}

${OBJECTFILES} : ${SOURCES}
	${COMPILE.cc} $^ ${INCLUDEOPTIONS}

# Clean Targets
clean::
	${RM} *.o

