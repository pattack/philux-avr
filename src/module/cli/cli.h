//
// Created by pouyan on 6/2/18.
//

#ifndef PHILUXAVR_CLI_H
#define PHILUXAVR_CLI_H

#include <io/io.h>

typedef struct cli cli;

typedef struct command command;

typedef int (*executable)(cli *c, char *argv[]);

struct command {
		const char *name;
		executable exec;
};

struct cli {
		io_reader *r;
		io_writer *w;

		int commands_count;
		command *commands;

		void (*add_handler)(cli *, const char *name, executable fn);

		void (*execute)(cli *);

		int (*puts)(cli *, const char *str);

		int (*gets)(cli *, char *str);
};

extern cli *new_cli(io_reader *, io_writer *);

#endif //PHILUXAVR_CLI_H
