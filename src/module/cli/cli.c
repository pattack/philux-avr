//
// Created by pouyan on 6/2/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"
#include <io/io.h>

static int cli_puts(cli *c, char *str) {
	int count;

	for (count = 0; '\0' != str[count]; count++) {
		if (0 == c->w->write(str[count])) {
			break;
		}
	}

	return count;
}

static int cli_gets(cli *c, char *str) {
	int count = 0;

	char b;
	for (char finished = 0; 0 == finished;) {
		if (1 == c->r->read(&b)) {
			char echo = 1;

			switch (b) {
				case 3: // ^C
					strcpy(str, "");
							b = '\r';
							finished = 1;
							break;

				case 4: // ^D
					break;

				case '\r':
				case '\n':
					finished = 1;
							break;

				case '\t':
					// TODO: Print help
					break;

				case '\b':
					if (0 == count) {
						echo = 0;
					} else {
						str[--count] = '\0';
					}
							break;

				default:
					if (b > 31) {
						str[count++] = b;
						str[count] = '\0';
					}
							break;
			}

			if (1 == echo) {
				c->w->write(b);
			}
		}
	}

	return count;
}

static void cli_handle(cli *c, const char *command) {
	// Parse command
	int count = 0;
	char *argv[25];
	char *cmd = strdup(command);
	while (NULL != (argv[count++] = strsep(&cmd, " ")));

	if (count > 0) {
		for (int i = 0; i < c->commands_count; i++) {
			if (0 == strcmp(c->commands[i].name, argv[0])) {
				c->commands[i].exec(c, argv);

				// TODO: Read status

				return;
			}
		}

		char buf[128] = "";
		sprintf(buf, "command not found: %s\r\n", command);
		cli_puts(c, buf);
	}
}

static void cli_execute(cli *c) {
	for (;;) {
		cli_puts(c, "# ");

		// Read command
		char cmd[256] = "";
		cli_gets(c, cmd);

		// Handle command
		cli_handle(c, cmd);
	}
}

static void cli_add_handler(cli *c, const char *name, executable fn) {
	command cmd = {
		.name = name,
		.exec = fn,
	};

	c->commands_count++;
	c->commands = (command *) realloc(c->commands, c->commands_count * sizeof(command));
	for (int i = 0; i < c->commands_count; i++) {
		// Last empty home or current home
		if (c->commands_count == (i + 1) || strcmp(c->commands[i].name, name) > 0) {
			for (int j = c->commands_count - 1; j > i; j--) {
				c->commands[j] = c->commands[j - 1];
			}

			c->commands[i] = cmd;

			break;
		}
	}
}

extern cli *new_cli(io_reader *r, io_writer *w) {
	cli *c = (cli *) malloc(sizeof(cli));
	c->w = w;
	c->r = r;
	c->add_handler = cli_add_handler;
	c->execute = cli_execute;
	c->puts = cli_puts;
	c->gets = cli_gets;
	c->commands_count = 0;

	return c;
}
