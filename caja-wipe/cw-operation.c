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

#include "cw-operation.h"

#include <glib.h>
#include <glib-object.h>

#include <gsecuredelete/gsecuredelete.h>


static void   cw_operation_real_add_files   (CwOperation *self,
                                             GList       *files);


G_DEFINE_INTERFACE (CwOperation,
                    cw_operation,
                    GSD_TYPE_ZEROABLE_OPERATION)


static void
cw_operation_default_init (CwOperationInterface *iface)
{
  iface->add_files = cw_operation_real_add_files;
}

static void
cw_operation_real_add_files (CwOperation *self,
                             GList       *files)
{
  CwOperationInterface *iface = CW_OPERATION_GET_INTERFACE (self);
  
  for (; files; files = files->next) {
    iface->add_file (self, files->data);
  }
}

void
cw_operation_add_file (CwOperation *self,
                       const gchar *file)
{
  CW_OPERATION_GET_INTERFACE (self)->add_file (self, file);
}

void
cw_operation_add_files (CwOperation *self,
                        GList       *files)
{
  CW_OPERATION_GET_INTERFACE (self)->add_files (self, files);
}

