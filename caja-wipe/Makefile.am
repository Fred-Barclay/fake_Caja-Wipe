CAJA_extensiondir = $(CAJA_EXTENSIONS_DIR)

CAJA_extension_LTLIBRARIES = libcaja-wipe.la

localedir = $(datadir)/locale

libcaja_wipe_la_SOURCES  = extension.c \
                               cw-compat.h \
                               cw-delete-operation.c \
                               cw-delete-operation.h \
                               cw-extension.c \
                               cw-extension.h \
                               cw-fill-operation.c \
                               cw-fill-operation.h \
                               cw-operation.c \
                               cw-operation.h \
                               cw-operation-manager.c \
                               cw-operation-manager.h \
                               cw-path-list.c \
                               cw-path-list.h \
                               cw-progress-dialog.c \
                               cw-progress-dialog.h \
                               cw-type-utils.h
libcaja_wipe_la_CPPFLAGS = -DG_LOG_DOMAIN=\"$(PACKAGE)\" \
                               -DLOCALEDIR=\"$(localedir)\"
libcaja_wipe_la_CFLAGS   = @GLIB_CFLAGS@ \
                               @GTK_CFLAGS@ \
                               @GCONF_CFLAGS@ \
                               @LIBCAJA_EXTENSION_CFLAGS@ \
                               @GSECUREDELETE_CFLAGS@ \
                               @GIO_UNIX_CFLAGS@
libcaja_wipe_la_LDFLAGS  = -module -avoid-version \
                               @GLIB_LIBS@ \
                               @GTK_LIBS@ \
                               @GCONF_LIBS@ \
                               @LIBCAJA_EXTENSION_LIBS@ \
                               @GSECUREDELETE_LIBS@ \
                               @GIO_UNIX_LIBS@
