#include <kernel/system/window_manage.h>

#include <kernel/driver/vga/vga.h>

#include <stdlib.h>

window_t* m_windows = 0;
int window_count = 0;

void quick_sort_windows(window_t* low, window_t* high) {
	
}

void process_left_mouse(int x, int y) {
	
}

void wm_mouse_button_down(int button, int x, int y) {
	switch(button) {
	case 0:
		process_left_mouse(x, y);
		break;
	default:
		break;
	}
}

void wm_mouse_button_up(int button, int x, int y) {
	// TODO: implement mouse buttons
}

void wm_init() {
	window_t* term = window_create(512, 384, "Terminal");
	term->bg_r = 32;
	term->bg_g = 32;
	term->bg_b = 32;
	window_register(term);
}

void wm_draw() {
	for (int i = 0; i < window_count; i++) {
		vga_drawwindow(m_windows[i]);
	}
}

void window_register(window_t* w) {
	if (window_count == 0) {
		m_windows = malloc(sizeof(window_t));
		m_windows[0] = *w;
		window_count++;
	} else {
		window_count++;
		window_t* new_m_windows = krealloc(m_windows, window_count * sizeof(window_t));
		memcpy(new_m_windows, m_windows, (window_count-1)*sizeof(window_t));
		new_m_windows[window_count] = *w;
		m_windows = new_m_windows;
	}
}

void window_deregister(window_t* w) {
	if (window_count == 0) {
		return;
	} else {
		
	}
}

window_t* window_create(int w, int h, char* title) {
	window_t* out = malloc(sizeof(window_t));
	out->x = 100;
	out->y = 100;
	out->w = w;
	out->h = h;
	out->tb_h = 18;
	out->rounded = 1;
	out->bg_r = 255;
	out->bg_r = 128;
	out->bg_r = 128;
	out->title = title;
	return out;
}
