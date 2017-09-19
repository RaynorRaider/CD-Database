#include"cdapp_gnome.h"

GtkWidget *treeview;
GtkWidget *appbar;

//create dialog to add cd
GtkWidget *artist_entry;
GtkWidget *title_entry;
GtkWidget *catalogue_entry;
//create dialog to add tracks
GtkWidget *tracks_entry[15];
GtkWidget *catalogue_to_add_entry;
//create dialog to delete CD
GtkWidget *catalogue_to_delete_entry;
//create dialog to login
GtkWidget *username_entry;
GtkWidget *password_entry;

/* main window pointer */
static GtkWidget *app;

void add_widget_with_label(GtkContainer *box, gchar *caption, GtkWidget *widget)
{
    GtkWidget *label = gtk_label_new(caption);
    GtkWidget *hbox = gtk_hbox_new(TRUE, 4);
    gtk_container_add(GTK_CONTAINER(hbox), label);
    gtk_container_add(GTK_CONTAINER(hbox), widget);
    gtk_container_add(box, hbox);
}


/* the meun */
/************/
static GnomeUIInfo filemenu[] =
{
    GNOMEUIINFO_MENU_NEW_ITEM("_New CD", NULL, on_addcd_activate, NULL),
    GNOMEUIINFO_SEPARATOR,
    GNOMEUIINFO_MENU_EXIT_ITEM(close_app, NULL),
    GNOMEUIINFO_END
};

static GnomeUIInfo helpmenu[] =
{
    GNOMEUIINFO_MENU_ABOUT_ITEM(on_about_activate, NULL),
    GNOMEUIINFO_END
};

static GnomeUIInfo menubar[] =
{
    GNOMEUIINFO_MENU_FILE_TREE(filemenu),
    GNOMEUIINFO_MENU_HELP_TREE(helpmenu),
    GNOMEUIINFO_END
};

/*************/

/* Create the main window(return a pointer) */
GtkWidget *create_main_window()
{
    GtkWidget *toolbar;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *search_button;
    GtkWidget *scrolledwindow;
    GtkCellRenderer *renderer;

    app = gnome_app_new("GnomeCD", "CD Database");

    gtk_window_set_position(GTK_WINDOW(app), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(app), 540, 480);

    gnome_app_create_menus(GNOME_APP(app), menubar);

    /* create the toolbar */
    toolbar = gtk_toolbar_new();
    gnome_app_add_toolbar(GNOME_APP(app), GTK_TOOLBAR(toolbar), "toolbar",
            BONOBO_DOCK_ITEM_BEH_EXCLUSIVE, BONOBO_DOCK_TOP, 1, 0, 0);
    gtk_container_set_border_width(GTK_CONTAINER(toolbar), 1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar), "gtk-add", "Add new CD", NULL,
            GTK_SIGNAL_FUNC(on_addcd_activate), NULL, -1);
    gtk_toolbar_insert_space(GTK_TOOLBAR(toolbar), 1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar), "gtk-edit", "Add new tracks",
            NULL, GTK_SIGNAL_FUNC(on_add_tracks_activate), NULL, -1);
    gtk_toolbar_insert_space(GTK_TOOLBAR(toolbar), 1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar), "gtk-delete", "delete a CD",
            NULL, GTK_SIGNAL_FUNC(on_delete_cd_activate), NULL, -1);
    gtk_toolbar_insert_space(GTK_TOOLBAR(toolbar), 1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar), "gtk-info", "Count the CD and Tracks",
            NULL, GTK_SIGNAL_FUNC(count_db), NULL, -1);
    gtk_toolbar_insert_space(GTK_TOOLBAR(toolbar), 1);
    gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar), "gtk-quit", "Quit the Application",
            NULL, GTK_SIGNAL_FUNC(quit_app), NULL, -1);


    /* create the search component */
    label = gtk_label_new("关键字查找 :");
    entry = gtk_entry_new();
    search_button = gtk_button_new_with_label("查找");

    /* a scrolled window */
    scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
            GTK_POLICY_AUTOMATIC,
            GTK_POLICY_AUTOMATIC);

    vbox = gtk_vbox_new(FALSE, 0);
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 6);
    gtk_box_pack_start(GTK_BOX(hbox), search_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 0);

    /* GtkTreeView component */
    treeview = gtk_tree_view_new();
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
            COLUMN_TITLE, "专辑名", renderer, "text", COLUMN_TITLE, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
            COLUMN_ARTIST, "音乐家", renderer, "text", COLUMN_ARTIST, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview),
            COLUMN_CATALOGUE, "标签", renderer, "text", COLUMN_CATALOGUE, NULL);
    gtk_tree_view_set_search_column(GTK_TREE_VIEW(treeview), COLUMN_TITLE);
    gtk_container_add(GTK_CONTAINER(scrolledwindow), treeview);
    
    
    gnome_app_set_contents(GNOME_APP(app), vbox);

    appbar = gnome_appbar_new(FALSE, TRUE, GNOME_PREFERENCES_NEVER);
    gnome_app_set_statusbar(GNOME_APP(app), appbar);

    gnome_app_install_menu_hints(GNOME_APP(app), menubar);

    g_signal_connect(GTK_OBJECT(search_button), "clicked",
            GTK_SIGNAL_FUNC(on_search_button_clicked), entry);

    g_signal_connect(GTK_OBJECT(app), "delete_event",
            GTK_SIGNAL_FUNC(delete_event_handler), NULL);

    g_signal_connect(GTK_OBJECT(app), "destroy", 
            GTK_SIGNAL_FUNC(quit_app), NULL);

    return app;
}

/* a dialog to add a cd */
GtkWidget *create_addcd_dialog()
{
    artist_entry = gtk_entry_new();
    title_entry = gtk_entry_new();
    catalogue_entry = gtk_entry_new();

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Add CD",GTK_WINDOW(app),
            GTK_DIALOG_DESTROY_WITH_PARENT, 
            GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
            GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
            NULL);
    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                "音乐家", artist_entry);
    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                "标题", title_entry);
    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                "标签", catalogue_entry);

    g_signal_connect(GTK_OBJECT(dialog), "response",
            GTK_SIGNAL_FUNC(addcd_dialog_button_clicked), NULL);
    return dialog;
}

/* a dialog to add tracks */
GtkWidget *create_addtracks_dialog()
{
    catalogue_to_add_entry = gtk_entry_new();
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Add track",GTK_WINDOW(app),
            GTK_DIALOG_DESTROY_WITH_PARENT, 
            GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
            GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
            NULL);
    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                "CD标签", catalogue_to_add_entry);
    int i = 0;
    char label[10];
    while(i<15)
    {
        memset(label, 0, sizeof(label));
        sprintf(label, "track %d", i+1);
        tracks_entry[i] = gtk_entry_new();
        add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                label, tracks_entry[i]);
        i++;
    }
    g_signal_connect(GTK_OBJECT(dialog), "response",
            GTK_SIGNAL_FUNC(addtrack_dialog_button_clicked), NULL);
    return dialog;

}

/* a dialog to delete a CD */
GtkWidget *create_delete_CD_dialog()
{
    catalogue_to_delete_entry = gtk_entry_new();

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Delete CD",GTK_WINDOW(app),
            GTK_DIALOG_DESTROY_WITH_PARENT, 
            GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
            GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
            NULL);
    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                "待删除CD标签", catalogue_to_delete_entry);

    g_signal_connect(GTK_OBJECT(dialog), "response",
            GTK_SIGNAL_FUNC(delete_CD_dialog_button_clicked), NULL);
    return dialog;
}



/* a dialog to login in */
GtkWidget *create_login_dialog()
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Database Login",
            GTK_WINDOW(app), GTK_DIALOG_MODAL,
            GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
            GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
            NULL);
    username_entry = gtk_entry_new();
    password_entry = gtk_entry_new();

    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
            "Username", username_entry);
    add_widget_with_label(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), "Password",
            password_entry);

    gtk_widget_show_all(GTK_WIDGET(GTK_DIALOG(dialog)->vbox));
    return dialog;
}

