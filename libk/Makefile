DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -Iinclude
LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk

CFLAGS:=$(CFLAGS)
CPPFLAGS:=$(CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS)

FREEOBJS=\
assert/assert.o \
ctype/isalnum.o \
ctype/isalpha.o \
ctype/isblank.o \
ctype/iscntrl.o \
ctype/isdigit.o \
ctype/isgraph.o \
ctype/islower.o \
ctype/isprint.o \
ctype/ispunct.o \
ctype/isspace.o \
ctype/isupper.o \
ctype/isxdigit.o \
ctype/tolower.o \
ctype/toupper.o \
math/cos.o \
math/sin.o \
math/tan.o \
stdio/printf.o \
stdio/putchar.o \
stdio/puts.o \
stdlib/abort.o \
stdlib/itoa.o \
stdlib/ftoa.o \
stdlib/atoi.o \
stdlib/atof.o \
stdlib/malloc.o \
stdlib/free.o \
string/memcmp.o \
string/memcpy.o \
string/memmove.o \
string/memset.o \
string/strlen.o \
string/strcmp.o \
string/strcpy.o \
string/strncpy.o \
string/append.o \
string/backspace.o \
string/strtok.o \

HOSTEDOBJS=

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

BINARIES=libc.a libk.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .S

all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS) -D__is_libc

.c.S:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS) -D__is_libc

.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.S.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
