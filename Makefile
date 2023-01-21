.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

OS = linux
# Linux:   linux
# Mac OS:  macos
# Windows: windows
include mk/$(OS).mk


LIB_MAJOR = 1
LIB_MINOR = 0
LIB_VERSION = $(LIB_MAJOR).$(LIB_MINOR)
LIB_NAME = glitter


OBJ =\
	libglitter_compose_double.o\
	libglitter_compose_float.o\
	libglitter_compose_uint16.o\
	libglitter_compose_uint32.o\
	libglitter_compose_uint64.o\
	libglitter_compose_uint8.o\
	libglitter_create_render_context.o\
	libglitter_free_render_context.o\
	libglitter_update_render_context.o

HDR =\
	common.h\
	libglitter.h

LOBJ = $(OBJ:.o=.lo)


all: libglitter.a libglitter.$(LIBEXT)
$(OBJ): $(HDR)
$(LOBJ): $(HDR)
libglitter_compose_float.o: libglitter_compose_double.c
libglitter_compose_uint16.o: libglitter_compose_uint64.c libglitter_compose_double.c
libglitter_compose_uint32.o: libglitter_compose_uint64.c libglitter_compose_double.c
libglitter_compose_uint64.o: libglitter_compose_double.c
libglitter_compose_uint8.o: libglitter_compose_uint64.c libglitter_compose_double.c

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

.c.lo:
	$(CC) -fPIC -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

libglitter.a: $(OBJ)
	@rm -f -- $@
	$(AR) rc $@ $(OBJ)
	$(AR) ts $@ > /dev/null

libglitter.$(LIBEXT): $(LOBJ)
	$(CC) $(LIBFLAGS) -o $@ $(LOBJ) $(LDFLAGS)

install: libglitter.a libglitter.$(LIBEXT)
	mkdir -p -- "$(DESTDIR)$(PREFIX)/lib"
	mkdir -p -- "$(DESTDIR)$(PREFIX)/include"
	cp -- libglitter.a "$(DESTDIR)$(PREFIX)/lib/"
	cp -- libglitter.$(LIBEXT) "$(DESTDIR)$(PREFIX)/lib/libglitter.$(LIBMINOREXT)"
	$(FIX_INSTALL_NAME) "$(DESTDIR)$(PREFIX)/lib/libglitter.$(LIBMINOREXT)"
	ln -sf -- libglitter.$(LIBMINOREXT) "$(DESTDIR)$(PREFIX)/lib/libglitter.$(LIBMAJOREXT)"
	ln -sf -- libglitter.$(LIBMAJOREXT) "$(DESTDIR)$(PREFIX)/lib/libglitter.$(LIBEXT)"
	cp -- libglitter.h "$(DESTDIR)$(PREFIX)/include/"

uninstall:
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libglitter.a"
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libglitter.$(LIBMAJOREXT)"
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libglitter.$(LIBMINOREXT)"
	-rm -f -- "$(DESTDIR)$(PREFIX)/lib/libglitter.$(LIBEXT)"
	-rm -f -- "$(DESTDIR)$(PREFIX)/include/libglitter.h"

clean:
	-rm -f -- *.o *.a *.lo *.su *.so *.so.* *.dll *.dylib
	-rm -f -- *.gch *.gcov *.gcno *.gcda *.$(LIBEXT)

.SUFFIXES:
.SUFFIXES: .lo .o .c

.PHONY: all install uninstall clean
