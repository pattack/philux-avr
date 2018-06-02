//
// Created by pouyan on 6/2/18.
//

#include "cli.h"

#include <stdlib.h>

static void execute(struct cli *c) {
	for (;;) {
	}
}

extern cli *new_cli(io_reader *r, io_writer *w) {
	cli *c = malloc(sizeof(cli));
	c->w = w;
	c->r = r;
	c->execute = execute;

	return c;
}
