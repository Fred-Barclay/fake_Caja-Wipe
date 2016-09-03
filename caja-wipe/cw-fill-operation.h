/*
 *  caja-wipe - a caja extension to wipe file(s)
 *
 *  Copyright (C) 2009-2012 Colomban Wendling <ban@herbesfolles.org>
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

#ifndef CW_FILL_OPERATION_H
#define CW_FILL_OPERATION_H

#include <glib.h>
#include <glib-object.h>
#include <gsecuredelete/gsecuredelete.h>

#include "cw-operation.h"

G_BEGIN_DECLS


#define CW_TYPE_FILL_OPERATION          (cw_fill_operation_get_type ())
#define CW_FILL_OPERATION(o)            (G_TYPE_CHECK_INSTANCE_CAST ((o), CW_TYPE_FILL_OPERATION, CwFillOperation))
#define CW_FILL_OPERATION_CLASS(k)      (G_TYPE_CHECK_CLASS_CAST ((k), CW_TYPE_FILL_OPERATION, CwFillOperationClass))
#define CW_IS_FILL_OPERATION(o)         (G_TYPE_CHECK_INSTANCE_TYPE ((o), CW_TYPE_FILL_OPERATION))
#define CW_IS_FILL_OPERATION_CLASS(k)   (G_TYPE_CHECK_CLASS_TYPE ((k), CW_TYPE_FILL_OPERATION))
#define CW_FILL_OPERATION_GET_CLASS(o)  (G_TYPE_INSTANCE_GET_CLASS ((o), CW_TYPE_FILL_OPERATION, CwFillOperationClass))

typedef struct _CwFillOperation         CwFillOperation;
typedef struct _CwFillOperationClass    CwFillOperationClass;
typedef struct _CwFillOperationPrivate  CwFillOperationPrivate;

struct _CwFillOperation {
  GsdFillOperation parent;
  CwFillOperationPrivate *priv;
};

struct _CwFillOperationClass {
  GsdFillOperationClass parent;
};

/**
 * CwFillOperationError:
 * @CW_FILL_OPERATION_ERROR_MISSING_MOUNT: A file have no mount
 * @CW_FILL_OPERATION_ERROR_REMOTE_MOUNT: A mount is not local
 * @CW_FILL_OPERATION_ERROR_FAILED: An error occurred
 *
 * Possible errors from the %CW_FILL_OPERATION_ERROR domain.
 */
typedef enum
{
  CW_FILL_OPERATION_ERROR_MISSING_MOUNT,
  CW_FILL_OPERATION_ERROR_REMOTE_MOUNT,
  CW_FILL_OPERATION_ERROR_FAILED
} CwFillOperationError;

/**
 * CW_FILL_OPERATION_ERROR:
 *
 * Domain for error coming from a CajaWipe's fill operation.
 */
#define CW_FILL_OPERATION_ERROR (cw_fill_operation_error_quark ())

GQuark        cw_fill_operation_error_quark   (void) G_GNUC_CONST;
GType         cw_fill_operation_get_type      (void) G_GNUC_CONST;

gboolean      cw_fill_operation_filter_files  (GList    *paths,
                                               GList   **work_paths_,
                                               GList   **work_mounts_,
                                               GError  **error);
CwOperation  *cw_fill_operation_new           (void);


G_END_DECLS

#endif /* guard */
