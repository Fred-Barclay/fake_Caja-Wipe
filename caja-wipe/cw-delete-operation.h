/*
 *  caja-wipe - a caja extension to wipe file(s)
 *
 *  Copyright (C) 2009-2012 Colomban Wendling <ban@herbesfolles.org>
 *  Copyright (C) 2016 Caja Write Authors
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

#ifndef CW_DELETE_OPERATION_H
#define CW_DELETE_OPERATION_H

#include <glib.h>
#include <glib-object.h>
#include <gsecuredelete/gsecuredelete.h>

#include "cw-operation.h"

G_BEGIN_DECLS


#define CW_TYPE_DELETE_OPERATION         (cw_delete_operation_get_type ())
#define CW_DELETE_OPERATION(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), CW_TYPE_DELETE_OPERATION, CwDeleteOperation))
#define CW_DELETE_OPERATION_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), CW_TYPE_DELETE_OPERATION, CwDeleteOperationClass))
#define CW_IS_DELETE_OPERATION(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CW_TYPE_DELETE_OPERATION))
#define CW_IS_DELETE_OPERATION_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), CW_TYPE_DELETE_OPERATION))
#define CW_DELETE_OPERATION_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), CW_TYPE_DELETE_OPERATION, CwDeleteOperationClass))

typedef struct _CwDeleteOperation         CwDeleteOperation;
typedef struct _CwDeleteOperationClass    CwDeleteOperationClass;

struct _CwDeleteOperation {
  GsdDeleteOperation parent;
};

struct _CwDeleteOperationClass {
  GsdDeleteOperationClass parent;
};


GType         cw_delete_operation_get_type    (void) G_GNUC_CONST;
CwOperation  *cw_delete_operation_new         (void);


G_END_DECLS

#endif /* guard */
