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
	libglitter_colour_space_convert_rasters_double.o\
	libglitter_colour_space_convert_rasters_float.o\
	libglitter_compose_double.o\
	libglitter_compose_float.o\
	libglitter_compose_uint16.o\
	libglitter_compose_uint32.o\
	libglitter_compose_uint64.o\
	libglitter_compose_uint8.o\
	libglitter_enable_acceleration.o\
	libglitter_desaturate_double.o\
	libglitter_desaturate_float.o\
	libglitter_create_render_context.o\
	libglitter_free_render_context.o\
	libglitter_get_colour_space_conversion_matrix_double.o\
	libglitter_get_colour_space_conversion_matrix_float.o\
	libglitter_per_channel_desaturate_double.o\
	libglitter_per_channel_desaturate_float.o\
	libglitter_reorder_rasters.o\
	libglitter_split_uint32_raster.o\
	libglitter_split_uint64_raster.o\
	libglitter_update_render_context.o

HDR =\
	common.h\
	libglitter.h

LOBJ = $(OBJ:.o=.lo)
TESTS = $(OBJ:.o=.test)


all: libglitter.a libglitter.$(LIBEXT) $(TESTS)
$(OBJ): $(HDR)
$(LOBJ): $(HDR)
libglitter_compose_float.o: libglitter_compose_double.c
libglitter_compose_uint16.o: libglitter_compose_uint64.c libglitter_compose_double.c
libglitter_compose_uint32.o: libglitter_compose_uint64.c libglitter_compose_double.c
libglitter_compose_uint64.o: libglitter_compose_double.c
libglitter_compose_uint8.o: libglitter_compose_uint64.c libglitter_compose_double.c
libglitter_desaturate_float.o: libglitter_desaturate_double.c
libglitter_per_channel_desaturate_float.o: libglitter_per_channel_desaturate_double.c
libglitter_split_uint32_raster.o: libglitter_split_uint64_raster.c
$(TESTS): $(HDR) libglitter.a

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

.c.lo:
	$(CC) -fPIC -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

.c.test:
	$(CC) -o $@ $< libglitter.a $(CFLAGS) $(CPPFLAGS) -DTEST $(LDFLAGS)

libglitter.a: $(OBJ)
	@rm -f -- $@
	$(AR) rc $@ $(OBJ)
	$(AR) ts $@ > /dev/null

libglitter.$(LIBEXT): $(LOBJ)
	$(CC) $(LIBFLAGS) -o $@ $(LOBJ) $(LDFLAGS)

check: $(TESTS)
	@for t in $(TESTS); do printf './%s\n' $$t; ./$$t || exit 1; done

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
	-rm -f -- *.o *.a *.lo *.su *.so *.so.* *.dll *.dylib *.test
	-rm -f -- *.gch *.gcov *.gcno *.gcda *.$(LIBEXT)

.SUFFIXES:
.SUFFIXES: .lo .o .c .test

.PHONY: all check install uninstall clean
