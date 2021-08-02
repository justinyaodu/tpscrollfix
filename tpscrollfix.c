#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Convert integer macros to strings: https://stackoverflow.com/a/5459929
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define NAME "tpscrollfix"
#define VERSION "0.1.0"

#define debug_printf(...) \
	if (debug_level >= 1) { printf(NAME ": " __VA_ARGS__); }

#define DEFAULT_BUTTON 10
#define TRACKPOINT_UP 4
#define TRACKPOINT_DOWN 5
#define DEFAULT_COMMAND "xdotool click 2"
#define DEFAULT_DEBUG_LEVEL 0

unsigned short int scroll_button = DEFAULT_BUTTON;
unsigned short int trackpoint_up = TRACKPOINT_UP;
unsigned short int trackpoint_down = TRACKPOINT_DOWN;
char *click_command = DEFAULT_COMMAND;
unsigned short int debug_level = DEFAULT_DEBUG_LEVEL;

void loop()
{
	debug_printf("watching button %d\n", scroll_button);

    bool send_click = false;
    bool button_pressed = false;
    bool moving_trackpoint = false;

	size_t line_size = 64;
	char* line = malloc(line_size);

	while (getline(&line, &line_size, stdin) != -1)
	{
        int axis;
        int button;

        // to do not debug the trackpoint motion
        if (debug_level >= 2 && sscanf(line, "motion a[%d]", &axis) != 1) {
            fprintf(stderr, "\t%s", line);
        }

        if (sscanf(line, "button press %d", &button) == 1 && button == scroll_button) {
            moving_trackpoint = false;
            button_pressed = true;

            debug_printf("button %d pressed\n", button);
        }

        if (sscanf(line, "button press %d", &button) == 1 && button_pressed &&
            (button == trackpoint_up || button == trackpoint_down)
            ) {
            moving_trackpoint = true;

            debug_printf("scrolling with %d\n", button);
        }

        if (sscanf(line, "button release %d", &button) == 1 && button == scroll_button && !moving_trackpoint) {
            send_click = true;
            button_pressed = false;

            debug_printf("button %d released\n", button);
        }

        if (send_click) {
            send_click = false;

            system(click_command);
        }
	}

	free(line);
}

void usage()
{
	fprintf(stderr,
		"usage:\n" 

		"  " NAME " [-b BUTTON] [-c COMMAND] [-d LEVEL]\n"
		"  " NAME " -h | --help | -v | --version\n"

		"options:\n"

		"  -b BUTTON, --button BUTTON     "
		"button code sent by the remapped middle button"
		" [default: " STR(DEFAULT_BUTTON) "]\n"

		"  -c COMMAND, --command COMMAND  "
		"command used to send a middle click"
		" [default: " DEFAULT_COMMAND "]\n"

		"  -d LEVEL, --debug LEVEL        "
		"0: silent, 1: minimal logs, 2: verbose logs"
		" [default: " STR(DEFAULT_DEBUG_LEVEL) "]\n"

		"  -h, --help                     "
		"show this usage message\n"

		"  -v, --version                  "
		"show version information\n"

		"example:\n"

		"  xinput set-button-map 'Lenovo TrackPoint Keyboard II Mouse' 1 10 3 4 5 6 7\n"
		"  xinput test 'Lenovo TrackPoint Keyboard II Mouse' | " NAME "\n"
	);
}

void version()
{
	fprintf(stderr,
		NAME " " VERSION "\n"
		"Copyright (C) 2021 Justin Yao Du.\n"
		"Licensed under the MIT License.\n"
		"https://github.com/justinyaodu/tpscrollfix\n"
	);
}

int main(int argc, char** argv)
{
	int opt;
	while ((opt = getopt(argc, argv, "b:c:d:hv")) != -1)
	{
		switch (opt)
		{
			case 'b':
				scroll_button = atoi(optarg);
				break;
			case 'c':
				click_command = optarg;
				break;
			case 'd':
				debug_level = atoi(optarg);
				break;
			case 'h':
				usage();
				return 0;
			case 'v':
				version();
				return 0;
			default:
				usage();
				return 1;
		}
	}

	loop();

	return 0;
}
