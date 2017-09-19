#include<gnome.h>
#include"app_mysql.h"

enum {
    COLUMN_TITLE,
    COLUMN_ARTIST,
    COLUMN_CATALOGUE,
    N_COLUMNS
};

/* interface.c */
GtkWidget *create_main_window();
GtkWidget *create_login_dialog();
GtkWidget *create_addcd_dialog();
GtkWidget *create_addtracks_dialog();
GtkWidget *create_delete_CD_dialog();
/* callbacks.c */
void quit_app(GtkWidget *window, gpointer data);
gboolean delete_event_handler(GtkWidget *window, GdkEvent *event, gpointer data);
gboolean close_app(GtkWidget *, gpointer data);
void addcd_dialog_button_clicked(GtkDialog *dialog, gint response, gpointer userdata);
void on_addcd_activate(GtkMenuItem *widget, gpointer user_data);
void on_about_activate(GtkMenuItem *widget, gpointer user_data);
void on_search_button_clicked(GtkButton *widget, gpointer userdata);
void count_db(GtkWidget *window, gpointer data);
void on_add_tracks_activate(GtkMenuItem *menuitem, gpointer user_data);
void addtrack_dialog_button_clicked(GtkDialog *dialog, gint response, gpointer userdata);
void on_delete_cd_activate(GtkMenuItem *menuitem, gpointer user_data);
void delete_CD_dialog_button_clicked(GtkDialog *dialog, gint response, gpointer userdata);

