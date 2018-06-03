//
// Created by pouyan on 6/2/18.
//

#ifndef PHILUXAVR_CLI_H
#define PHILUXAVR_CLI_H

#include <io/io.h>

typedef struct cli cli;

typedef int (*command)(cli *c, const char *name, ...);

struct cli {
		io_reader *r;
		io_writer *w;

		const char** cmd_names;
		command* handlers;

		void (*add_handler)(cli *, const char *name, command fn);

		void (*execute)(cli *);

		int (*puts)(cli *, char *str);

		int (*gets)(cli *, char *str);
};

extern cli *new_cli(io_reader *, io_writer *);

#endif //PHILUXAVR_CLI_H
