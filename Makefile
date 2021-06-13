
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

HEADERS          := \
$(INCLUDEDIR)/argx/argx.h

OBJS_SUFFIX      := 
IS_RELEASE       := 
ifeq ($(filter release, $(MAKECMDGOALS)), release)
	CFLAGS       += $(C_RELEASEFLAGS)
	IS_RELEASE   := 1
	OBJS_SUFFIX  := release
else
	IS_RELEASE   := 0
	CFLAGS       += $(C_DEBUGFLAGS)
	OBJS_SUFFIX  := debug
endif

CDEPS             := $(SRC:%.c=%_$(OBJS_SUFFIX).c.d)
CDEPS             := $(CDEPS:$(SRCDIR)/%=%)
CDEPS             := $(addprefix $(BUILDDIR)/, $(CDEPS))

COBJS             := $(SRC:%.c=%_$(OBJS_SUFFIX).c.o)
COBJS             := $(COBJS:$(SRCDIR)/%=%)
COBJS             := $(addprefix $(BUILDDIR)/, $(COBJS))

OUT_LIBSTATIC_PATH := $(BUILDDIR)/lib$(LIBNAME).a
OUT_LIBSHARED_PATH := $(BUILDDIR)/lib$(LIBNAME).so

all: debug

debug: $(COBJS) Makefile
	@# ar doesn't replace archive members sometimes.. ? 
	@# I think it skips them when the new ones are smaller than the old ones.. ??
	@# so I'm deleting the existing one before re-ar-ing.
	rm -f $(OUT_LIBSTATIC_PATH)
	$(AR) rcs $(OUT_LIBSTATIC_PATH) $(COBJS)
	$(CC) -shared $(COBJS) $(LDFLAGS) -o $(OUT_LIBSHARED_PATH)

# release does the same thing as debug, only some CFLAGS and suffixes change.
release: debug

-include $(CDEPS)

$(BUILDDIR)/%_$(OBJS_SUFFIX).c.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $< -c $(CFLAGS) $(LDFLAGS) -o $@

clean:
	find $(BUILDDIR) \( -name '*.c.o' -o -name '*.c.d' \) -type f -delete || true

mrclean: clean
	rm -f $(BUILDDIR)/*.so $(BUILDDIR)/*.a
	find $(BUILDDIR) -name '*' -type d -delete || true

tests: $(TESTS)
	$(MAKE)
	$(CC) tests/test.c -g -Wall -Wextra -Iinclude -Lbuild -l:libargx.a -o tests/test.out
	tests/test.out --help --uint 2976579765 --string itiomorbovina
	tests/test.out -h -u 2976579765 -s itiomorbovina
	@rm -f tests/test.out

install:
	$(MAKE) release
	mkdir -p /usr/include/$(LIBNAME)
	cp $(HEADERS) /usr/include/$(LIBNAME)
	cp $(OUT_LIBSHARED_PATH) /usr/lib

uninstall:
	rm -f $(addprefix /usr/include/$(LIBNAME)/, $(notdir $(HEADERS)))
	rmdir /usr/include/$(LIBNAME) || true
	rm -f /usr/lib/$(notdir $(OUT_LIBSHARED_PATH))

.PHONY: all debug release clean mrclean install uninstall tests
