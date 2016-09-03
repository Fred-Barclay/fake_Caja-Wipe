
Caja Wipe
========

Overview
========

Caja Wipe allows you to wipe files or available diskspace from within
Nautilus.

Dependencies
============

- libcaja-extension1 (http://live.gnome.org/Nautilus)
- GTK2 >= 2.6 or GTK3 (http://www.gtk.org)
- libgsecuredelete0 (http://wipetools.tuxfamily.org/libgsecuredelete.html)
- GConf 2.x (http://projects.gnome.org/gconf/) -- only needed for Nautilus 2.x (may not be needed for Caja)

Build dependencies
==================

- make
- gcc
- intltool
- gnome-doc-utils

Some distributions might also require one of `rarian-compat` or
`scrollkeeper` together with `gnome-doc-utils`.

If you are building from Git rather than a tarball, you also need
`autoconf`, `automake` (>= 1.11.1) and `libtool` (>= 2.2).

Quick installation instructions
===============================

  $ ./configure  
  $ make  
  $ make install

If you have both GTK2 and GTK3 development files installed and want to
build against GTK2, you should pass the ``--without-gtk3`` flag to
``./configure``.

If you are building from Git, you first need to run ``./autogen.sh``,
which will prepare the build system and generate the `configure` script.
