//
// Created by pouyan on 6/2/18.
//

#ifndef PHILUXAVR_IO_H
#define PHILUXAVR_IO_H

typedef struct {
		int (*read)(char *b);
} io_reader;

typedef struct {
		int (*write)(char b);
} io_writer;

#endif //PHILUXAVR_IO_H
