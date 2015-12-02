#/* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
# * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
# * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
# * THE AUTHORS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
# * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER. USE IT AT YOUR OWN RISK */

ARCH = arm-none-eabi
MBED_VOLUME = /Volumes/MBED

# Tool definitions
CC      = $(ARCH)-gcc
LD      = $(ARCH)-ld
AR      = $(ARCH)-ar
AS      = $(ARCH)-as
CP      = $(ARCH)-objcopy
OD	= $(ARCH)-objdump
SIZE	= $(ARCH)-size
RM	= rm
Q	= # @./quiet "$@"

# Flags
CFLAGS   = -W -Wall -O0 --std=gnu99 -fgnu89-inline -mcpu=cortex-m3 -mthumb -mfloat-abi=hard
CFLAGS  += -ffunction-sections -fdata-sections
ASFLAGS  =
LDFLAGS  = --gc-sections
CPFLAGS  =
ODFLAGS  = -x --syms
PRFLAGS ?=

# Source files
LINKER_SCRIPT = LPC17xx.ld
CSRCS  = startup_LPC17xx.c $(wildcard *.c)
CSRCS += main.c
ASRCS  = 


OBJS   = $(CSRCS:.c=.o) $(ASRCS:.s=.o)

.PHONY: all size clean nuke

all: main.bin main.hex 

isp: main.bin
#	@./quiet $< cp $^ $(MBED_VOLUME)/

size: main.elf
	@$(SIZE) $<

%.hex: %.elf
	$Q $(CP) $(CPFLAGS) -O ihex $< $*.hex

%.bin: %.elf
	$Q $(CP) $(CPFLAGS) -O binary $< $*.bin

main.elf: $(LINKER_SCRIPT) $(OBJS)
	$Q $(LD) -Map $(@:.elf=.map) $(LDFLAGS) -T $^ -o $@ 
	$Q $(OD) $(ODFLAGS) $@ > $(@:.elf=.dump)
	@$(SIZE) $@

%.o: %.c
	@$(CC) -MM $< -MF $*.d -MP
	$Q $(CC) -c $(CFLAGS) $< -o $@

%.o: %.S
	$Q $(AS) $(ASFLAGS) $< -o $@

clean:
	@-rm -f *.elf
	@-\
for D in "." "**"; do \
  rm -f $$D/*.o $$D/*.d $$D/*.lst $$D/*.dump $$D/*.map; \
done

nuke: clean
	-rm -f *.hex *.bin

-include $(CSRCS:.c=.d)
