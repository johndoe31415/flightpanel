#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <surface.h>
#include "gtk_helper.h"

struct app_data_t {
	const struct surface_t *surfaces;
	int surface_count;
};

static gboolean window_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data) {
	if (event->keyval == GDK_KEY_Escape) {
		GApplication *app = (GApplication*)data;
		g_application_quit(app);
		return true;
	}
	return false;
}

static void activate(GtkApplication *app, gpointer user_data) {
	struct app_data_t *appdata = (struct app_data_t*)user_data;

	GtkWidget *window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Surface");
//	gtk_window_set_default_size(GTK_WINDOW(window), surface->width, surface->height);

	g_signal_connect(window, "key_press_event", G_CALLBACK(window_keypress), app);

	const int spacing_height = 10;
	int total_height = (appdata->surface_count - 1) * spacing_height;
	int total_width = 0;
	for (int i = 0; i < appdata->surface_count; i++) {
		if (appdata->surfaces[i].width > total_width) {
			total_width = appdata->surfaces[i].width;
		}
		total_height += appdata->surfaces[i].height;
	}

	guchar *data = malloc(3 * total_width * total_height);
	memset(data, 0xaa, 3 * total_width * total_height);

	int base_y = 0;
	for (int i = 0; i < appdata->surface_count; i++) {
		for (int x = 0; x < appdata->surfaces[i].width; x++) {
			for (int y = 0; y < appdata->surfaces[i].height; y++) {
				const int offset = 3 * ((total_width * (base_y + y) + x));
				bool pixel = surface_getpixel(&appdata->surfaces[i], x, y);
				data[offset + 0] = pixel ? 0xff : 0x00;
				data[offset + 1] = pixel ? 0xff : 0x00;
				data[offset + 2] = pixel ? 0xff : 0x00;
			}
		}
		base_y += appdata->surfaces[i].height + spacing_height;
	}
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(data, GDK_COLORSPACE_RGB, false, 8, total_width, total_height, 3 * total_width, NULL, NULL);

	GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_container_add(GTK_CONTAINER(window), image);

	gtk_widget_show_all(window);
}

int start_gtk_application(const struct surface_t *surfaces, int surface_count) {
	char *argv[] = {
		"gtkapp",
		NULL,
	};
	struct app_data_t appdata = {
		.surfaces = surfaces,
		.surface_count = surface_count,
	};

	GtkApplication *app = gtk_application_new("org.gtk.fp", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), &appdata);
	int status = g_application_run(G_APPLICATION(app), 1, argv);
	g_object_unref(app);

	return status;
}

#ifdef DEBUG_GTK
int main(int argc, char **argv) {
	struct surface_t surfaces[] = {
		{
			.width = 128,
			.height = 64,
			.data = (uint8_t[128 * 64 / 8]) { },
		},
	};
	return start_gtk_application(surfaces, 1);
}
#endif
