################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../CC3220SF_LAUNCHXL_FREERTOS.cmd 

SYSCFG_SRCS += \
../common.syscfg 

C_SRCS += \
../client_cbs.c \
./syscfg/ti_drivers_net_wifi_config.c \
./syscfg/ti_drivers_config.c \
../debug.c \
../eventQueue.c \
../main_freertos.c \
../main_motor_task.c \
../main_mqtt_task.c \
../mqtt_client_app.c \
../network_if.c \
../parseJSON.c \
../pubSubQueue.c \
../publish_timer.c \
../spi_encoder_comm.c \
../uart_term.c 

GEN_FILES += \
./syscfg/ti_drivers_net_wifi_config.c \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./client_cbs.d \
./syscfg/ti_drivers_net_wifi_config.d \
./syscfg/ti_drivers_config.d \
./debug.d \
./eventQueue.d \
./main_freertos.d \
./main_motor_task.d \
./main_mqtt_task.d \
./mqtt_client_app.d \
./network_if.d \
./parseJSON.d \
./pubSubQueue.d \
./publish_timer.d \
./spi_encoder_comm.d \
./uart_term.d 

OBJS += \
./client_cbs.obj \
./syscfg/ti_drivers_net_wifi_config.obj \
./syscfg/ti_drivers_config.obj \
./debug.obj \
./eventQueue.obj \
./main_freertos.obj \
./main_motor_task.obj \
./main_mqtt_task.obj \
./mqtt_client_app.obj \
./network_if.obj \
./parseJSON.obj \
./pubSubQueue.obj \
./publish_timer.obj \
./spi_encoder_comm.obj \
./uart_term.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"client_cbs.obj" \
"syscfg\ti_drivers_net_wifi_config.obj" \
"syscfg\ti_drivers_config.obj" \
"debug.obj" \
"eventQueue.obj" \
"main_freertos.obj" \
"main_motor_task.obj" \
"main_mqtt_task.obj" \
"mqtt_client_app.obj" \
"network_if.obj" \
"parseJSON.obj" \
"pubSubQueue.obj" \
"publish_timer.obj" \
"spi_encoder_comm.obj" \
"uart_term.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"client_cbs.d" \
"syscfg\ti_drivers_net_wifi_config.d" \
"syscfg\ti_drivers_config.d" \
"debug.d" \
"eventQueue.d" \
"main_freertos.d" \
"main_motor_task.d" \
"main_mqtt_task.d" \
"mqtt_client_app.d" \
"network_if.d" \
"parseJSON.d" \
"pubSubQueue.d" \
"publish_timer.d" \
"spi_encoder_comm.d" \
"uart_term.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_net_wifi_config.c" \
"syscfg\ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../client_cbs.c" \
"./syscfg/ti_drivers_net_wifi_config.c" \
"./syscfg/ti_drivers_config.c" \
"../debug.c" \
"../eventQueue.c" \
"../main_freertos.c" \
"../main_motor_task.c" \
"../main_mqtt_task.c" \
"../mqtt_client_app.c" \
"../network_if.c" \
"../parseJSON.c" \
"../pubSubQueue.c" \
"../publish_timer.c" \
"../spi_encoder_comm.c" \
"../uart_term.c" 

SYSCFG_SRCS__QUOTED += \
"../common.syscfg" 


