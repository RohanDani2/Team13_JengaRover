################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Rohan J. Dani/CCS_workspace/Team13_JengaRover" --include_path="C:/Users/Rohan J. Dani/CCS_workspace/Team13_JengaRover/Debug" --include_path="C:/ti/simplelink_cc32xx_sdk_3_30_01_02/source" --include_path="C:/ti/simplelink_cc32xx_sdk_3_30_01_02/source/ti/posix/ccs" --include_path="C:/FreeRTOSv10.2.1_191129/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.2.1_191129/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/Users/Rohan J. Dani/CCS_workspace/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/include" --define=SF_DEBUG -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/Rohan J. Dani/CCS_workspace/Team13_JengaRover/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1663602177:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1663602177-inproc

build-1663602177-inproc: ../common.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs930/ccs/utils/sysconfig/sysconfig_cli.bat" -s "C:/ti/simplelink_cc32xx_sdk_3_30_01_02/.metadata/product.json" -o "syscfg" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_drivers_net_wifi_config.c: build-1663602177 ../common.syscfg
syscfg/ti_drivers_config.c: build-1663602177
syscfg/ti_drivers_config.h: build-1663602177
syscfg/syscfg_c.rov.xs: build-1663602177
syscfg/: build-1663602177

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Rohan J. Dani/CCS_workspace/Team13_JengaRover" --include_path="C:/Users/Rohan J. Dani/CCS_workspace/Team13_JengaRover/Debug" --include_path="C:/ti/simplelink_cc32xx_sdk_3_30_01_02/source" --include_path="C:/ti/simplelink_cc32xx_sdk_3_30_01_02/source/ti/posix/ccs" --include_path="C:/FreeRTOSv10.2.1_191129/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.2.1_191129/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/Users/Rohan J. Dani/CCS_workspace/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/include" --define=SF_DEBUG -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/Rohan J. Dani/CCS_workspace/Team13_JengaRover/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


