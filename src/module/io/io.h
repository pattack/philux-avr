//
// Created by pouyan on 6/2/18.
//

#ifndef PHILUXAVR_IO_H
#define PHILUXAVR_IO_H

typedef struct {
    int (*read)(char* p);
} io_reader;

typedef struct {
    int (*write)(char* p);
} io_writer;

#endif //PHILUXAVR_IO_H
