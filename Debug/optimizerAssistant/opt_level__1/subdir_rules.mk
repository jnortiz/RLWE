################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/jnortiz/ti_0/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O1 --opt_for_speed=1 --include_path="/home/jnortiz/ti_0/ccsv6/ccs_base/arm/include" --include_path="/home/jnortiz/ti_0/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/jnortiz/ti_0/msp430/MSPWare_3_30_00_18/driverlib/driverlib/MSP432P4xx" --include_path="/home/jnortiz/ti_0/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --display_error_number --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

startup_msp432p401r_ccs.obj: ../startup_msp432p401r_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/jnortiz/ti_0/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O1 --opt_for_speed=1 --include_path="/home/jnortiz/ti_0/ccsv6/ccs_base/arm/include" --include_path="/home/jnortiz/ti_0/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/jnortiz/ti_0/msp430/MSPWare_3_30_00_18/driverlib/driverlib/MSP432P4xx" --include_path="/home/jnortiz/ti_0/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --display_error_number --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="startup_msp432p401r_ccs.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

system_msp432p401r.obj: ../system_msp432p401r.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/jnortiz/ti_0/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O1 --opt_for_speed=1 --include_path="/home/jnortiz/ti_0/ccsv6/ccs_base/arm/include" --include_path="/home/jnortiz/ti_0/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/jnortiz/ti_0/msp430/MSPWare_3_30_00_18/driverlib/driverlib/MSP432P4xx" --include_path="/home/jnortiz/ti_0/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --display_error_number --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="system_msp432p401r.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


