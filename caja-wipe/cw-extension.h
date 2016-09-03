/*
 *  caja-wipe - a caja extension to wipe file(s)
 *
 *  Copyright (C) 2009-2011 Colomban Wendling <ban@herbesfolles.org>
 *  Copyright (C) 2016 Caja Wipe Authors
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef CW_EXTENSION_H
#define CW_EXTENSION_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS


#define CW_TYPE_EXTENSION   (cw_extension_get_type ())
#define CW_EXTENSION(o)     (G_TYPE_CHECK_INSTANCE_CAST ((o), CW_TYPE_EXTENSION, CwExtension))
#define CW_IS_EXTENSION(o)  (G_TYPE_CHECK_INSTANCE_TYPE ((o), CW_TYPE_EXTENSION))
typedef struct _CwExtension       CwExtension;
typedef struct _CwExtensionClass  CwExtensionClass;

#define CW_EXTENSION_ERROR (cw_extension_error_quark ())

typedef enum {
  CW_EXTENSION_ERROR_SPAWN_FAILED,
  CW_EXTENSION_ERROR_CHILD_CRASHED,
  CW_EXTENSION_ERROR_CHILD_FAILED,
  CW_EXTENSION_ERROR_UNSUPPORTED_LOCATION,
  CW_EXTENSION_ERROR_NOT_IMPLEMENTED,
  CW_EXTENSION_ERROR_FAILED
} CwExtensionError;

struct _CwExtension {
  GObject parent_slot;
};

struct _CwExtensionClass {
  GObjectClass parent_slot;
};

GType   cw_extension_get_type         (void) G_GNUC_CONST;
GType   cw_extension_register_type    (GTypeModule *module);
GQuark  cw_extension_error_quark      (void) G_GNUC_CONST;


G_END_DECLS

#endif /* guard */
