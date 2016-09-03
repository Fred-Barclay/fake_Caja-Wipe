/*
 *  caja-wipe - a caja extension to wipe file(s)
 *
 *  Copyright (C) 2009-2011 Colomban Wendling <ban@herbesfolles.org>
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

#ifndef CW_PROGRESS_DIALOG_H
#define CW_PROGRESS_DIALOG_H

#include <stdarg.h>
#include <glib.h>
#include <gtk/gtk.h>


G_BEGIN_DECLS


#define CW_TYPE_PROGRESS_DIALOG         (cw_progress_dialog_get_type ())
#define CW_PROGRESS_DIALOG(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), CW_TYPE_PROGRESS_DIALOG, CwProgressDialog))
#define CW_PROGRESS_DIALOG_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), CW_TYPE_PROGRESS_DIALOG, CwProgressDialogClass))
#define CW_IS_PROGRESS_DIALOG(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), CW_TYPE_PROGRESS_DIALOG))
#define CW_IS_PROGRESS_DIALOG_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), CW_TYPE_PROGRESS_DIALOG))
#define CW_PROGRESS_DIALOG_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), CW_TYPE_PROGRESS_DIALOG, CwProgressDialogClass))

typedef struct _CwProgressDialog        CwProgressDialog;
typedef struct _CwProgressDialogClass   CwProgressDialogClass;
typedef struct _CwProgressDialogPrivate CwProgressDialogPrivate;

struct _CwProgressDialog {
  GtkDialog parent_instance;
  CwProgressDialogPrivate *priv;
};

struct _CwProgressDialogClass {
  GtkDialogClass parent_class;
};

#define CW_PROGRESS_DIALOG_RESPONSE_COMPLETE 1


GType         cw_progress_dialog_get_type                   (void) G_GNUC_CONST;

GtkWidget    *cw_progress_dialog_new                        (GtkWindow       *parent,
                                                             GtkDialogFlags   flags,
                                                             const gchar     *format,
                                                             ...) G_GNUC_PRINTF (3, 4);
void          cw_progress_dialog_set_fraction               (CwProgressDialog  *dialog,
                                                             gdouble            fraction);
gdouble       cw_progress_dialog_get_fraction               (CwProgressDialog  *dialog);
void          cw_progress_dialog_pulse                      (CwProgressDialog  *dialog);
void          cw_progress_dialog_set_pulse_step             (CwProgressDialog  *dialog,
                                                             gdouble            fraction);
gdouble       cw_progress_dialog_get_pulse_step             (CwProgressDialog  *dialog);
void          cw_progress_dialog_set_progress_text          (CwProgressDialog  *dialog,
                                                             const gchar       *format,
                                                             ...) G_GNUC_PRINTF (2, 3);
const gchar  *cw_progress_dialog_get_progress_text          (CwProgressDialog  *dialog);
void          cw_progress_dialog_set_text                   (CwProgressDialog  *dialog,
                                                             const gchar       *format,
                                                             ...) G_GNUC_PRINTF (2, 3);
const gchar  *cw_progress_dialog_get_text                   (CwProgressDialog  *dialog);
void          cw_progress_dialog_cancel                     (CwProgressDialog  *dialog);
gboolean      cw_progress_dialog_is_canceled                (CwProgressDialog  *dialog);
void          cw_progress_dialog_finish                     (CwProgressDialog  *dialog,
                                                             gboolean           success);
gboolean      cw_progress_dialog_is_finished                (CwProgressDialog  *dialog);
void          cw_progress_dialog_set_has_close_button       (CwProgressDialog  *dialog,
                                                             gboolean           has_close_button);
gboolean      cw_progress_dialog_get_has_close_button       (CwProgressDialog  *dialog);
void          cw_progress_dialog_set_has_cancel_button      (CwProgressDialog  *dialog,
                                                             gboolean           has_close_button);
gboolean      cw_progress_dialog_get_has_cancel_button      (CwProgressDialog  *dialog);

void          cw_progress_dialog_set_auto_hide_action_area  (CwProgressDialog  *dialog,
                                                             gboolean           auto_hide);
gboolean      cw_progress_dialog_get_auto_hide_action_area  (CwProgressDialog  *dialog);


G_END_DECLS

#endif /* guard */
