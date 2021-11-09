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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Quiet.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Quiet.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Commands/digiCommand.c Commands/standardCommands.c Commands/anaiCommand.c Commands/anaoCommand.c Commands/pwmCommand.c Commands/servoCommand.c Commands/digoCommand.c mcc_generated_files/examples/i2c1_master_example.c mcc_generated_files/usb/example_mcc_usb_cdc.c mcc_generated_files/usb/usb_device_events.c mcc_generated_files/usb/usb_device.c mcc_generated_files/usb/usb_descriptors.c mcc_generated_files/usb/usb_device_cdc.c mcc_generated_files/spi2.c mcc_generated_files/epwm1.c mcc_generated_files/epwm2.c mcc_generated_files/eusart2.c mcc_generated_files/eusart1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c mcc_generated_files/tmr1.c mcc_generated_files/i2c1_master.c mcc_generated_files/tmr2.c mcc_generated_files/adc.c mcc_generated_files/device_config.c mcc_generated_files/epwm3.c main.c CLI/cli.c outputs.c analogInputs.c Commands/systemCommand.c settings.c mcc_generated_files/memory.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Commands/digiCommand.p1 ${OBJECTDIR}/Commands/standardCommands.p1 ${OBJECTDIR}/Commands/anaiCommand.p1 ${OBJECTDIR}/Commands/anaoCommand.p1 ${OBJECTDIR}/Commands/pwmCommand.p1 ${OBJECTDIR}/Commands/servoCommand.p1 ${OBJECTDIR}/Commands/digoCommand.p1 ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1 ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1 ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1 ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1 ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1 ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1 ${OBJECTDIR}/mcc_generated_files/spi2.p1 ${OBJECTDIR}/mcc_generated_files/epwm1.p1 ${OBJECTDIR}/mcc_generated_files/epwm2.p1 ${OBJECTDIR}/mcc_generated_files/eusart2.p1 ${OBJECTDIR}/mcc_generated_files/eusart1.p1 ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 ${OBJECTDIR}/mcc_generated_files/mcc.p1 ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 ${OBJECTDIR}/mcc_generated_files/tmr1.p1 ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1 ${OBJECTDIR}/mcc_generated_files/tmr2.p1 ${OBJECTDIR}/mcc_generated_files/adc.p1 ${OBJECTDIR}/mcc_generated_files/device_config.p1 ${OBJECTDIR}/mcc_generated_files/epwm3.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/CLI/cli.p1 ${OBJECTDIR}/outputs.p1 ${OBJECTDIR}/analogInputs.p1 ${OBJECTDIR}/Commands/systemCommand.p1 ${OBJECTDIR}/settings.p1 ${OBJECTDIR}/mcc_generated_files/memory.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/Commands/digiCommand.p1.d ${OBJECTDIR}/Commands/standardCommands.p1.d ${OBJECTDIR}/Commands/anaiCommand.p1.d ${OBJECTDIR}/Commands/anaoCommand.p1.d ${OBJECTDIR}/Commands/pwmCommand.p1.d ${OBJECTDIR}/Commands/servoCommand.p1.d ${OBJECTDIR}/Commands/digoCommand.p1.d ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1.d ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1.d ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1.d ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1.d ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1.d ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1.d ${OBJECTDIR}/mcc_generated_files/spi2.p1.d ${OBJECTDIR}/mcc_generated_files/epwm1.p1.d ${OBJECTDIR}/mcc_generated_files/epwm2.p1.d ${OBJECTDIR}/mcc_generated_files/eusart2.p1.d ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1.d ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d ${OBJECTDIR}/mcc_generated_files/adc.p1.d ${OBJECTDIR}/mcc_generated_files/device_config.p1.d ${OBJECTDIR}/mcc_generated_files/epwm3.p1.d ${OBJECTDIR}/main.p1.d ${OBJECTDIR}/CLI/cli.p1.d ${OBJECTDIR}/outputs.p1.d ${OBJECTDIR}/analogInputs.p1.d ${OBJECTDIR}/Commands/systemCommand.p1.d ${OBJECTDIR}/settings.p1.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Commands/digiCommand.p1 ${OBJECTDIR}/Commands/standardCommands.p1 ${OBJECTDIR}/Commands/anaiCommand.p1 ${OBJECTDIR}/Commands/anaoCommand.p1 ${OBJECTDIR}/Commands/pwmCommand.p1 ${OBJECTDIR}/Commands/servoCommand.p1 ${OBJECTDIR}/Commands/digoCommand.p1 ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1 ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1 ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1 ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1 ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1 ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1 ${OBJECTDIR}/mcc_generated_files/spi2.p1 ${OBJECTDIR}/mcc_generated_files/epwm1.p1 ${OBJECTDIR}/mcc_generated_files/epwm2.p1 ${OBJECTDIR}/mcc_generated_files/eusart2.p1 ${OBJECTDIR}/mcc_generated_files/eusart1.p1 ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 ${OBJECTDIR}/mcc_generated_files/mcc.p1 ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 ${OBJECTDIR}/mcc_generated_files/tmr1.p1 ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1 ${OBJECTDIR}/mcc_generated_files/tmr2.p1 ${OBJECTDIR}/mcc_generated_files/adc.p1 ${OBJECTDIR}/mcc_generated_files/device_config.p1 ${OBJECTDIR}/mcc_generated_files/epwm3.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/CLI/cli.p1 ${OBJECTDIR}/outputs.p1 ${OBJECTDIR}/analogInputs.p1 ${OBJECTDIR}/Commands/systemCommand.p1 ${OBJECTDIR}/settings.p1 ${OBJECTDIR}/mcc_generated_files/memory.p1

# Source Files
SOURCEFILES=Commands/digiCommand.c Commands/standardCommands.c Commands/anaiCommand.c Commands/anaoCommand.c Commands/pwmCommand.c Commands/servoCommand.c Commands/digoCommand.c mcc_generated_files/examples/i2c1_master_example.c mcc_generated_files/usb/example_mcc_usb_cdc.c mcc_generated_files/usb/usb_device_events.c mcc_generated_files/usb/usb_device.c mcc_generated_files/usb/usb_descriptors.c mcc_generated_files/usb/usb_device_cdc.c mcc_generated_files/spi2.c mcc_generated_files/epwm1.c mcc_generated_files/epwm2.c mcc_generated_files/eusart2.c mcc_generated_files/eusart1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c mcc_generated_files/tmr1.c mcc_generated_files/i2c1_master.c mcc_generated_files/tmr2.c mcc_generated_files/adc.c mcc_generated_files/device_config.c mcc_generated_files/epwm3.c main.c CLI/cli.c outputs.c analogInputs.c Commands/systemCommand.c settings.c mcc_generated_files/memory.c



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Quiet.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F46J53
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Commands/digiCommand.p1: Commands/digiCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/digiCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/digiCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/digiCommand.p1 Commands/digiCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/digiCommand.d ${OBJECTDIR}/Commands/digiCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/digiCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/standardCommands.p1: Commands/standardCommands.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/standardCommands.p1.d 
	@${RM} ${OBJECTDIR}/Commands/standardCommands.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/standardCommands.p1 Commands/standardCommands.c 
	@-${MV} ${OBJECTDIR}/Commands/standardCommands.d ${OBJECTDIR}/Commands/standardCommands.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/standardCommands.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/anaiCommand.p1: Commands/anaiCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/anaiCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/anaiCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/anaiCommand.p1 Commands/anaiCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/anaiCommand.d ${OBJECTDIR}/Commands/anaiCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/anaiCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/anaoCommand.p1: Commands/anaoCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/anaoCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/anaoCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/anaoCommand.p1 Commands/anaoCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/anaoCommand.d ${OBJECTDIR}/Commands/anaoCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/anaoCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/pwmCommand.p1: Commands/pwmCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/pwmCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/pwmCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/pwmCommand.p1 Commands/pwmCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/pwmCommand.d ${OBJECTDIR}/Commands/pwmCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/pwmCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/servoCommand.p1: Commands/servoCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/servoCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/servoCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/servoCommand.p1 Commands/servoCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/servoCommand.d ${OBJECTDIR}/Commands/servoCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/servoCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/digoCommand.p1: Commands/digoCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/digoCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/digoCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/digoCommand.p1 Commands/digoCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/digoCommand.d ${OBJECTDIR}/Commands/digoCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/digoCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1: mcc_generated_files/examples/i2c1_master_example.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/examples" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1 mcc_generated_files/examples/i2c1_master_example.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.d ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1: mcc_generated_files/usb/example_mcc_usb_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/usb" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1 mcc_generated_files/usb/example_mcc_usb_cdc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.d ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1: mcc_generated_files/usb/usb_device_events.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/usb" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1 mcc_generated_files/usb/usb_device_events.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.d ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1: mcc_generated_files/usb/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/usb" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1 mcc_generated_files/usb/usb_device.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/usb/usb_device.d ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1: mcc_generated_files/usb/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/usb" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1 mcc_generated_files/usb/usb_descriptors.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.d ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1: mcc_generated_files/usb/usb_device_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/usb" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1 mcc_generated_files/usb/usb_device_cdc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.d ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/spi2.p1: mcc_generated_files/spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/spi2.p1 mcc_generated_files/spi2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/spi2.d ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/epwm1.p1: mcc_generated_files/epwm1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/epwm1.p1 mcc_generated_files/epwm1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/epwm1.d ${OBJECTDIR}/mcc_generated_files/epwm1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/epwm1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/epwm2.p1: mcc_generated_files/epwm2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/epwm2.p1 mcc_generated_files/epwm2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/epwm2.d ${OBJECTDIR}/mcc_generated_files/epwm2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/epwm2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/eusart2.p1: mcc_generated_files/eusart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/eusart2.p1 mcc_generated_files/eusart2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/eusart2.d ${OBJECTDIR}/mcc_generated_files/eusart2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/eusart2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/eusart1.p1: mcc_generated_files/eusart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/eusart1.p1 mcc_generated_files/eusart1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/eusart1.d ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 mcc_generated_files/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/mcc.p1: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/mcc.p1 mcc_generated_files/mcc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/mcc.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pin_manager.p1: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 mcc_generated_files/pin_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pin_manager.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr1.p1: mcc_generated_files/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr1.p1 mcc_generated_files/tmr1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr1.d ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/i2c1_master.p1: mcc_generated_files/i2c1_master.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1 mcc_generated_files/i2c1_master.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/i2c1_master.d ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr2.p1: mcc_generated_files/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr2.p1 mcc_generated_files/tmr2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr2.d ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/adc.p1: mcc_generated_files/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/adc.p1 mcc_generated_files/adc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/adc.d ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/device_config.p1: mcc_generated_files/device_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/device_config.p1 mcc_generated_files/device_config.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/device_config.d ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/epwm3.p1: mcc_generated_files/epwm3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm3.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/epwm3.p1 mcc_generated_files/epwm3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/epwm3.d ${OBJECTDIR}/mcc_generated_files/epwm3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/epwm3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/CLI/cli.p1: CLI/cli.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/CLI" 
	@${RM} ${OBJECTDIR}/CLI/cli.p1.d 
	@${RM} ${OBJECTDIR}/CLI/cli.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/CLI/cli.p1 CLI/cli.c 
	@-${MV} ${OBJECTDIR}/CLI/cli.d ${OBJECTDIR}/CLI/cli.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/CLI/cli.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/outputs.p1: outputs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/outputs.p1.d 
	@${RM} ${OBJECTDIR}/outputs.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/outputs.p1 outputs.c 
	@-${MV} ${OBJECTDIR}/outputs.d ${OBJECTDIR}/outputs.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/outputs.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/analogInputs.p1: analogInputs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/analogInputs.p1.d 
	@${RM} ${OBJECTDIR}/analogInputs.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/analogInputs.p1 analogInputs.c 
	@-${MV} ${OBJECTDIR}/analogInputs.d ${OBJECTDIR}/analogInputs.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/analogInputs.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/systemCommand.p1: Commands/systemCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/systemCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/systemCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/systemCommand.p1 Commands/systemCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/systemCommand.d ${OBJECTDIR}/Commands/systemCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/systemCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/settings.p1: settings.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/settings.p1.d 
	@${RM} ${OBJECTDIR}/settings.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/settings.p1 settings.c 
	@-${MV} ${OBJECTDIR}/settings.d ${OBJECTDIR}/settings.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/settings.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/memory.p1: mcc_generated_files/memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/memory.p1 mcc_generated_files/memory.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/memory.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/Commands/digiCommand.p1: Commands/digiCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/digiCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/digiCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/digiCommand.p1 Commands/digiCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/digiCommand.d ${OBJECTDIR}/Commands/digiCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/digiCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/standardCommands.p1: Commands/standardCommands.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/standardCommands.p1.d 
	@${RM} ${OBJECTDIR}/Commands/standardCommands.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/standardCommands.p1 Commands/standardCommands.c 
	@-${MV} ${OBJECTDIR}/Commands/standardCommands.d ${OBJECTDIR}/Commands/standardCommands.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/standardCommands.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/anaiCommand.p1: Commands/anaiCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/anaiCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/anaiCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/anaiCommand.p1 Commands/anaiCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/anaiCommand.d ${OBJECTDIR}/Commands/anaiCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/anaiCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/anaoCommand.p1: Commands/anaoCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/anaoCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/anaoCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/anaoCommand.p1 Commands/anaoCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/anaoCommand.d ${OBJECTDIR}/Commands/anaoCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/anaoCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/pwmCommand.p1: Commands/pwmCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/pwmCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/pwmCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/pwmCommand.p1 Commands/pwmCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/pwmCommand.d ${OBJECTDIR}/Commands/pwmCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/pwmCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/servoCommand.p1: Commands/servoCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/servoCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/servoCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/servoCommand.p1 Commands/servoCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/servoCommand.d ${OBJECTDIR}/Commands/servoCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/servoCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/digoCommand.p1: Commands/digoCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/digoCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/digoCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/digoCommand.p1 Commands/digoCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/digoCommand.d ${OBJECTDIR}/Commands/digoCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/digoCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1: mcc_generated_files/examples/i2c1_master_example.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/examples" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1 mcc_generated_files/examples/i2c1_master_example.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.d ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/examples/i2c1_master_example.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1: mcc_generated_files/usb/example_mcc_usb_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/usb" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1 mcc_generated_files/usb/example_mcc_usb_cdc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.d ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/usb/example_mcc_usb_cdc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1: mcc_generated_files/usb/usb_device_events.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/usb" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1 mcc_generated_files/usb/usb_device_events.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.d ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_events.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1: mcc_generated_files/usb/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/usb" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1 mcc_generated_files/usb/usb_device.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/usb/usb_device.d ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/usb/usb_device.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1: mcc_generated_files/usb/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/usb" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1 mcc_generated_files/usb/usb_descriptors.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.d ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/usb/usb_descriptors.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1: mcc_generated_files/usb/usb_device_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/usb" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1 mcc_generated_files/usb/usb_device_cdc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.d ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/usb/usb_device_cdc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/spi2.p1: mcc_generated_files/spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/spi2.p1 mcc_generated_files/spi2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/spi2.d ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/epwm1.p1: mcc_generated_files/epwm1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/epwm1.p1 mcc_generated_files/epwm1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/epwm1.d ${OBJECTDIR}/mcc_generated_files/epwm1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/epwm1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/epwm2.p1: mcc_generated_files/epwm2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/epwm2.p1 mcc_generated_files/epwm2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/epwm2.d ${OBJECTDIR}/mcc_generated_files/epwm2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/epwm2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/eusart2.p1: mcc_generated_files/eusart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/eusart2.p1 mcc_generated_files/eusart2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/eusart2.d ${OBJECTDIR}/mcc_generated_files/eusart2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/eusart2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/eusart1.p1: mcc_generated_files/eusart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/eusart1.p1 mcc_generated_files/eusart1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/eusart1.d ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 mcc_generated_files/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/mcc.p1: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/mcc.p1 mcc_generated_files/mcc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/mcc.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pin_manager.p1: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 mcc_generated_files/pin_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pin_manager.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr1.p1: mcc_generated_files/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr1.p1 mcc_generated_files/tmr1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr1.d ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/i2c1_master.p1: mcc_generated_files/i2c1_master.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1 mcc_generated_files/i2c1_master.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/i2c1_master.d ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/i2c1_master.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr2.p1: mcc_generated_files/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/tmr2.p1 mcc_generated_files/tmr2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr2.d ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/adc.p1: mcc_generated_files/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/adc.p1 mcc_generated_files/adc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/adc.d ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/device_config.p1: mcc_generated_files/device_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/device_config.p1 mcc_generated_files/device_config.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/device_config.d ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/epwm3.p1: mcc_generated_files/epwm3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/epwm3.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/epwm3.p1 mcc_generated_files/epwm3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/epwm3.d ${OBJECTDIR}/mcc_generated_files/epwm3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/epwm3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/CLI/cli.p1: CLI/cli.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/CLI" 
	@${RM} ${OBJECTDIR}/CLI/cli.p1.d 
	@${RM} ${OBJECTDIR}/CLI/cli.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/CLI/cli.p1 CLI/cli.c 
	@-${MV} ${OBJECTDIR}/CLI/cli.d ${OBJECTDIR}/CLI/cli.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/CLI/cli.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/outputs.p1: outputs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/outputs.p1.d 
	@${RM} ${OBJECTDIR}/outputs.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/outputs.p1 outputs.c 
	@-${MV} ${OBJECTDIR}/outputs.d ${OBJECTDIR}/outputs.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/outputs.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/analogInputs.p1: analogInputs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/analogInputs.p1.d 
	@${RM} ${OBJECTDIR}/analogInputs.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/analogInputs.p1 analogInputs.c 
	@-${MV} ${OBJECTDIR}/analogInputs.d ${OBJECTDIR}/analogInputs.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/analogInputs.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Commands/systemCommand.p1: Commands/systemCommand.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/Commands" 
	@${RM} ${OBJECTDIR}/Commands/systemCommand.p1.d 
	@${RM} ${OBJECTDIR}/Commands/systemCommand.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/Commands/systemCommand.p1 Commands/systemCommand.c 
	@-${MV} ${OBJECTDIR}/Commands/systemCommand.d ${OBJECTDIR}/Commands/systemCommand.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Commands/systemCommand.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/settings.p1: settings.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/settings.p1.d 
	@${RM} ${OBJECTDIR}/settings.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/settings.p1 settings.c 
	@-${MV} ${OBJECTDIR}/settings.d ${OBJECTDIR}/settings.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/settings.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/memory.p1: mcc_generated_files/memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/mcc_generated_files/memory.p1 mcc_generated_files/memory.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/memory.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

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
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Quiet.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/Quiet.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/Quiet.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/Quiet.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Quiet.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/Quiet.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=bytewrite -O0 -fasmfile -Og -maddrqual=ignore -xassembler-with-cpp -I"CLI" -I"Commands" -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/Quiet.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
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
