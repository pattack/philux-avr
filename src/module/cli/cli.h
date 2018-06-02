//
// Created by pouyan on 6/2/18.
//

#ifndef PHILUXAVR_CLI_H
#define PHILUXAVR_CLI_H

#include <io/io.h>

typedef struct cli {
		io_reader *r;
		io_writer *w;

		void (*execute)(struct cli *);
} cli;

extern cli *new_cli(io_reader *, io_writer *);

#endif //PHILUXAVR_CLI_H
