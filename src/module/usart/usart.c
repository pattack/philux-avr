//
// Created by pouyan on 6/2/18.
//

#include "usart.h"

#include <stdio.h>
#include <stdlib.h>

static int read(char* p) {
    return 0;
}

static int write(char* p) {
    return 0;
}

extern io_reader* new_usart_reader() {
    io_reader* r = malloc(sizeof(io_reader));
    r->read = read;

    return r;
}

extern io_writer* new_usart_writer() {
    io_writer* w = malloc(sizeof(io_writer));
    w->write = write;

    return w;
}
