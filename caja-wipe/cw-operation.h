/*
 *  caja-wipe - a caja extension to wipe file(s)
 *
 *  Copyright (C) 2012 Colomban Wendling <ban@herbesfolles.org>
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

#ifndef CW_OPERATION_H
#define CW_OPERATION_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS


#define CW_TYPE_OPERATION             (cw_operation_get_type ())
#define CW_OPERATION(o)               (G_TYPE_CHECK_INSTANCE_CAST ((o), CW_TYPE_OPERATION, CwOperation))
#define CW_IS_OPERATION(o)            (G_TYPE_CHECK_INSTANCE_TYPE ((o), CW_TYPE_OPERATION))
#define CW_OPERATION_GET_INTERFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), CW_TYPE_OPERATION, CwOperationInterface))

typedef struct _CwOperation           CwOperation;
typedef struct _CwOperationInterface  CwOperationInterface;

struct _CwOperationInterface {
  GTypeInterface parent;

  void   (*add_file)        (CwOperation *self,
                             const gchar *path);
  void   (*add_files)       (CwOperation *self,
                             GList       *files);
};


GType   cw_operation_get_type   (void) G_GNUC_CONST;

void    cw_operation_add_file   (CwOperation *self,
                                 const gchar *path);
void    cw_operation_add_files  (CwOperation *self,
                                 GList       *files);


G_END_DECLS

#endif /* guard */
