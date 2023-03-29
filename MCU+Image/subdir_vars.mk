################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DRV8833.cpp \
../adc.cpp \
../control.cpp \
../main_tirtos.cpp \
../sensing.cpp \
../water.cpp 

CMD_SRCS += \
../cc32xxs_tirtos7.cmd 

SYSCFG_SRCS += \
../control.syscfg \
../image.syscfg 

C_SRCS += \
./syscfg/ti_drivers_config.c \
./syscfg/ti_sysbios_config.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c \
./syscfg/ti_sysbios_config.c 

GEN_MISC_DIRS += \
./syscfg/ \
./syscfg/ 

C_DEPS += \
./syscfg/ti_drivers_config.d \
./syscfg/ti_sysbios_config.d 

OBJS += \
./DRV8833.obj \
./adc.obj \
./control.obj \
./syscfg/ti_drivers_config.obj \
./syscfg/ti_sysbios_config.obj \
./main_tirtos.obj \
./sensing.obj \
./water.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.genlibs \
./syscfg/syscfg_c.rov.xs \
./syscfg/ti_sysbios_config.h \
./syscfg/ti_drivers_net_wifi_config.json 

CPP_DEPS += \
./DRV8833.d \
./adc.d \
./control.d \
./main_tirtos.d \
./sensing.d \
./water.d 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" \
"syscfg\" 

OBJS__QUOTED += \
"DRV8833.obj" \
"adc.obj" \
"control.obj" \
"syscfg\ti_drivers_config.obj" \
"syscfg\ti_sysbios_config.obj" \
"main_tirtos.obj" \
"sensing.obj" \
"water.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\ti_utils_build_linker.cmd.genlibs" \
"syscfg\syscfg_c.rov.xs" \
"syscfg\ti_sysbios_config.h" \
"syscfg\ti_drivers_net_wifi_config.json" 

C_DEPS__QUOTED += \
"syscfg\ti_drivers_config.d" \
"syscfg\ti_sysbios_config.d" 

CPP_DEPS__QUOTED += \
"DRV8833.d" \
"adc.d" \
"control.d" \
"main_tirtos.d" \
"sensing.d" \
"water.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_config.c" \
"syscfg\ti_sysbios_config.c" 

CPP_SRCS__QUOTED += \
"../DRV8833.cpp" \
"../adc.cpp" \
"../control.cpp" \
"../main_tirtos.cpp" \
"../sensing.cpp" \
"../water.cpp" 

SYSCFG_SRCS__QUOTED += \
"../control.syscfg" \
"../image.syscfg" 

C_SRCS__QUOTED += \
"./syscfg/ti_drivers_config.c" \
"./syscfg/ti_sysbios_config.c" 


