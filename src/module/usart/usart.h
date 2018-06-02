//
// Created by pouyan on 6/2/18.
//

#ifndef PHILUXAVR_USART_H
#define PHILUXAVR_USART_H

#ifndef F_CPU
#  error "usart.h requires F_CPU to be defined"
#endif

#ifndef USART_BAUDRATE
#  error "usart.h requires USART_BAUDRATE to be defined"
#endif

#define USART_BAUD_PRESCALE ((F_CPU / (USART_BAUDRATE * 16UL)) - 1)

#include <io/io.h>

extern io_reader *new_usart_reader();

extern io_writer *new_usart_writer();

#endif //PHILUXAVR_USART_H
