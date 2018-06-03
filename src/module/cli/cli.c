//
// Created by pouyan on 6/2/18.
//

#include "cli.h"
#include <io/io.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <util/delay.h>

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

static void execute(struct cli *c) {
	for (;;) {
		cli_puts(c, "$ ");

		// Read command
		char command[256] = "";
		cli_gets(c, command);

		// TODO: Dispatch command

		// Write results
		char buf[300] = "";
		sprintf(buf, "Command: %s\r\n", command);
		cli_puts(c, buf);
	}
}

extern cli *new_cli(io_reader *r, io_writer *w) {
	cli *c = malloc(sizeof(cli));
	c->w = w;
	c->r = r;
	c->execute = execute;

	return c;
}
