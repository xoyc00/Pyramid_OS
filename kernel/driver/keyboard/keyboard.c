#include <kernel/driver/keyboard.h>
#include <kernel/cpu/ports.h>
#include <kernel/cpu/idt.h>
#include <kernel/tty.h>
#include <kernel/driver/vga/vga.h>
#include <kernel/shell.h>
#include <stdio.h>
#include <string.h>

#define ESCAPE 0x01
#define BACKSPACE 0x0e
#define ENTER 0x1c
#define SHIFT_PRESS 0x2a
#define SHIFT_RELEASE 0xaa
#define CAPS 0xba

#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80

static char key_buffer[512];

int shifted = 0;
int caps = 0;

extern int vga_drvr_enabled;
extern int program_executing;

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii_caps[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '\n', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};
const char sc_ascii_shift[] = { '?', '?', '!','@', '#', '$', '%', '^',     
    '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '{', '}', '\n', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ':', '\"', '~', '?', '|', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};
const char sc_ascii_std[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 
        'u', 'i', 'o', 'p', '[', ']', '\n', '?', 'a', 's', 'd', 'f', 'g', 
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};


/* Processes keyboard input. Called whenever the keybaord interrupt is recieved. */
void keyboard_callback() {
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = inb(0x60);
	if (scancode == ESCAPE) {
		program_executing = 0;
		return;
	}
	if (scancode == BACKSPACE) {
		backspace(key_buffer);
		if (vga_drvr_enabled) {
			vga_terminal_backspace();
		} else {
			terminal_backspace();
		}
	} else if (scancode == ENTER) {
		printf("\n");
		process_input(key_buffer, 1);
		key_buffer[0] = '\0';
	} else if (scancode == SHIFT_PRESS) {
		shifted = 1;
	} else if (scancode == SHIFT_RELEASE) {
		shifted = 0;
	} else if (scancode == CAPS) {
		caps = !caps;
	} else if (scancode == KEY_UP) {
		if (vga_drvr_enabled) {
			vga_terminal_u();
		} else {
			terminal_u();
		}
	} else if (scancode == KEY_DOWN) {
		if (vga_drvr_enabled) {
			vga_terminal_d();
		} else {
			terminal_d();
		}
	} else if (scancode == KEY_LEFT) {
		if (vga_drvr_enabled) {
			vga_terminal_l();
		} else {
			terminal_l();
		}
	} else if (scancode == KEY_RIGHT) {
		if (vga_drvr_enabled) {
			vga_terminal_r();
		} else {
			terminal_r();
		}
	} else {
		if (scancode > SC_MAX) return;
    	
		char c; 

		if (shifted && caps)
			c = sc_ascii_std[(int)scancode];
		else if (shifted)
			c = sc_ascii_shift[(int)scancode];
		else if (caps)
			c = sc_ascii_caps[(int)scancode];
		else
			c = sc_ascii_std[(int)scancode];

		printf("%c", c);
		append(key_buffer, c);
	}
}
