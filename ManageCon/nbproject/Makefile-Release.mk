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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/377180152/Client.o \
	${OBJECTDIR}/_ext/377180152/Input.o \
	${OBJECTDIR}/_ext/207716480/Input.o \
	${OBJECTDIR}/_ext/207716480/Server.o \
	${OBJECTDIR}/client.o \
	${OBJECTDIR}/server.o \
	${OBJECTDIR}/test.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/managecon.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/managecon.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/managecon ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/377180152/Client.o: ../../../Desktop/Online_Test/Client_Side/Client.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/377180152
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/377180152/Client.o ../../../Desktop/Online_Test/Client_Side/Client.cpp

${OBJECTDIR}/_ext/377180152/Input.o: ../../../Desktop/Online_Test/Client_Side/Input.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/377180152
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/377180152/Input.o ../../../Desktop/Online_Test/Client_Side/Input.cpp

${OBJECTDIR}/_ext/207716480/Input.o: ../../../Desktop/Online_Test/Server_Side/Input.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/207716480
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/207716480/Input.o ../../../Desktop/Online_Test/Server_Side/Input.cpp

${OBJECTDIR}/_ext/207716480/Server.o: ../../../Desktop/Online_Test/Server_Side/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/207716480
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/207716480/Server.o ../../../Desktop/Online_Test/Server_Side/Server.cpp

${OBJECTDIR}/client.o: client.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/client.o client.cpp

${OBJECTDIR}/server.o: server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/server.o server.cpp

${OBJECTDIR}/test.o: test.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/test.o test.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/managecon.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
