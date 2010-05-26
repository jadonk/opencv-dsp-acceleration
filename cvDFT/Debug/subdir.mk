################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LIB_SRCS += \
../dsplib64plus.lib 

C_SRCS += \
../fft_example.c \
../gen_twiddle_fft16x16.c \
../gen_twiddle_fft16x32.c \
../gen_twiddle_fft32x32.c 

CMD_SRCS += \
../lnk.cmd 

OBJS += \
./fft_example.obj \
./gen_twiddle_fft16x16.obj \
./gen_twiddle_fft16x32.obj \
./gen_twiddle_fft32x32.obj 

C_DEPS += \
./fft_example.pp \
./gen_twiddle_fft16x16.pp \
./gen_twiddle_fft16x32.pp \
./gen_twiddle_fft32x32.pp 


# Each subdirectory must supply rules for building sources it contributes
fft_example.obj: ../fft_example.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/bin/cl6x" -mv64+ -g --fp_mode=strict --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/include" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft16x16" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft16x32" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft32x32" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210" --include_path="C:/Documents and Settings/ppoudel/My Documents/workspace/cvDFT/Debug" --include_path="C:/Program Files/Texas Instruments/bios_5_40_02_22/packages/ti/bios/include" --include_path="C:/Program Files/Texas Instruments/bios_5_40_02_22/packages/ti/rtdx/include/c6000" --diag_warning=225 --sat_reassoc=off --fp_reassoc=on --mem_model:const=data --mem_model:data=far_aggregates $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="fft_example.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

gen_twiddle_fft16x16.obj: ../gen_twiddle_fft16x16.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/bin/cl6x" -mv64+ -g --fp_mode=strict --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/include" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft16x16" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft16x32" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft32x32" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210" --include_path="C:/Documents and Settings/ppoudel/My Documents/workspace/cvDFT/Debug" --include_path="C:/Program Files/Texas Instruments/bios_5_40_02_22/packages/ti/bios/include" --include_path="C:/Program Files/Texas Instruments/bios_5_40_02_22/packages/ti/rtdx/include/c6000" --diag_warning=225 --sat_reassoc=off --fp_reassoc=on --mem_model:const=data --mem_model:data=far_aggregates $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="gen_twiddle_fft16x16.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

gen_twiddle_fft16x32.obj: ../gen_twiddle_fft16x32.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/bin/cl6x" -mv64+ -g --fp_mode=strict --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/include" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft16x16" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft16x32" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft32x32" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210" --include_path="C:/Documents and Settings/ppoudel/My Documents/workspace/cvDFT/Debug" --include_path="C:/Program Files/Texas Instruments/bios_5_40_02_22/packages/ti/bios/include" --include_path="C:/Program Files/Texas Instruments/bios_5_40_02_22/packages/ti/rtdx/include/c6000" --diag_warning=225 --sat_reassoc=off --fp_reassoc=on --mem_model:const=data --mem_model:data=far_aggregates $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="gen_twiddle_fft16x32.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

gen_twiddle_fft32x32.obj: ../gen_twiddle_fft32x32.c $(GEN_SRCS) $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/bin/cl6x" -mv64+ -g --fp_mode=strict --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/include" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft16x16" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft16x32" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210/src/DSP_fft32x32" --include_path="C:/Program Files/Texas Instruments/DSPLib/dsplib_v210" --include_path="C:/Documents and Settings/ppoudel/My Documents/workspace/cvDFT/Debug" --include_path="C:/Program Files/Texas Instruments/bios_5_40_02_22/packages/ti/bios/include" --include_path="C:/Program Files/Texas Instruments/bios_5_40_02_22/packages/ti/rtdx/include/c6000" --diag_warning=225 --sat_reassoc=off --fp_reassoc=on --mem_model:const=data --mem_model:data=far_aggregates $(GEN_OPTS_QUOTED) --preproc_with_compile --preproc_dependency="gen_twiddle_fft32x32.pp" $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


