
LIBNAME          := argx
ifeq ($(LIBNAME),)
$(error LIBNAME was not set)
endif

CC               ?= gcc
AR               ?= ar
CFLAGS           := -MMD -MP -Iinclude
LDFLAGS          := 

C_RELEASEFLAGS   := -O2
C_DEBUGFLAGS     := -g

CWARNINGS        := -Wall -Wextra
CFLAGS           += $(CWARNINGS)

EXTRA_CFLAGS     ?= 
EXTRA_LDFLAGS    ?=

CFLAGS           += $(EXTRA_CFLAGS)
LDFLAGS          += $(EXTRA_LDFLAGS)

BUILDDIR         := build
INCLUDEDIR       := include
SRCDIR           := src

SRC              := \
$(SRCDIR)/argx.c \

IS_RELEASE       := $(filter release, $(MAKECMDGOALS))
ifeq ($(IS_RELEASE), release)
	CFLAGS += $(C_RELEASEFLAGS)
else
	CFLAGS += $(C_DEBUGFLAGS)
endif

DEPS             := $(SRC:%.c=%.d)
DEPS             := $(DEPS:$(SRCDIR)/%=%)
DEPS             := $(addprefix $(BUILDDIR)/, $(DEPS))

OBJS             := $(SRC:%.c=%.o)
OBJS             := $(OBJS:$(SRCDIR)/%=%)
OBJS             := $(addprefix $(BUILDDIR)/, $(OBJS))

HEADERS          := \
$(INCLUDEDIR)/argx/argx.h

OUT_LIBSTATIC_PATH := $(BUILDDIR)/lib$(LIBNAME).a
OUT_LIBSHARED_PATH := $(BUILDDIR)/lib$(LIBNAME).so

all: debug

debug: $(OBJS) Makefile
	$(AR) rcs $(OUT_LIBSTATIC_PATH) $(OBJS)
	$(CC) -shared $(OBJS) $(LDFLAGS) -o $(OUT_LIBSHARED_PATH)

release: $(OBJS) Makefile
	$(AR) rcs $(OUT_LIBSTATIC_PATH) $(OBJS)
	$(CC) -shared $(OBJS) $(LDFLAGS) -o $(OUT_LIBSHARED_PATH)

-include $(DEPS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/*.d

mrclean:
	$(MAKE) clean
	rm -f $(OUT_LIBSHARED_PATH) $(OUT_LIBSTATIC_PATH)
	rmdir $(BUILDDIR) || true

install:
	$(MAKE) release
	mkdir -p /usr/include/$(LIBNAME)
	cp $(HEADERS) /usr/include/$(LIBNAME)
	cp $(OUT_LIBSHARED_PATH) /usr/lib

uninstall:
	rm -f $(addprefix /usr/include/$(LIBNAME)/, $(notdir $(HEADERS)))
	rmdir /usr/include/$(LIBNAME) || true
	rm -f /usr/lib/$(notdir $(OUT_LIBSHARED_PATH))

.PHONY: all debug release clean mrclean install uninstall
