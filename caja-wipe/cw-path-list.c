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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "cw-path-list.h"

#include <string.h>
#include <glib.h>
#include <libcaja-extension/caja-file-info.h>
#ifdef HAVE_GCONF
# include <gconf/gconf-client.h>
#endif


/* checks whether a #CajaFileInfo have the given URI scheme */
static gboolean
cfi_has_uri_scheme (CajaFileInfo *cfi,
                    const gchar      *scheme)
{
  gboolean  matches = FALSE;
  gchar    *cfi_scheme;
  
  cfi_scheme = caja_file_info_get_uri_scheme (cfi);
  if (cfi_scheme == scheme || strcmp (cfi_scheme, scheme) == 0) {
    matches = TRUE;
  }
  g_free (cfi_scheme);
  
  return matches;
}

/* gets the Caja' desktop path (to handle x-caja-desktop:// URIs)
 * heavily based on the implementation from caja-open-terminal */
static gchar *
get_desktop_path (void)
{
  gchar *path = NULL;
  
#ifdef HAVE_GCONF
  if (! path) {
    GConfClient *conf_client;
    
    conf_client = gconf_client_get_default ();
    if (gconf_client_get_bool (conf_client,
                               "/apps/caja/preferences/desktop_is_home_dir",
                               NULL)) {
      path = g_strdup (g_get_home_dir ());
    }
    g_object_unref (conf_client);
  }
#endif /* HAVE_GCONF */
  
  if (! path) {
    path = g_strdup (g_get_user_special_dir (G_USER_DIRECTORY_DESKTOP));
  }
  
  return path;
}

/* gets the path of a #CajaFileInfo.
 * this is different from getting if GFile then getting the path since it tries
 * handle x-caja-desktop */
gchar *
cw_path_from_cfi (CajaFileInfo *cfi)
{
  GFile *file;
  gchar *path;
  
  file = caja_file_info_get_location (cfi);
  path = g_file_get_path (file);
  if (! path) {
    if (cfi_has_uri_scheme (cfi, "x-caja-desktop")) {
      path = get_desktop_path ();
    }
  }
  
  return path;
}

/* frees a list of paths */
void
cw_path_list_free (GList *paths)
{
  g_list_foreach (paths, (GFunc) g_free, NULL);
  g_list_free (paths);
}

/* copies a list of paths
 * free the returned list with cw_path_list_free() */
GList *
cw_path_list_copy (GList *src)
{
  GList *paths = NULL;
  
  while (src) {
    paths = g_list_prepend (paths, g_strdup (src->data));
    src = g_list_next (src);
  }
  paths = g_list_reverse (paths);
  
  return paths;
}

/* converts a list of #CajaFileInfo to a list of paths.
 * free the returned list with cw_path_list_free()
 * 
 * Returns: The list of paths on success, or %NULL on failure. This function
 *          will always fail on non-local-mounted (then without paths) files */
GList *
cw_path_list_new_from_cfi_list (GList *cfis)
{
  gboolean  success = TRUE;
  GList    *paths   = NULL;
  
  while (cfis && success) {
    gchar *path;
    
    path = cw_path_from_cfi (cfis->data);
    if (path) {
      paths = g_list_prepend (paths, path);
    } else {
      success = FALSE;
    }
    cfis = g_list_next (cfis);
  }
  if (! success) {
    cw_path_list_free (paths);
    paths = NULL;
  } else {
    paths = g_list_reverse (paths);
  }
  
  return paths;
}
