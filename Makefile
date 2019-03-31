MCU  = stm8s003f3
ARCH = stm8

F_CPU   ?= 16000000

SRCDIR   = src
INCLUDEDIR = include
BUILDDIR = build
TARGET  ?= $(BUILDDIR)/main.ihx

SRCS    := $(wildcard *.c $(SRCDIR)/*.c)
ASRCS   := $(wildcard *.s $(SRCDIR)/*.s)

OBJS     = $(subst $(SRCDIR)/,$(BUILDDIR)/,$(SRCS:.c=.rel))
OBJS    += $(subst $(SRCDIR)/,$(BUILDDIR)/,$(ASRCS:.s=.rel))

$(info $$OBJS is [${OBJS}])
$(info $$SRCS is [${SRCS}])
$(info $$ASRCS is [${ASRCS}])

CC       = sdcc
LD       = sdld
AS       = sdasstm8
OBJCOPY  = sdobjcopy
ASFLAGS  = -plosgff
CFLAGS   = -m$(ARCH) -p$(MCU) -DF_CPU=$(F_CPU)UL
CFLAGS  += -I$(INCLUDEDIR)
CFLAGS  += --std-sdcc11
CFLAGS  += --stack-auto --noinduction --use-non-free
CFLAGS  += --opt-code-size
## Extra optimization rules - use with care
#CFLAGS  += --peep-file $(LIBDIR)/util/extra.def
LDFLAGS  = -m$(ARCH) -l$(ARCH) --out-fmt-ihx

all: $(TARGET) size

$(TARGET): $(BUILDDIR) $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(BUILDDIR)/%.rel: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.rel: $(SRCDIR)/%.s
	$(AS) $(ASFLAGS) build/$<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

size:
	@$(OBJCOPY) -I ihex --output-target=binary $(TARGET) $(TARGET).bin
	@echo "----------"
	@echo "Image size:"
	@stat -L -c %s $(TARGET).bin

flash: $(TARGET)
	stm8flash -c stlinkv2 -p $(MCU) -w $(TARGET)

serial: $(TARGET)
	stm8gal -p /dev/ttyUSB0 -w $(TARGET)

clean:
	rm -rf build

.PHONY: clean all flash
