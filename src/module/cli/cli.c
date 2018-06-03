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
	char buf[300] = "";

	// TODO: Dispatch command
	{
		sprintf(buf, "command not found: %s\r\n", command);
	}

	// Write results
	cli_puts(c, buf);
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

static void cli_add_handler(cli *c, const char *name, command fn) {
	static int count;

	count++;

	c->handlers = (command*) realloc(c->handlers, count * sizeof(command));
	c->handlers[count - 1] = fn;

	c->cmd_names = (const char**) realloc(c->cmd_names, count * sizeof(const char*));
	for (int i = 0; i < count; i++) {
		// Last empty home or current home
		if (count == (i + 1) || strcmp(c->cmd_names[i], name) > 0) {
			for (int j = count - 1; j > i; j--) {
				c->cmd_names[j] = c->cmd_names[j - 1];
			}

			c->cmd_names[i] = name;

			break;
		}
	}
}

extern cli *new_cli(io_reader *r, io_writer *w) {
	cli *c = (cli*) malloc(sizeof(cli));
	c->w = w;
	c->r = r;
	c->add_handler = cli_add_handler;
	c->execute = cli_execute;
	c->puts = cli_puts;
	c->gets = cli_gets;

	return c;
}
