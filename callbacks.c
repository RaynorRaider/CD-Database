#include "cdapp_gnome.h"

extern GtkWidget *treeview;
extern GtkWidget *app;
extern GtkWidget *appbar;
extern GtkWidget *artist_entry;
extern GtkWidget *title_entry;
extern GtkWidget *catalogue_entry;
extern GtkWidget *catalogue_to_add_entry;
extern GtkWidget *tracks_entry[15];
extern GtkWidget *catalogue_to_delete_entry;

static GtkWidget *addcd_dialog;
static GtkWidget *addtracks_dialog;
static GtkWidget *delete_CD_dialog;
/* close database before quit the app */
void quit_app(GtkWidget *window, gpointer data)
{
    database_end();
    gtk_main_quit();
}

/* a dialog to make sure you want to quit */
gboolean confirm_exit()
{
    gint result;
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_YES_NO, "确定要退出吗?");

    result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    return (result == GTK_RESPONSE_YES);
}

/* before GTK+destroy */
gboolean delete_event_handler(GtkWidget *window, GdkEvent *event, gpointer data)
{
    return !confirm_exit();
}

void addcd_dialog_button_clicked(GtkDialog *dialog, gint response, gpointer userdata)
{
    const gchar *artist_const;
    const gchar *title_const;
    const gchar *catalogue_const;
    gchar artist[200];
    gchar title[200];
    gchar catalogue[200];
    int cd_id;

    if(response == GTK_RESPONSE_ACCEPT)
    {
        artist_const = gtk_entry_get_text(GTK_ENTRY(artist_entry));
        title_const = gtk_entry_get_text(GTK_ENTRY(title_entry));
        catalogue_const = gtk_entry_get_text(GTK_ENTRY(catalogue_entry));

        strcpy(artist, artist_const);
        strcpy(title, title_const);
        strcpy(catalogue, catalogue_const);

        add_cd(artist, title, catalogue, &cd_id);
    }

    addcd_dialog = NULL;
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void addtrack_dialog_button_clicked(GtkDialog *dialog, gint response, gpointer userdata)
{
    const gchar *catalogue_const;
    gchar catalogue[200];
    struct current_tracks_st track_st;
    int cd_id;
    int i = 0;
    if(response == GTK_RESPONSE_ACCEPT)
    {
        memset(&track_st, 0, sizeof(track_st));
        catalogue_const = gtk_entry_get_text(GTK_ENTRY(catalogue_to_add_entry));
        strcpy(catalogue, catalogue_const);
        cd_id = find_cd_with_catalogue(catalogue);
        if(cd_id != -1)
        {
            //delete all tracks in this CD
            delete_tracks(cd_id);
            track_st.cd_id = cd_id;
            while(i<15)
            {
                strcpy(track_st.track[i], gtk_entry_get_text(GTK_ENTRY(tracks_entry[i])));
                i++;
            }
            add_tracks(&track_st);
        }
        else
        {
            GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                    GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,"CD not exist");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }
    addtracks_dialog = NULL;
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void delete_CD_dialog_button_clicked(GtkDialog *dialog, gint response, gpointer userdata)
{
    const gchar *catalogue_const;
    gchar catalogue[200];
    int cd_id;
    if(response == GTK_RESPONSE_ACCEPT)
    {
        catalogue_const = gtk_entry_get_text(GTK_ENTRY(catalogue_to_delete_entry));
        strcpy(catalogue, catalogue_const);
        cd_id = find_cd_with_catalogue(catalogue);
        if(cd_id != -1)
        {
            delete_cd(cd_id);
        }
        else
        {
            GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                    GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,"CD not exist");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }

    delete_CD_dialog = NULL;
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

/* get the search result, and fill the GtkTreeView */
void on_search_button_clicked(GtkButton *button, gpointer userdata)
{
    struct cd_search_st cd_res;
    struct current_cd_st cd;
    struct current_tracks_st ct;
    gint res1, res2, res3;
    gchar track_title[110];
    const gchar *search_string_const;
    gchar search_string[200];
    gchar search_text[200];
    gint i = 0, j = 0;

    GtkTreeStore *tree_store;
    GtkTreeIter parent_iter, child_iter;

    memset(&track_title, 0, sizeof(track_title));
    
    search_string_const = gtk_entry_get_text(GTK_ENTRY(userdata));
    strcpy(search_string, search_string_const);
    res1 = find_cds(search_string, &cd_res);

    /* update the appbar to tell user the result */
    sprintf(search_text, "Display %d result(s) for search string '%s'",
                MIN(res1, MAX_CD_RESULT), search_string);
    gnome_appbar_push(GNOME_APPBAR(appbar), search_text);

    tree_store = gtk_tree_store_new(N_COLUMNS, 
            G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    while(i<res1 && i<MAX_CD_RESULT)
    {
        res2 = get_cd(cd_res.cd_id[i], &cd);

        /* Add a new row to the model */
        gtk_tree_store_append(tree_store, &parent_iter, NULL);
        gtk_tree_store_set(tree_store, &parent_iter,
                COLUMN_TITLE, cd.title,
                COLUMN_ARTIST, cd.artist_name,
                COLUMN_CATALOGUE, cd.catalogue, -1);
        
        res3 = get_cd_tracks(cd_res.cd_id[i], &ct);
        i++;
        j = 0;
        /* Populate the tree with the current cd's tracks */
        while(j < res3)
        {
            sprintf(track_title, " Track %d. ", j+1);
            strcat(track_title, ct.track[j]);
            j++;
            gtk_tree_store_append(tree_store, &child_iter, &parent_iter);
            gtk_tree_store_set(tree_store, &child_iter, COLUMN_TITLE, track_title, -1);
        }

    }
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(tree_store));
}
          
void on_addcd_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    if(addcd_dialog != NULL)
        return;

    addcd_dialog = create_addcd_dialog();
    gtk_widget_show_all(addcd_dialog);
}

gboolean close_app(GtkWidget *window, gpointer data)
{
    gboolean exit;
    if((exit = confirm_exit()))
    {
        quit_app(NULL, NULL);
    }
    return exit;
}

void on_about_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    const char *authors[] = {"Neil Matthew","Richard Stones","李骏", NULL};
    GtkWidget *about = gnome_about_new("CD Database", "1.0",
            "(c) RaynorRaider ", "Beginning Linux Programming",
            (const char **)authors, NULL, "Translators", NULL);
    gtk_widget_show(about);
}

void count_db(GtkWidget *window, gpointer data)
{
    int count_data[3];
    database_count(&count_data);
    GtkWidget *dialog = gtk_message_dialog_new(NULL, 
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,"CD数目: %d张\n总曲目: %d首\n音乐家: %d人",
            count_data[0], count_data[1], count_data[2]);

    int result = gtk_dialog_run(GTK_DIALOG(dialog)); 
    gtk_widget_destroy(dialog);
}

void on_add_tracks_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    if(addtracks_dialog != NULL)
        return;

    addtracks_dialog = create_addtracks_dialog();
    gtk_widget_show_all(addtracks_dialog);

}

void on_delete_cd_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    if(delete_CD_dialog != NULL)
        return;

    delete_CD_dialog = create_delete_CD_dialog();
    gtk_widget_show_all(delete_CD_dialog);

}
















