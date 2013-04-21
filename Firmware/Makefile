# Hey Emacs, this is a -*- makefile -*-
#
# WinARM template makefile
# by Martin Thomas, Kaiserslautern, Germany
# <eversmith(at)heizung-thomas(dot)de>
#
# Released to the Public Domain
# Please read the make user manual!
#
# The user-configuration is based on the WinAVR makefile-template
# written by Eric B. Weddington, Jörg Wunsch, et al. but internal
# handling used here is very different.
# This makefile can also be used with the GNU tools included in
# Yagarto, GNUARM or the codesourcery packages. It should work
# on Unix/Linux-Systems too. Just a rather up-to-date GNU make is
# needed.
#
#
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make program = Upload load-image to the device
#
# make filename.s = Just compile filename.c into the assembler code only
#
# make filename.o = Create object filename.o from filename.c (using CFLAGS)
#
# To rebuild project do "make clean" then "make all".
#
# Changelog:
# - 17. Feb. 2005  - added thumb-interwork support (mth)
# - 28. Apr. 2005  - added C++ support (mth)
# - 29. Arp. 2005  - changed handling for lst-Filename (mth)
# -  1. Nov. 2005  - exception-vector placement options (mth)
# - 15. Nov. 2005  - added library-search-path (EXTRA_LIB...) (mth)
# -  2. Dec. 2005  - fixed ihex and binary file extensions (mth)
# - 22. Feb. 2006  - added AT91LIBNOWARN setting (mth)
# - 19. Apr. 2006  - option FLASH_TOOL (mth)
# - 23. Jun. 2006  - option USE_THUMB_MODE -> THUMB/THUMB_IW
# -  3. Aug. 2006  - added -ffunction-sections -fdata-sections to CFLAGS
#                    and --gc-sections to LDFLAGS. Only available for gcc 4 (mth)
#                    (needs appropriate linker-script, remove them when using a
#                    "simple" linker-script)
# -  4. Aug. 2006  - pass SUBMDL-define to frontend (mth)
# - 11. Nov. 2006  - FLASH_TOOL-config, TCHAIN-config (mth)
# - 28. Mar. 2007  - remove .dep-Directory with rm -r -f and force "no error" (mth)
# - 24. Apr. 2007  - added "both" option for format (.bin and .hex) (mth)
# - 20. Aug. 2007  - extraincdirs in asflags, passing a "board"-define (mth)
# - 13. Sep. 2007  - create assembler from c-sources fixed (make foo.s for foo.c) (mth)
#                  - IMGEXT no longer used and removed (mth)
#                  - moved some entries (mth)
# - 25. Oct. 2007  - reverted 20070328-change (b/o "race condition" with
#                    make clean all or when called from Eclipse) (mth)
#                  - removed "for flash" from objdump message-string (mth)
#                  - added same remarks (mth)
# - 30. Oct. 2007  - Support for an output-directory with all files
#                    created during "make all". (mth)
#                  - modified targets which creates assembler (lower-case s)
#                    from C-source using make <.c-file w/o ext.>.s (mth)
#                  - removed redundant/unused defines, overall cleanup (mth)
# - 10. Nov. 2007  - renamed TCHAIN to TCHAIN_PREFIX, other minor cleanup (mth)
# - 13. Mar. 2008  - renamed FORMAT to LOADFORMAT, edited some comments/messages (mth)
# - 13. Apr. 2009  - OpenOCD options for batch-programming (make program) (mth)
# -  1. May  2009  - replaced SUBMDL with CHIP (mth)
# - 15. Jul. 2009  - ComSpec environment-variable to select host-OS, should
#                    increase compatiblity, only tested with WinXP, cs-(GNU)-make 3.81 (mth)
# -  1. Sep. 2009  - rename ROM_RUN->FLASH_RUN, VECT_TAB_ROM->VECT_TAB_FLASH (mth)
# -  7. Sep. 2009  - modified directory creation ("md", related to change 20090615) (mth)

##
## Special Version to be used with CS G++ lite (cs-make/cs-rm, no other
## Shell-Tools needed). Tested with Win XP and Sourcery G++ Lite 2009q1-161.
## M. Thomas 9/2009
##

# Toolchain prefix (i.e arm-elf- -> arm-elf-gcc.exe)
#TCHAIN_PREFIX = arm-eabi-
#TCHAIN_PREFIX = arm-elf-
TCHAIN_PREFIX = /opt/arm/bin/arm-none-eabi-
REMOVE_CMD=rm
#REMOVE_CMD=cs-rm

FLASH_TOOL = OPENOCD
#FLASH_TOOL = STM32LOADER
#FLASH_TOOL = LPC21ISP
#FLASH_TOOL = UVISION

# MCU name, submodel and board
# - MCU used for compiler-option (-mcpu)
# - SUBMDL used for linker-script name (-T) and passed as define
# - BOARD just passed as define (optional)
MCU      = cortex-m3
CHIP     = STM32F103RB
F_XTAL   = 12000000
HSE_VALUE = $(F_XTAL)
SYSCLOCK_CL = SYSCLK_FREQ_72MHz=72000000
#SYSCLOCK_CL = SYSCLK_FREQ_24MHz=24000000

# Exception-vectors placement option is just passed as define,
# the user has to implement the necessary operations (i.e. remapping)
# Exception vectors in FLASH:
##VECTOR_TABLE_LOCATION=VECT_TAB_FLASH
# Exception vectors in RAM:
VECTOR_TABLE_LOCATION=VECT_TAB_RAM

# Directory for output files (lst, obj, dep, elf, sym, map, hex, bin etc.)
OUTDIR = bin

# Target file name (without extension).
TARGET = main

# Pathes to libraries
#APPLIBDIR = ../STM32F10x_StdPeriph_Lib_V3.4.0
APPLIBDIR = /home/zapol/dokumenty/STM32/STM32F10x_StdPeriph_Lib_V3.5.0

STMLIBDIR = $(APPLIBDIR)/Libraries

STMSPDDIR = $(STMLIBDIR)/STM32F10x_StdPeriph_Driver
STMSPDSRCDIR = $(STMSPDDIR)/src
STMSPDINCDIR = $(STMSPDDIR)/inc

CMSISCOREDIR  = $(STMLIBDIR)/CMSIS/CM3/CoreSupport
CMSISDEVDIR  = $(STMLIBDIR)/CMSIS/CM3/DeviceSupport/ST/STM32F10x

# List C source files here. (C dependencies are automatically generated.)
# use file-extension c for "c-only"-files
## Demo-Application:
SRC = main.c
SRC += stm32f10x_it.c system_stm32f10x.c pprintf.c utask.c debug.c usart.c utils.c motors.c
## compiler-specific sources
#SRC += startup_stm32f10x_md_mthomas.c
## CMSIS for STM32
SRC += $(CMSISCOREDIR)/core_cm3.c
## used parts of the STM-Library
SRC += $(STMSPDSRCDIR)/stm32f10x_usart.c
#SRC += $(STMSPDSRCDIR)/stm32f10x_flash.c
SRC += $(STMSPDSRCDIR)/stm32f10x_gpio.c
SRC += $(STMSPDSRCDIR)/stm32f10x_exti.c
SRC += $(STMSPDSRCDIR)/stm32f10x_rcc.c
SRC += $(STMSPDSRCDIR)/stm32f10x_spi.c
#SRC += $(STMSPDSRCDIR)/stm32f10x_fsmc.c
#SRC += $(STMSPDSRCDIR)/stm32f10x_rtc.c
#SRC += $(STMSPDSRCDIR)/stm32f10x_bkp.c
#SRC += $(STMSPDSRCDIR)/stm32f10x_pwr.c
SRC += $(STMSPDSRCDIR)/stm32f10x_dma.c
SRC += $(STMSPDSRCDIR)/stm32f10x_tim.c
SRC += $(STMSPDSRCDIR)/misc.c
## EEprom Emulation AppNote Code
#SRC += $(STMEEEMULSRCDIR)/eeprom.c

#SRC += $(EVALDIR)/stm32_eval.c
#SRC += $(BOARDDIR)/stm3210b_eval_lcd.c

# List C source files here which must be compiled in ARM-Mode (no -mthumb).
# use file-extension c for "c-only"-files
## just for testing, timer.c could be compiled in thumb-mode too
SRCARM =

# List C++ source files here.
# use file-extension .cpp for C++-files (not .C)
CPPSRC =

# List C++ source files here which must be compiled in ARM-Mode.
# use file-extension .cpp for C++-files (not .C)
#CPPSRCARM = $(TARGET).cpp
CPPSRCARM =

# List Assembler source files here.
# Make them always end in a capital .S. Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC = $(CMSISDEVDIR)/startup/gcc_ride7/startup_stm32f10x_md.s
#ASRC = startup_stm32f10x_md_vl.s

# List Assembler source files here which must be assembled in ARM-Mode..
ASRCARM =

# List any extra directories to look for include files here.
#    Each directory must be seperated by a space.
EXTRAINCDIRS  = $(STMSPDINCDIR) $(CMSISCOREDIR) $(CMSISDEVDIR)
#EXTRAINCDIRS += $(FATSDDIR) $(MININIDIR) $(STMEEEMULINCDIR) $(EVALDIR) $(BOARDDIR)

INCLUDES = $(patsubst %,-I%,$(EXTRAINCDIRS)) -I. -I..

# List any extra directories to look for library files here.
# Also add directories where the linker should search for
# includes from linker-script to the list
#     Each directory must be seperated by a space.
EXTRA_LIBDIRS =

# Extra libraries
#    Each library-name must be seperated by a space.
#    i.e. to link with libxyz.a, libabc.a and libefsl.a:
#    EXTRA_LIBS = xyz abc efsl
# for newlib-lpc (file: libnewlibc-lpc.a):
#    EXTRA_LIBS = newlib-lpc
EXTRA_LIBS =

# Optimization level, can be [0, 1, 2, 3, s].
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s
#OPT = 2
#OPT = 3
#OPT = 0

# Output format. (can be ihex or binary or both)
#  binary to create a load-image in raw-binary format i.e. for SAM-BA,
#  ihex to create a load-image in Intel hex format i.e. for lpc21isp
#LOADFORMAT = ihex
#LOADFORMAT = binary
LOADFORMAT = both

# Using the Atmel AT91_lib produces warnings with
# the default warning-levels.
#  yes - disable these warnings
#  no  - keep default settings
#AT91LIBNOWARN = yes
AT91LIBNOWARN = no

# Debugging format.
#DEBUG = stabs
DEBUG = dwarf-2

# Place project-specific -D (define) and/or
# -U options for C here.
CDEFS += -D_STM32F103RBT6_
CDEFS += -D_STM3x
CDEFS += -D_STM32x

CDEFS += -D STM32F10X_MD
CDEFS += -D USE_STDPERIPH_DRIVER
#CDEFS += -D USE_STM3210B_EVAL
CDEFS += -D HSE_VALUE=$(HSE_VALUE)

# enable parameter-checking in STM's library
CDEFS += -DUSE_FULL_ASSERT

# Place project-specific -D and/or -U options for
# Assembler with preprocessor here.
#ADEFS = -DUSE_IRQ_ASM_WRAPPER
ADEFS = -D_STM32F103RBT6_
ADEFS += -D STM32F10X_MD
ADEFS += -D_STM32x

#flags for C and LD

CFLAGS =  -g -Wall
CFLAGS += -MD
CFLAGS += -O$(OPT)
CFLAGS += -mcpu=$(MCU) -mthumb
CFLAGS += $(CDEFS)
CFLAGS += $(INCLUDES)
CFLAGS += -ffunction-sections -mlittle-endian

# Assembler flags.
ASFLAGS  = -MD
ASFLAGS += $(ADEFS)
ASFLAGS += -mcpu=$(MCU) -mthumb
ASFLAGS += -Wa,-EL

# Linker flags.
LDFLAGS = -nostartfiles -Wl,-Map -Xlinker $(OUTDIR)/$(TARGET).map
LDFLAGS += -mcpu=$(MCU) -mthumb
#LDFLAGS += -Wl,-T -Xlinker $(CMSISDIR)/STM32_128K_20K_FLASH.ld
LDFLAGS += -Wl,-T -Xlinker stm32_flash.ld
LDFLAGS += -u start
LDFLAGS += -Wl,-static
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -L $(CMSISDIR)
LDFLAGS += -lc -lm

# ---------------------------------------------------------------------------
# Options for lpc21isp by Martin Maurer
# lpc21isp only supports NXP LPC and Analog ADuC ARMs though the
# integrated uart-bootloader (ISP)
#
# Settings and variables:
LPC21ISP = lpc21isp
LPC21ISP_FLASHFILE = $(OUTDIR)/$(TARGET).hex
LPC21ISP_PORT = com1
LPC21ISP_BAUD = 57600
LPC21ISP_XTAL = 14746
# other options:
# -debug: verbose output
# -control: enter bootloader via RS232 DTR/RTS (only if hardware
#           supports this feature - see NXP AppNote)
#LPC21ISP_OPTIONS = -control
#LPC21ISP_OPTIONS += -debug
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# Options for OpenOCD flash-programming
# see openocd.pdf/openocd.texi for further information
#
OOCD_LOADFILE+=$(OUTDIR)/$(TARGET).elf
# if OpenOCD is in the $PATH just set OPENOCDEXE=openocd
OOCD_EXE=openocd
# debug level
OOCD_CL=-d1
#OOCD_CL=-d3
# interface and board/target settings (using the OOCD target-library here)
OOCD_CL+=-f /usr/share/openocd/scripts/interface/oocdlink.cfg -f /usr/share/openocd/scripts/target/stm32f1x.cfg
# initialize
OOCD_CL+=-c init
# enable "fast mode" - can be disabled for tests
#OOCD_CL+=-c "fast enable"
# show the targets
#OOCD_CL+=-c "targets stm32.cpu"
# commands to prepare flash-write
#OOCD_CL+= -c "soft_reset_halt"
OOCD_CL+= -c "reset halt"
# increase JTAG frequency a little bit - can be disabled for tests
OOCD_CL+= -c "adapter_khz 1200"
# flash-write and -verify
OOCD_CL+=-c "targets"
OOCD_CL+=-c "flash write_image erase $(OOCD_LOADFILE)" -c "verify_image $(OOCD_LOADFILE)"
# reset target
OOCD_CL+=-c "reset run"
OOCD_CL+=-c "targets"
# terminate OOCD after programming
OOCD_CL+=-c shutdown
# ---------------------------------------------------------------------------


# Define programs and commands.
CC      = $(TCHAIN_PREFIX)gcc
CPP     = $(TCHAIN_PREFIX)g++
AR      = $(TCHAIN_PREFIX)ar
OBJCOPY = $(TCHAIN_PREFIX)objcopy
OBJDUMP = $(TCHAIN_PREFIX)objdump
SIZE    = $(TCHAIN_PREFIX)size
NM      = $(TCHAIN_PREFIX)nm
REMOVE  = $(REMOVE_CMD) -f
###SHELL   = sh
###COPY    = cp


# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = "-------- begin (mode: $(RUN_MODE)) --------"
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER = Size after build:
MSG_LOAD_FILE = Creating load file:
MSG_EXTENDED_LISTING = Creating Extended Listing/Disassembly:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = "**** Linking :"
MSG_COMPILING = "**** Compiling C :"
MSG_COMPILING_ARM = "**** Compiling C (ARM-only):"
MSG_COMPILINGCPP = "Compiling C++ :"
MSG_COMPILINGCPP_ARM = "Compiling C++ (ARM-only):"
MSG_ASSEMBLING = "**** Assembling:"
MSG_ASSEMBLING_ARM = "****Assembling (ARM-only):"
MSG_CLEANING = Cleaning project:
MSG_FORMATERROR = Can not handle output-format
MSG_LPC21_RESETREMINDER = You may have to bring the target in bootloader-mode now.
MSG_ASMFROMC = "Creating asm-File from C-Source:"
MSG_ASMFROMC_ARM = "Creating asm-File from C-Source (ARM-only):"

# List of all source files.
ALLSRC     = $(ASRCARM) $(ASRC) $(SRCARM) $(SRC) $(CPPSRCARM) $(CPPSRC)
# List of all source files without directory and file-extension.
ALLSRCBASE = $(notdir $(basename $(ALLSRC)))

# Define all object files.
ALLOBJ     = $(addprefix $(OUTDIR)/, $(addsuffix .o, $(ALLSRCBASE)))

# Define all listing files (used for make clean).
LSTFILES   = $(addprefix $(OUTDIR)/, $(addsuffix .lst, $(ALLSRCBASE)))
# Define all depedency-files (used for make clean).
DEPFILES   = $(addprefix $(OUTDIR)/dep/, $(addsuffix .o.d, $(ALLSRCBASE)))

elf: $(OUTDIR)/$(TARGET).elf
lss: $(OUTDIR)/$(TARGET).lss
sym: $(OUTDIR)/$(TARGET).sym
hex: $(OUTDIR)/$(TARGET).hex
bin: $(OUTDIR)/$(TARGET).bin

# Default target.
#all: begin gccversion sizebefore build sizeafter finished end
all: begin gccversion build sizeafter finished end

ifeq ($(LOADFORMAT),ihex)
build: elf hex lss sym bin
else
ifeq ($(LOADFORMAT),binary)
build: elf bin lss sym
else
ifeq ($(LOADFORMAT),both)
build: elf hex bin lss sym
else
$(error "$(MSG_FORMATERROR) $(FORMAT)")
endif
endif
endif


# Eye candy.
begin:
##	@echo
	@echo $(MSG_BEGIN)

finished:
##	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
##	@echo

# Display sizes of sections.
ELFSIZE = $(SIZE) -A  $(OUTDIR)/$(TARGET).elf
##ELFSIZE = $(SIZE) --format=Berkeley --common $(OUTDIR)/$(TARGET).elf
sizebefore:
#	@if [ -f  $(OUTDIR)/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi
	@echo $(MSG_SIZE_BEFORE)
	$(ELFSIZE)

sizeafter:
#	@if [ -f  $(OUTDIR)/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi
	@echo $(MSG_SIZE_AFTER)
	$(ELFSIZE)

# Display compiler version information.
gccversion :
	@$(CC) --version
#	@echo $(ALLOBJ)

# Program the device.
ifeq ($(FLASH_TOOL),STM32LOADER)
# Program the device with STM32Loader Python script.
program: $(OUTDIR)/$(TARGET).bin
##	@echo
	@echo "Programming with STM32Loader"
	python2 loader.py -e -w -v -p /dev/ttyUSB0 $(OUTDIR)/$(TARGET).bin
else
ifeq ($(FLASH_TOOL),UVISION)
# Program the device with Keil's uVision (needs configured uVision-workspace).
program: $(OUTDIR)/$(TARGET).hex
##	@echo
	@echo "Programming with uVision"
	C:\Keil\uv3\Uv3.exe -f uvisionflash.Uv2 -ouvisionflash.txt
else
ifeq ($(FLASH_TOOL),OPENOCD)
# Program the device with Dominic Rath's OPENOCD in "batch-mode", needs cfg and "reset-script".
program: $(OUTDIR)/$(TARGET).elf
	@echo "Programming with OPENOCD"
	$(OOCD_EXE) $(OOCD_CL)
reset:
	openocd -d0 -f /usr/share/openocd/scripts/interface/oocdlink.cfg -f /usr/share/openocd/scripts/targets/stm32f1x.cfg -c init -c "jtag_khz 1200" -c "reset run" -c targets -c shutdown
else
# Program the device using lpc21isp (for NXP2k and ADuC UART bootloader)
program: $(OUTDIR)/$(TARGET).hex
##	@echo
	@echo $(MSG_LPC21_RESETREMINDER)
	-$(LPC21ISP) $(LPC21ISP_OPTIONS) $(LPC21ISP_FLASHFILE) $(LPC21ISP_PORT) $(LPC21ISP_BAUD) $(LPC21ISP_XTAL)
endif
endif
endif

# Create final output file (.hex) from ELF output file.
%.hex: %.elf
##	@echo
	@echo $(MSG_LOAD_FILE) $@
	$(OBJCOPY) -O ihex $< $@

# Create final output file (.bin) from ELF output file.
%.bin: %.elf
##	@echo
	@echo $(MSG_LOAD_FILE) $@
	$(OBJCOPY) -O binary $< $@

# Create extended listing file/disassambly from ELF output file.
# using objdump testing: option -C
%.lss: %.elf
##	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -C -r $< > $@
#	$(OBJDUMP) -x -S $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
##	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(ALLOBJ)
%.elf:  $(ALLOBJ)
	@echo
	@echo $(MSG_LINKING) $@
# use $(CC) for C-only projects or $(CPP) for C++-projects:
	$(CC) $(THUMB) $(ALLOBJ) --output $@ $(LDFLAGS)
#	$(CPP) $(THUMB) $(CFLAGS) $(ALLOBJ) --output $@ $(LDFLAGS)


# Assemble: create object files from assembler source files.
define ASSEMBLE_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1)
##	@echo
	@echo $(MSG_ASSEMBLING) $$< "->" $$@
	$(CC) -c $(THUMB) $$(ASFLAGS) $$< -o $$@
endef
$(foreach src, $(ASRC), $(eval $(call ASSEMBLE_TEMPLATE, $(src))))

# Assemble: create object files from assembler source files. ARM-only
define ASSEMBLE_ARM_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1)
##	@echo
	@echo $(MSG_ASSEMBLING_ARM) $$< "->" $$@
	$(CC) -c $$(ASFLAGS) $$< -o $$@
endef
$(foreach src, $(ASRCARM), $(eval $(call ASSEMBLE_ARM_TEMPLATE, $(src))))


# Compile: create object files from C source files.
define COMPILE_C_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1)
##	@echo
	@echo $(MSG_COMPILING) $$< "->" $$@
	$(CC) -c $(THUMB) $$(CFLAGS) $$(CONLYFLAGS) $$< -o $$@
endef
$(foreach src, $(SRC), $(eval $(call COMPILE_C_TEMPLATE, $(src))))

# Compile: create object files from C source files. ARM-only
define COMPILE_C_ARM_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1)
##	@echo
	@echo $(MSG_COMPILING_ARM) $$< "->" $$@
	$(CC) -c $$(CFLAGS) $$(CONLYFLAGS) $$< -o $$@
endef
$(foreach src, $(SRCARM), $(eval $(call COMPILE_C_ARM_TEMPLATE, $(src))))


# Compile: create object files from C++ source files.
define COMPILE_CPP_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1)
##	@echo
	@echo $(MSG_COMPILINGCPP) $$< "->" $$@
	$(CC) -c $(THUMB) $$(CFLAGS) $$(CPPFLAGS) $$< -o $$@
endef
$(foreach src, $(CPPSRC), $(eval $(call COMPILE_CPP_TEMPLATE, $(src))))

# Compile: create object files from C++ source files. ARM-only
define COMPILE_CPP_ARM_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1)
##	@echo
	@echo $(MSG_COMPILINGCPP_ARM) $$< "->" $$@
	$(CC) -c $$(CFLAGS) $$(CPPFLAGS) $$< -o $$@
endef
$(foreach src, $(CPPSRCARM), $(eval $(call COMPILE_CPP_ARM_TEMPLATE, $(src))))


# Compile: create assembler files from C source files. ARM/Thumb
$(SRC:.c=.s) : %.s : %.c
	@echo $(MSG_ASMFROMC) $< to $@
	$(CC) $(THUMB) -S $(CFLAGS) $(CONLYFLAGS) $< -o $@

# Compile: create assembler files from C source files. ARM only
$(SRCARM:.c=.s) : %.s : %.c
	@echo $(MSG_ASMFROMC_ARM) $< to $@
	$(CC) -S $(CFLAGS) $(CONLYFLAGS) $< -o $@

# Target: clean project.
clean: begin clean_list finished end

clean_list :
##	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(OUTDIR)/$(TARGET).map
	$(REMOVE) $(OUTDIR)/$(TARGET).elf
	$(REMOVE) $(OUTDIR)/$(TARGET).hex
	$(REMOVE) $(OUTDIR)/$(TARGET).bin
	$(REMOVE) $(OUTDIR)/$(TARGET).sym
	$(REMOVE) $(OUTDIR)/$(TARGET).lss
	$(REMOVE) $(ALLOBJ)
	$(REMOVE) $(LSTFILES)
	$(REMOVE) $(DEPFILES)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRCARM:.c=.s)
	$(REMOVE) $(CPPSRC:.cpp=.s)
	$(REMOVE) $(CPPSRCARM:.cpp=.s)


# Create output files directory
# all known MS Windows OS define the ComSpec environment variable
ifdef ComSpec
$(shell md $(OUTDIR) 2>NUL)
else
$(shell mkdir $(OUTDIR) 2>/dev/null)
endif

# Include the dependency files.
ifdef ComSpec
-include $(shell md $(OUTDIR)\dep 2>NUL) $(wildcard $(OUTDIR)/dep/*)
else
-include $(shell mkdir $(OUTDIR) 2>/dev/null) $(shell mkdir $(OUTDIR)/dep 2>/dev/null) $(wildcard $(OUTDIR)/dep/*)
endif


# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex bin lss sym clean clean_list program

