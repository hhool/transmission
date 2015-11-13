/*
 * This file Copyright (C) Mnemosyne LLC
 *
 * This file is licensed by the GPL version 2. Works owned by the
 * Transmission project are granted a special exemption to clause 2(b)
 * so that the bulk of its code can remain under the MIT license.
 * This exemption does not extend to derived works not owned by
 * the Transmission project.
 *
 * $Id: hig.c 12963 2011-10-11 04:11:08Z jordan $
 */

#include <gtk/gtk.h>
#include "hig.h"
#include "util.h" /* gtr_hbox_new */

GtkWidget*
hig_workarea_create( void )
{
    GtkWidget * t = gtk_table_new( 1, 2, FALSE );

    gtk_container_set_border_width( GTK_CONTAINER( t ), GUI_PAD_BIG );
    gtk_table_set_col_spacing( GTK_TABLE( t ), 0, GUI_PAD_BIG );
    gtk_table_set_row_spacings( GTK_TABLE( t ), GUI_PAD );
    return t;
}

void
hig_workarea_add_section_divider( GtkWidget * t, guint * row )
{
    GtkWidget * w = gtk_alignment_new( 0.0f, 0.0f, 0.0f, 0.0f );

    gtk_widget_set_size_request( w, 0u, 6u );
    gtk_table_attach( GTK_TABLE( t ), w, 0, 2, *row, *row + 1, 0, 0, 0, 0 );
    ++ * row;
}

void
hig_workarea_add_section_title_widget( GtkWidget * t, guint * row, GtkWidget * w )
{
    gtk_table_attach( GTK_TABLE( t ), w, 0, 2, *row, *row + 1, ~0, 0, 0, 0 );
    ++ * row;
}

void
hig_workarea_add_section_title( GtkWidget *  t, guint * row, const char * section_title )
{
    char buf[512];
    GtkWidget * l;

    g_snprintf( buf, sizeof( buf ), "<b>%s</b>", section_title );
    l = gtk_label_new( buf );
    gtk_misc_set_alignment( GTK_MISC( l ), 0.0f, 0.5f );
    gtk_label_set_use_markup( GTK_LABEL( l ), TRUE );
    hig_workarea_add_section_title_widget( t, row, l );
}

static GtkWidget*
rowNew( GtkWidget * w )
{
    GtkWidget * a;
    GtkWidget * h = gtr_hbox_new( FALSE, 0 );

    /* spacer */
    a = gtk_alignment_new( 0.0f, 0.0f, 0.0f, 0.0f );
    gtk_widget_set_size_request( a, 18u, 0u );
    gtk_box_pack_start( GTK_BOX( h ), a, FALSE, FALSE, 0 );

    /* lhs widget */
    if( GTK_IS_MISC( w ) )
        gtk_misc_set_alignment( GTK_MISC( w ), 0.0f, 0.5f );
    if( GTK_IS_LABEL( w ) )
        gtk_label_set_use_markup( GTK_LABEL( w ), TRUE );
    gtk_box_pack_start( GTK_BOX( h ), w, TRUE, TRUE, 0 );

    return h;
}

void
hig_workarea_add_wide_control( GtkWidget * t, guint * row, GtkWidget * w )
{
    GtkWidget * r = rowNew( w );

    gtk_table_attach( GTK_TABLE( t ), r, 0, 2, *row, *row + 1, GTK_FILL, 0, 0, 0 );
    ++ * row;
}

void
hig_workarea_add_wide_tall_control( GtkWidget * t, guint * row, GtkWidget * w )
{
    GtkWidget * r = rowNew( w );

    gtk_table_attach( GTK_TABLE( t ), r, 0, 2, *row, *row + 1,
                      GTK_EXPAND | GTK_SHRINK | GTK_FILL,
                      GTK_EXPAND | GTK_SHRINK | GTK_FILL,
                      0, 0 );

    ++ * row;
}

GtkWidget *
hig_workarea_add_wide_checkbutton( GtkWidget  * t,
                                   guint      * row,
                                   const char * mnemonic_string,
                                   gboolean     is_active )
{
    GtkWidget * w = gtk_check_button_new_with_mnemonic( mnemonic_string );

    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( w ), is_active );
    hig_workarea_add_wide_control( t, row, w );
    return w;
}

void
hig_workarea_add_label_w( GtkWidget * t, guint row, GtkWidget * l )
{
    GtkWidget * w = rowNew( l );

    gtk_table_attach( GTK_TABLE( t ), w, 0, 1, row, row + 1, GTK_FILL, GTK_FILL, 0, 0 );
}

GtkWidget*
hig_workarea_add_label( GtkWidget * t, guint row, const char * mnemonic_string )
{
    GtkWidget * l = gtk_label_new_with_mnemonic( mnemonic_string );

    hig_workarea_add_label_w( t, row, l );
    return l;
}

static void
hig_workarea_add_tall_control( GtkWidget * t, guint row, GtkWidget * control )
{
    if( GTK_IS_MISC( control ) )
        gtk_misc_set_alignment( GTK_MISC( control ), 0.0f, 0.5f );

    gtk_table_attach( GTK_TABLE( t ), control,
                      1, 2, row, row + 1,
                      GTK_EXPAND | GTK_SHRINK | GTK_FILL,
                      GTK_EXPAND | GTK_SHRINK | GTK_FILL,
                      0, 0 );
}

static void
hig_workarea_add_control( GtkWidget * t, guint row, GtkWidget * control )
{
    if( GTK_IS_MISC( control ) )
        gtk_misc_set_alignment( GTK_MISC( control ), 0.0f, 0.5f );

    gtk_table_attach( GTK_TABLE( t ), control,
                      1, 2, row, row + 1,
                      GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0, 0 );
}

void
hig_workarea_add_row_w( GtkWidget * t,
                        guint     * row,
                        GtkWidget * label,
                        GtkWidget * control,
                        GtkWidget * mnemonic )
{
    hig_workarea_add_label_w( t, *row, label );
    hig_workarea_add_control( t, *row, control );
    if( GTK_IS_LABEL( label ) )
        gtk_label_set_mnemonic_widget( GTK_LABEL( label ),
                                       mnemonic ? mnemonic : control );
    ++ * row;
}

GtkWidget*
hig_workarea_add_row( GtkWidget  * t,
                      guint      * row,
                      const char * mnemonic_string,
                      GtkWidget  * control,
                      GtkWidget  * mnemonic )
{
    GtkWidget * l = gtk_label_new_with_mnemonic( mnemonic_string );

    hig_workarea_add_row_w( t, row, l, control, mnemonic );
    return l;
}

GtkWidget*
hig_workarea_add_tall_row( GtkWidget  * table,
                           guint      * row,
                           const char * mnemonic_string,
                           GtkWidget  * control,
                           GtkWidget  * mnemonic )
{
    GtkWidget * l = gtk_label_new_with_mnemonic( mnemonic_string );
    GtkWidget * h = gtr_hbox_new( FALSE, 0 );
    GtkWidget * v = gtr_vbox_new( FALSE, 0 );
    gtk_box_pack_start( GTK_BOX( h ), l, FALSE, FALSE, 0 );
    gtk_box_pack_start( GTK_BOX( v ), h, FALSE, FALSE, GUI_PAD_SMALL );

    hig_workarea_add_label_w( table, *row, v );
    hig_workarea_add_tall_control( table, *row, control );

    if( GTK_IS_LABEL( l ) )
        gtk_label_set_mnemonic_widget( GTK_LABEL( l ),
                                       mnemonic ? mnemonic : control );

    ++ * row;
    return l;
}

void
hig_workarea_finish( GtkWidget * t, guint * row )
{
    gtk_table_resize( GTK_TABLE( t ), *row, 2 );
}
