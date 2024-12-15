ARCH ?= X86_64

CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
LD=$(CROSS_COMPILE)ld
RM=$(CROSS_COMPILE)rm
DEBUG_LEVEL ?= 2
SHARED ?= 0

ROOT_DIRECTORY=$(PWD)/..
BIN=requester inverter

SRCS=check.c common.c
OBJS=$(SRCS:.c=.o)
REQUESTER_SRC=requester.c requester_func.c
REQUESTER_OBJ=$(REQUESTER_SRC:.c=.o)
INVERTER_SRC=inverter.c inverter_func.c
INVERTER_OBJ=$(INVERTER_SRC:.c=.o)

CFLAGS=-Wall -I$(ROOT_DIRECTORY)/include -I. -DDEBUG_LEVEL=$(DEBUG_LEVEL)
LDFLAGS=-L$(ROOT_DIRECTORY)/lib 

all: requester inverter

requester: $(REQUESTER_OBJ) $(OBJS)
	$(CC) -o $@ $(REQUESTER_OBJ) $(OBJS) $(LDFLAGS)
	@echo "LINK => $@"

inverter: $(INVERTER_OBJ) $(OBJS)
	$(CC) -o $@ $(INVERTER_OBJ) $(OBJS) $(LDFLAGS)
	@echo "LINK => $@"

%.o: %.c
	$(CC) -c $< $(CFLAGS)
	@echo "CC <= $<"

clean:
	$(RM) $(BIN) $(OBJS) $(REQUESTER_OBJ) $(INVERTER_OBJ) $(LIB)
