################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --cla_support=cla1 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/Users/eend/OneDrive - University of Bristol/2024_25/FreeRTOS_Documents/CCS_Codes/Device_support/F2837xD_common/include" --include_path="/Users/huangxiaoqing/ti/C2000Ware_5_03_00_00/device_support/f2837xd/common/include" --include_path="/Users/huangxiaoqing/ti/C2000Ware_5_03_00_00/device_support/f2837xd/headers/include" --include_path="C:/Users/eend/OneDrive - University of Bristol/2024_25/FreeRTOS_Documents/CCS_Codes/Device_support/F2837xD_headers/include" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="/Applications/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all -g --define=CPU1 --define=_LAUNCHXL_F28379D --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --cla_support=cla1 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/Users/eend/OneDrive - University of Bristol/2024_25/FreeRTOS_Documents/CCS_Codes/Device_support/F2837xD_common/include" --include_path="/Users/huangxiaoqing/ti/C2000Ware_5_03_00_00/device_support/f2837xd/common/include" --include_path="/Users/huangxiaoqing/ti/C2000Ware_5_03_00_00/device_support/f2837xd/headers/include" --include_path="C:/Users/eend/OneDrive - University of Bristol/2024_25/FreeRTOS_Documents/CCS_Codes/Device_support/F2837xD_headers/include" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="/Applications/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all -g --define=CPU1 --define=_LAUNCHXL_F28379D --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


