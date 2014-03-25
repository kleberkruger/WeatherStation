#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin_4.x-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/ConfigFile/ConfigFile.o \
	${OBJECTDIR}/src/ConfigFile/ConfigList.o \
	${OBJECTDIR}/src/Logger/Logger.o \
	${OBJECTDIR}/src/Main.o \
	${OBJECTDIR}/src/MbedConnector.o \
	${OBJECTDIR}/src/ReadingData.o \
	${OBJECTDIR}/src/TestMonitor.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mbed_monitor.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mbed_monitor.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mbed_monitor ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/ConfigFile/ConfigFile.o: src/ConfigFile/ConfigFile.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/ConfigFile
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ConfigFile/ConfigFile.o src/ConfigFile/ConfigFile.cpp

${OBJECTDIR}/src/ConfigFile/ConfigList.o: src/ConfigFile/ConfigList.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/ConfigFile
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ConfigFile/ConfigList.o src/ConfigFile/ConfigList.cpp

${OBJECTDIR}/src/Logger/Logger.o: src/Logger/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Logger
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Logger/Logger.o src/Logger/Logger.cpp

${OBJECTDIR}/src/Main.o: src/Main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Main.o src/Main.cpp

${OBJECTDIR}/src/MbedConnector.o: src/MbedConnector.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/MbedConnector.o src/MbedConnector.cpp

${OBJECTDIR}/src/ReadingData.o: src/ReadingData.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ReadingData.o src/ReadingData.cpp

${OBJECTDIR}/src/TestMonitor.o: src/TestMonitor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/TestMonitor.o src/TestMonitor.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mbed_monitor.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
