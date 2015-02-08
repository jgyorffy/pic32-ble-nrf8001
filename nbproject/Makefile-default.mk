#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Bluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Bluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=ble/aci_queue.c ble/acilib.c ble/hal_aci_tl.c ble/lib_aci.c hardware_gpio.c misc/TimeDelay.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/ble/aci_queue.o ${OBJECTDIR}/ble/acilib.o ${OBJECTDIR}/ble/hal_aci_tl.o ${OBJECTDIR}/ble/lib_aci.o ${OBJECTDIR}/hardware_gpio.o ${OBJECTDIR}/misc/TimeDelay.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/ble/aci_queue.o.d ${OBJECTDIR}/ble/acilib.o.d ${OBJECTDIR}/ble/hal_aci_tl.o.d ${OBJECTDIR}/ble/lib_aci.o.d ${OBJECTDIR}/hardware_gpio.o.d ${OBJECTDIR}/misc/TimeDelay.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/ble/aci_queue.o ${OBJECTDIR}/ble/acilib.o ${OBJECTDIR}/ble/hal_aci_tl.o ${OBJECTDIR}/ble/lib_aci.o ${OBJECTDIR}/hardware_gpio.o ${OBJECTDIR}/misc/TimeDelay.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=ble/aci_queue.c ble/acilib.c ble/hal_aci_tl.c ble/lib_aci.c hardware_gpio.c misc/TimeDelay.c main.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Bluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/ble/aci_queue.o: ble/aci_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ble 
	@${RM} ${OBJECTDIR}/ble/aci_queue.o.d 
	@${RM} ${OBJECTDIR}/ble/aci_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/ble/aci_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ble/aci_queue.o.d" -o ${OBJECTDIR}/ble/aci_queue.o ble/aci_queue.c   
	
${OBJECTDIR}/ble/acilib.o: ble/acilib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ble 
	@${RM} ${OBJECTDIR}/ble/acilib.o.d 
	@${RM} ${OBJECTDIR}/ble/acilib.o 
	@${FIXDEPS} "${OBJECTDIR}/ble/acilib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ble/acilib.o.d" -o ${OBJECTDIR}/ble/acilib.o ble/acilib.c   
	
${OBJECTDIR}/ble/hal_aci_tl.o: ble/hal_aci_tl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ble 
	@${RM} ${OBJECTDIR}/ble/hal_aci_tl.o.d 
	@${RM} ${OBJECTDIR}/ble/hal_aci_tl.o 
	@${FIXDEPS} "${OBJECTDIR}/ble/hal_aci_tl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ble/hal_aci_tl.o.d" -o ${OBJECTDIR}/ble/hal_aci_tl.o ble/hal_aci_tl.c   
	
${OBJECTDIR}/ble/lib_aci.o: ble/lib_aci.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ble 
	@${RM} ${OBJECTDIR}/ble/lib_aci.o.d 
	@${RM} ${OBJECTDIR}/ble/lib_aci.o 
	@${FIXDEPS} "${OBJECTDIR}/ble/lib_aci.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ble/lib_aci.o.d" -o ${OBJECTDIR}/ble/lib_aci.o ble/lib_aci.c   
	
${OBJECTDIR}/hardware_gpio.o: hardware_gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/hardware_gpio.o.d 
	@${RM} ${OBJECTDIR}/hardware_gpio.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware_gpio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hardware_gpio.o.d" -o ${OBJECTDIR}/hardware_gpio.o hardware_gpio.c   
	
${OBJECTDIR}/misc/TimeDelay.o: misc/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/misc 
	@${RM} ${OBJECTDIR}/misc/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/misc/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/misc/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/misc/TimeDelay.o.d" -o ${OBJECTDIR}/misc/TimeDelay.o misc/TimeDelay.c   
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   
	
else
${OBJECTDIR}/ble/aci_queue.o: ble/aci_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ble 
	@${RM} ${OBJECTDIR}/ble/aci_queue.o.d 
	@${RM} ${OBJECTDIR}/ble/aci_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/ble/aci_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ble/aci_queue.o.d" -o ${OBJECTDIR}/ble/aci_queue.o ble/aci_queue.c   
	
${OBJECTDIR}/ble/acilib.o: ble/acilib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ble 
	@${RM} ${OBJECTDIR}/ble/acilib.o.d 
	@${RM} ${OBJECTDIR}/ble/acilib.o 
	@${FIXDEPS} "${OBJECTDIR}/ble/acilib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ble/acilib.o.d" -o ${OBJECTDIR}/ble/acilib.o ble/acilib.c   
	
${OBJECTDIR}/ble/hal_aci_tl.o: ble/hal_aci_tl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ble 
	@${RM} ${OBJECTDIR}/ble/hal_aci_tl.o.d 
	@${RM} ${OBJECTDIR}/ble/hal_aci_tl.o 
	@${FIXDEPS} "${OBJECTDIR}/ble/hal_aci_tl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ble/hal_aci_tl.o.d" -o ${OBJECTDIR}/ble/hal_aci_tl.o ble/hal_aci_tl.c   
	
${OBJECTDIR}/ble/lib_aci.o: ble/lib_aci.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/ble 
	@${RM} ${OBJECTDIR}/ble/lib_aci.o.d 
	@${RM} ${OBJECTDIR}/ble/lib_aci.o 
	@${FIXDEPS} "${OBJECTDIR}/ble/lib_aci.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ble/lib_aci.o.d" -o ${OBJECTDIR}/ble/lib_aci.o ble/lib_aci.c   
	
${OBJECTDIR}/hardware_gpio.o: hardware_gpio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/hardware_gpio.o.d 
	@${RM} ${OBJECTDIR}/hardware_gpio.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware_gpio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/hardware_gpio.o.d" -o ${OBJECTDIR}/hardware_gpio.o hardware_gpio.c   
	
${OBJECTDIR}/misc/TimeDelay.o: misc/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/misc 
	@${RM} ${OBJECTDIR}/misc/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/misc/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/misc/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/misc/TimeDelay.o.d" -o ${OBJECTDIR}/misc/TimeDelay.o misc/TimeDelay.c   
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Bluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PIC32MXSK=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Bluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}              -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PIC32MXSK=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Bluetooth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Bluetooth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Bluetooth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
