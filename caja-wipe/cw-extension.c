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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "cw-extension.h"

#include <libcaja-extension/caja-menu-provider.h>
#include <libcaja-extension/caja-file-info.h>

#include <glib.h>
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>
#include <gio/gio.h>

#include <gsecuredelete/gsecuredelete.h>

#include "cw-path-list.h"
#include "cw-operation-manager.h"
#include "cw-delete-operation.h"
#include "cw-fill-operation.h"
#include "cw-compat.h"
#include "cw-type-utils.h"


/* private prototypes */
static GList *cw_extension_real_get_file_items        (CajaMenuProvider *provider,
                                                       GtkWidget            *window,
                                                       GList                *files);
static GList *cw_extension_real_get_background_items  (CajaMenuProvider *provider,
                                                       GtkWidget            *window,
                                                       CajaFileInfo     *current_folder);
static void   cw_extension_menu_provider_iface_init   (CajaMenuProviderIface *iface);


#define ITEM_DATA_MOUNTPOINTS_KEY "Cw::Extension::mountpoints"
#define ITEM_DATA_PATHS_KEY       "Cw::Extension::paths"
#define ITEM_DATA_WINDOW_KEY      "Cw::Extension::parent-window"



GQuark
cw_extension_error_quark (void)
{
  static volatile gsize quark = 0;
  
  if (g_once_init_enter (&quark)) {
    GQuark q = g_quark_from_static_string ("CwExtensionError");
    
    g_once_init_leave (&quark, q);
  }
  
  return (GQuark) quark;
}

CW_DEFINE_TYPE_MODULE_WITH_CODE (CwExtension,
                                 cw_extension,
                                 G_TYPE_OBJECT,
                                 CW_TYPE_MODULE_IMPLEMENT_INTERFACE (CAJA_TYPE_MENU_PROVIDER,
                                                                     cw_extension_menu_provider_iface_init))



static void
cw_extension_menu_provider_iface_init (CajaMenuProviderIface *iface)
{
  iface->get_file_items       = cw_extension_real_get_file_items;
  iface->get_background_items = cw_extension_real_get_background_items;
}

static void
cw_extension_class_init (CwExtensionClass *class)
{
}

static void
cw_extension_init (CwExtension *self)
{
}

/* Runs the wipe operation */
static void
cw_extension_run_delete_operation (GtkWindow *parent,
                                   GList     *files)
{
  gchar  *confirm_primary_text = NULL;
  guint   n_items;
  
  n_items = g_list_length (files);
  if (n_items > 1) {
    confirm_primary_text = g_strdup_printf (g_dngettext(GETTEXT_PACKAGE,
    /* TRANSLATORS: singular is not really used, N is strictly >1 */
                                                        "Are you sure you want "
                                                        "to wipe the %u "
                                                        "selected items?",
                                                        /* plural form */
                                                        "Are you sure you want "
                                                        "to wipe the %u "
                                                        "selected items?",
                                                        n_items),
                                            n_items);
  } else if (n_items > 0) {
    gchar *name;
    
    name = g_filename_display_basename (files->data);
    confirm_primary_text = g_strdup_printf (_("Are you sure you want to wipe "
                                              "\"%s\"?"),
                                            name);
    g_free (name);
  }
  cw_operation_manager_run (
    parent, files,
    /* confirm dialog */
    confirm_primary_text,
    _("If you wipe an item, it will not be recoverable."),
    _("_Wipe"),
    gtk_image_new_from_stock (GTK_STOCK_DELETE, GTK_ICON_SIZE_BUTTON),
    /* progress dialog */
    _("Wiping files..."),
    /* operation launcher */
    cw_delete_operation_new (),
    /* failed dialog */
    _("Wipe failed."),
    /* success dialog */
    _("Wipe successful."),
    _("Item(s) have been successfully wiped.")
  );
  g_free (confirm_primary_text);
}

/* Runs the fill operation */
static void
cw_extension_run_fill_operation (GtkWindow *parent,
                                 GList     *paths,
                                 GList     *mountpoints)
{
  gchar  *confirm_primary_text = NULL;
  gchar  *success_secondary_text = NULL;
  guint   n_items;
  
  n_items = g_list_length (mountpoints);
  /* FIXME: can't truly use g_dngettext since the args are not the same */
  if (n_items > 1) {
    GList    *tmp;
    GString  *devices = g_string_new (NULL);
    
    for (tmp = mountpoints; tmp; tmp = g_list_next (tmp)) {
      gchar *name;
      
      name = g_filename_display_name (tmp->data);
      if (devices->len > 0) {
        if (! tmp->next) {
          /* TRANSLATORS: this is the last device names separator */
          g_string_append (devices, _(" and "));
        } else {
          /* TRANSLATORS: this is the device names separator (except last) */
          g_string_append (devices, _(", "));
        }
      }
      /* TRANSLATORS: this is the device name */
      g_string_append_printf (devices, _("\"%s\""), name);
      g_free (name);
    }
    confirm_primary_text = g_strdup_printf (_("Are you sure you want to wipe "
                                              "the available diskspace on the "
                                              "%s partitions or devices?"),
                                            devices->str);
    success_secondary_text = g_strdup_printf (_("Available diskspace on the "
                                                "partitions or devices %s "
                                                "have been successfully wiped."),
                                              devices->str);
    g_string_free (devices, TRUE);
  } else if (n_items > 0) {
    gchar *name;
    
    name = g_filename_display_name (mountpoints->data);
    confirm_primary_text = g_strdup_printf (_("Are you sure you want to wipe "
                                              "the available diskspace on the "
                                              "\"%s\" partition or device?"),
                                            name);
    success_secondary_text = g_strdup_printf (_("Available diskspace on the "
                                                "partition or device \"%s\" "
                                                "have been successfully wiped."),
                                              name);
    g_free (name);
  }
  cw_operation_manager_run (
    parent, paths,
    /* confirm dialog */
    confirm_primary_text,
    _("This operation may take a while."),
    _("_Wipe available diskspace"),
    gtk_image_new_from_stock (GTK_STOCK_CLEAR, GTK_ICON_SIZE_BUTTON),
    /* progress dialog */
    _("Wiping available diskspace..."),
    /* operation launcher */
    cw_fill_operation_new (),
    /* failed dialog */
    _("Wipe failed"),
    /* success dialog */
    _("Wipe successful"),
    success_secondary_text
  );
  g_free (confirm_primary_text);
  g_free (success_secondary_text);
}


static void
wipe_menu_item_activate_handler (GObject *item,
                                 gpointer data)
{
  cw_extension_run_delete_operation (g_object_get_data (item, ITEM_DATA_WINDOW_KEY),
                                     g_object_get_data (item, ITEM_DATA_PATHS_KEY));
}

static CajaMenuItem *
create_wipe_menu_item (CajaMenuProvider *provider,
                       const gchar          *item_name,
                       GtkWidget            *window,
                       GList                *paths)
{
  CajaMenuItem *item;
  
  item = caja_menu_item_new (item_name,
                                 _("Wipe"),
                                 _("Delete each selected item and overwrite its data"),
                                 GTK_STOCK_DELETE);
  g_object_set_data (G_OBJECT (item), ITEM_DATA_WINDOW_KEY, window);
  g_object_set_data_full (G_OBJECT (item), ITEM_DATA_PATHS_KEY,
                          cw_path_list_copy (paths),
                          (GDestroyNotify) cw_path_list_free);
  g_signal_connect (item, "activate",
                    G_CALLBACK (wipe_menu_item_activate_handler), NULL);
  
  return item;
}

static void
fill_menu_item_activate_handler (GObject *item,
                                 gpointer data)
{
  cw_extension_run_fill_operation (g_object_get_data (item, ITEM_DATA_WINDOW_KEY),
                                   g_object_get_data (item, ITEM_DATA_PATHS_KEY),
                                   g_object_get_data (item, ITEM_DATA_MOUNTPOINTS_KEY));
}

static CajaMenuItem *
create_fill_menu_item (CajaMenuProvider *provider,
                       const gchar          *item_name,
                       GtkWidget            *window,
                       GList                *files)
{
  CajaMenuItem *item        = NULL;
  GList            *mountpoints = NULL;
  GList            *folders     = NULL;
  GError           *err         = NULL;
  
  if (! cw_fill_operation_filter_files (files, &folders, &mountpoints, &err)) {
    g_warning (_("File filtering failed: %s"), err->message);
    g_error_free (err);
  } else {
    item = caja_menu_item_new (item_name,
                                   _("Wipe available diskspace"),
                                   _("Overwrite available diskspace in this device(s)"),
                                   GTK_STOCK_CLEAR);
    g_object_set_data (G_OBJECT (item), ITEM_DATA_WINDOW_KEY, window);
    g_object_set_data_full (G_OBJECT (item), ITEM_DATA_PATHS_KEY,
                            folders,
                            (GDestroyNotify) cw_path_list_free);
    g_object_set_data_full (G_OBJECT (item), ITEM_DATA_MOUNTPOINTS_KEY,
                            mountpoints,
                            (GDestroyNotify) cw_path_list_free);
    g_signal_connect (item, "activate",
                      G_CALLBACK (fill_menu_item_activate_handler), NULL);
  }
  
  return item;
}

/* adds @item to the #GList @items if not %NULL */
#define ADD_ITEM(items, item)                         \
  G_STMT_START {                                      \
    CajaMenuItem *ADD_ITEM__item = (item);        \
                                                      \
    if (ADD_ITEM__item != NULL) {                     \
      items = g_list_append (items, ADD_ITEM__item);  \
    }                                                 \
  } G_STMT_END

/* populates Caja' file menu */
static GList *
cw_extension_real_get_file_items (CajaMenuProvider *provider,
                                  GtkWidget            *window,
                                  GList                *files)
{
  GList *items = NULL;
  GList *paths;
  
  paths = cw_path_list_new_from_cfi_list (files);
  if (paths) {
    ADD_ITEM (items, create_wipe_menu_item (provider,
                                            "caja-wipe::files-items::wipe",
                                            window, paths));
    ADD_ITEM (items, create_fill_menu_item (provider,
                                            "caja-wipe::files-items::fill",
                                            window, paths));
  }
  cw_path_list_free (paths);
  
  return items;
}

/* populates Caja' background menu */
static GList *
cw_extension_real_get_background_items (CajaMenuProvider *provider,
                                        GtkWidget            *window,
                                        CajaFileInfo     *current_folder)
{
  GList *items = NULL;
  GList *paths = NULL;
  
  paths = g_list_append (paths, cw_path_from_cfi (current_folder));
  if (paths && paths->data) {
    ADD_ITEM (items, create_fill_menu_item (provider,
                                            "caja-wipe::background-items::fill",
                                            window, paths));
  }
  cw_path_list_free (paths);
  
  return items;
}

#undef ADD_ITEM
