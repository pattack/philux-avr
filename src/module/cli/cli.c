//
// Created by pouyan on 6/2/18.
//

#include "cli.h"
#include <io/io.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <util/delay.h>

static int cli_puts(io_writer *w, char *str) {
	int count;

	for (count = 0; '\0' != str[count]; count++) {
		if (0 == w->write(str[count])) {
			break;
		}
	}

	return count;
}

static int cli_gets(io_reader *r, char **str) {
	int count = 0;

	char b;
	while (1 == r->read(&b)) {
		if ('\n' == b) {
			break;
		} else {
			*str[count++] = b;
		}
	}

	return count;
}

static void execute(struct cli *c) {
	char *command = (char *) malloc(256 * sizeof(char));
	char *buf = (char *) malloc(512 * sizeof(char));

	for (;;) {
		cli_puts(c->w, "$ ");

		_delay_ms(1000);
	}

	cli_gets(c->r, &command);
	sprintf(buf, "Command: %s\n", command);
	cli_puts(c->w, buf);
}

extern cli *new_cli(io_reader *r, io_writer *w) {
	cli *c = malloc(sizeof(cli));
	c->w = w;
	c->r = r;
	c->execute = execute;

	return c;
}
