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
../SPItoMotorEncoder.c \
../UartToMotorController.c \
../client_cbs.c \
./syscfg/ti_drivers_net_wifi_config.c \
./syscfg/ti_drivers_config.c \
../debug.c \
../eventQueue.c \
../main_freertos.c \
../main_mqtt_task.c \
../maintask.c \
../mqtt_client_app.c \
../network_if.c \
../parseJSON.c \
../pubSubQueue.c \
../publish_timer.c \
../uart_term.c 

GEN_FILES += \
./syscfg/ti_drivers_net_wifi_config.c \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./SPItoMotorEncoder.d \
./UartToMotorController.d \
./client_cbs.d \
./syscfg/ti_drivers_net_wifi_config.d \
./syscfg/ti_drivers_config.d \
./debug.d \
./eventQueue.d \
./main_freertos.d \
./main_mqtt_task.d \
./maintask.d \
./mqtt_client_app.d \
./network_if.d \
./parseJSON.d \
./pubSubQueue.d \
./publish_timer.d \
./uart_term.d 

OBJS += \
./SPItoMotorEncoder.obj \
./UartToMotorController.obj \
./client_cbs.obj \
./syscfg/ti_drivers_net_wifi_config.obj \
./syscfg/ti_drivers_config.obj \
./debug.obj \
./eventQueue.obj \
./main_freertos.obj \
./main_mqtt_task.obj \
./maintask.obj \
./mqtt_client_app.obj \
./network_if.obj \
./parseJSON.obj \
./pubSubQueue.obj \
./publish_timer.obj \
./uart_term.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"SPItoMotorEncoder.obj" \
"UartToMotorController.obj" \
"client_cbs.obj" \
"syscfg\ti_drivers_net_wifi_config.obj" \
"syscfg\ti_drivers_config.obj" \
"debug.obj" \
"eventQueue.obj" \
"main_freertos.obj" \
"main_mqtt_task.obj" \
"maintask.obj" \
"mqtt_client_app.obj" \
"network_if.obj" \
"parseJSON.obj" \
"pubSubQueue.obj" \
"publish_timer.obj" \
"uart_term.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"SPItoMotorEncoder.d" \
"UartToMotorController.d" \
"client_cbs.d" \
"syscfg\ti_drivers_net_wifi_config.d" \
"syscfg\ti_drivers_config.d" \
"debug.d" \
"eventQueue.d" \
"main_freertos.d" \
"main_mqtt_task.d" \
"maintask.d" \
"mqtt_client_app.d" \
"network_if.d" \
"parseJSON.d" \
"pubSubQueue.d" \
"publish_timer.d" \
"uart_term.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_net_wifi_config.c" \
"syscfg\ti_drivers_config.c" 

C_SRCS__QUOTED += \
"../SPItoMotorEncoder.c" \
"../UartToMotorController.c" \
"../client_cbs.c" \
"./syscfg/ti_drivers_net_wifi_config.c" \
"./syscfg/ti_drivers_config.c" \
"../debug.c" \
"../eventQueue.c" \
"../main_freertos.c" \
"../main_mqtt_task.c" \
"../maintask.c" \
"../mqtt_client_app.c" \
"../network_if.c" \
"../parseJSON.c" \
"../pubSubQueue.c" \
"../publish_timer.c" \
"../uart_term.c" 

SYSCFG_SRCS__QUOTED += \
"../common.syscfg" 


