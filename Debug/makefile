################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./client_cbs.obj" \
"./syscfg/ti_drivers_net_wifi_config.obj" \
"./syscfg/ti_drivers_config.obj" \
"./debug.obj" \
"./encoder_queue.obj" \
"./eventQueue.obj" \
"./main_freertos.obj" \
"./main_motor_task.obj" \
"./main_mqtt_task.obj" \
"./mqtt_client_app.obj" \
"./network_if.obj" \
"./parseJSON.obj" \
"./pubSubQueue.obj" \
"./publish_timer.obj" \
"./spi_encoder_comm.obj" \
"./stats.obj" \
"./uart_term.obj" \
"../CC3220SF_LAUNCHXL_FREERTOS.cmd" \
$(GEN_CMDS__FLAG) \
-l"ccs/m4/mqtt_release.a" \
-l"ccs/m4/slnetsock_release.a" \
-l"ccs/Release/slnetifwifi.a" \
-l"ti/display/lib/display.aem4" \
-l"ti/grlib/lib/ccs/m4/grlib.a" \
-l"third_party/spiffs/lib/ccs/m4/spiffs.a" \
-l"ti/drivers/lib/drivers_cc32xx.aem4" \
-l"third_party/fatfs/lib/ccs/m4/fatfs.a" \
-l"ccs/rtos/simplelink.a" \
-lfreertos_builds_CC3220SF_LAUNCHXL_release_ccs.lib \
-l"ti/devices/cc32xx/driverlib/ccs/Release/driverlib.a" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Team13_JengaRover.out \

EXE_OUTPUTS__QUOTED += \
"Team13_JengaRover.out" \

BIN_OUTPUTS += \
Team13_JengaRover.bin \

BIN_OUTPUTS__QUOTED += \
"Team13_JengaRover.bin" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Team13_JengaRover.out"

dependents:
	-cd C:/Users/Rohan\ J.\ Dani/CCS_workspace/freertos_builds_CC3220SF_LAUNCHXL_release_ccs/Debug && $(MAKE) all

# Tool invocations
Team13_JengaRover.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) C:/Users/Rohan\ J.\ Dani/CCS_workspace/freertos_builds_CC3220SF_LAUNCHXL_release_ccs/Debug/freertos_builds_CC3220SF_LAUNCHXL_release_ccs.lib
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --define=SF_DEBUG -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on -z -m"Team13_JengaRover.map" -i"C:/ti/simplelink_cc32xx_sdk_3_30_01_02/source/ti/net/mqtt/lib" -i"C:/ti/simplelink_cc32xx_sdk_3_30_01_02/source/ti/net/lib" -i"C:/ti/simplelink_cc32xx_sdk_3_30_01_02/source/ti/drivers/net/wifi/slnetif" -i"C:/ti/simplelink_cc32xx_sdk_3_30_01_02/source" -i"C:/ti/simplelink_cc32xx_sdk_3_30_01_02/source/ti/drivers/net/wifi" -i"C:/Users/Rohan J. Dani/CCS_workspace/freertos_builds_CC3220SF_LAUNCHXL_release_ccs/Debug" -i"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/lib" --diag_suppress=10063 --diag_wrap=off --display_error_number --warn_sections --xml_link_info="Team13_JengaRover_linkInfo.xml" --rom_model -o "Team13_JengaRover.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

Team13_JengaRover.bin: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armhex"  -o "Team13_JengaRover.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "client_cbs.obj" "syscfg\ti_drivers_net_wifi_config.obj" "syscfg\ti_drivers_config.obj" "debug.obj" "encoder_queue.obj" "eventQueue.obj" "main_freertos.obj" "main_motor_task.obj" "main_mqtt_task.obj" "mqtt_client_app.obj" "network_if.obj" "parseJSON.obj" "pubSubQueue.obj" "publish_timer.obj" "spi_encoder_comm.obj" "stats.obj" "uart_term.obj" 
	-$(RM) "client_cbs.d" "syscfg\ti_drivers_net_wifi_config.d" "syscfg\ti_drivers_config.d" "debug.d" "encoder_queue.d" "eventQueue.d" "main_freertos.d" "main_motor_task.d" "main_mqtt_task.d" "mqtt_client_app.d" "network_if.d" "parseJSON.d" "pubSubQueue.d" "publish_timer.d" "spi_encoder_comm.d" "stats.d" "uart_term.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-C:/ti/ccs930/ccs/utils/tiobj2bin/tiobj2bin Team13_JengaRover.out Team13_JengaRover.bin C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armofd C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armhex C:/ti/ccs930/ccs/utils/tiobj2bin/mkhex4bin
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
C:/Users/Rohan\ J.\ Dani/CCS_workspace/freertos_builds_CC3220SF_LAUNCHXL_release_ccs/Debug/freertos_builds_CC3220SF_LAUNCHXL_release_ccs.lib:

-include ../makefile.targets

