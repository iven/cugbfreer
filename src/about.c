#include "interface.h"

void show_about (GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    const gchar *authors [] = {
        "罗天 <ivenvd@gmail.com>",
        NULL
    };
    dialog = gtk_about_dialog_new ();
    gtk_about_dialog_set_name (GTK_ABOUT_DIALOG (dialog), "Cugb Freer");
    gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (dialog), "0.1 Alpha1");
    gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (dialog), "http://www.tofree.org/");
    gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (dialog), "ToFree 开源社区");
    gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (dialog), authors);
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),"Cugb Freer 是一个地大学生自行编写的集合地大各种网络常用功能于一体的工具箱。");
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}
