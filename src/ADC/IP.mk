local_dir	:= src/ADC
lib_dir		:= $(local_dir)
lib_name	:= #??.a
header_dir	:= #$(local_dir)/include

local_lib	:= #$(lib_dir)/$(lib_name)

# compile source
local_src	:= adc-ctrl.c adc-menu.c
lib_src		:= 

include build/common.mk
