
CC               ?= gcc
AR               ?= ar
RM               ?= rm
CFLAGS           ?= -Wall -Wextra
LDFLAGS          ?= 
CP               ?= cp

C_RELEASE_FLAGS  := -O2
C_DEBUG_FLAGS    := -g
CFLAGS           += -Iinclude -fPIC -MMD -MP

BUILD_DIR        ?= build
OBJS_DIR         ?= $(BUILD_DIR)
SRC_DIR          ?= src
INCLUDE_DIR      ?= include
STATIC_LIB_NAME  := libargx.a
STATIC_LIB_DIR   := $(BUILD_DIR)
SHARED_LIB_NAME  := libargx.so
SHARED_LIB_DIR   := $(BUILD_DIR)
STATIC_LIB_PATH  := $(STATIC_LIB_DIR)/$(STATIC_LIB_NAME)
SHARED_LIB_PATH  := $(SHARED_LIB_DIR)/$(SHARED_LIB_NAME)

OBJS := \
$(OBJS_DIR)/argx.o

SRC := \
$(SRC_DIR)/argx.c

DEPENDS := $(patsubst %.o,%.d,$(OBJS))

HEADERS = \
$(INCLUDE_DIR)/argx/argx.h

MAKEFLAGS += --no-print-directory

.PHONY: all debug release install clean test uninstall

$(shell mkdir -p $(BUILD_DIR))

all: debug

debug: CFLAGS += $(C_DEBUG_FLAGS)
debug: $(OBJS) $(HEADERS) Makefile
	@#create the static lib
	$(AR) rcs $(STATIC_LIB_PATH) $(OBJS)
	@# create the shared lib
	$(CC) -shared $(OBJS) -o $(SHARED_LIB_PATH)

release: CFLAGS += $(C_RELEASE_FLAGS)
release: $(OBJS) $(HEADERS) Makefile 
	@#create the static lib
	$(AR) rcs $(STATIC_LIB_PATH) $(OBJS)
	@#create the shared lib
	$(CC) -shared $(OBJS) -o $(SHARED_LIB_PATH)

clean:
	$(RM) $(OBJS) $(BIN_NAME) 
	$(RM) $(SHARED_LIB_PATH) $(STATIC_LIB_PATH)
	$(RM) $(DEPENDS)

install:
	$(MAKE) release
	$(CP) $(STATIC_LIB_PATH) /usr/lib
	$(CP) -r $(INCLUDE_DIR)/argx /usr/include/ 

uninstall:
	$(RM) -f /usr/lib/$(STATIC_LIB_NAME)
	$(RM) -rf /usr/include/argx

-include $(DEPENDS)

$(OBJS_DIR)/%.o : $(SRC_DIR)/%.c Makefile
	$(CC) -c $< $(CFLAGS) $(LDFLAGS) -o $@
