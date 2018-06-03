//
// Created by pouyan on 6/2/18.
//

#include "usart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>

// I/O Buffers
#define USART_BUFFER_SIZE 256
uint8_t ibuf[USART_BUFFER_SIZE], obuf[USART_BUFFER_SIZE];

static void init(uint16_t baud_prescale) {
	// Initialize buffers
	// ibuf = (uint8_t*) malloc(USART_BUFFER_SIZE * sizeof(uint8_t));
	memset(ibuf, '\0', USART_BUFFER_SIZE * sizeof(uint8_t));

	// obuf = (uint8_t*) malloc(USART_BUFFER_SIZE * sizeof(uint8_t));
	memset(obuf, '\0', USART_BUFFER_SIZE * sizeof(uint8_t));

	// Set baud rate
	// Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRH = (uint8_t) (baud_prescale >> 8);
	// Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	UBRRL = (uint8_t) baud_prescale;

	// Receive Complete Interrupt Enable (USART_RXC)
	// Receive Enable
	// Transmit Enable
	UCSRB = (1 << RXCIE) | (1 << RXEN) | (1 << TXEN);

	// Asynchronous (UMSEL = 0)
	// Even parity (UPM1:0 = 0b10)
	// 1 stopbit (USBS = 0)
	// 8 databits (UCSZ1:0 = 0b11)
	UCSRC = (1 << URSEL) | (1 << UPM1) | (1 << UCSZ1) | (1 << UCSZ0);

	// Enable interrupts
	sei();
}

int push(uint8_t *buf, uint8_t b) {
	for (uint8_t i = 0; i < (USART_BUFFER_SIZE - 1); i++) {
		if ('\0' == buf[i]) {
			buf[i] = b;
			buf[i + 1] = '\0';

			return 1;
		}
	}

	return 0;
}

uint8_t shift(uint8_t *buf) {
	if ('\0' == buf[0]) {
		return 0;
	}

	uint8_t b = buf[0];

	// Shift array to Left
	for (uint8_t i = 0; i < (USART_BUFFER_SIZE - 1); i++) {
		buf[i] = buf[i + 1];
	}

	return b;
}

/**
 * Reader
 */

static int read(char *p) {
	// Flush input buffer into p
	uint8_t b = shift(ibuf);

	if (0 == b) {
		return 0;
	}

	if (NULL == p) {
		p = (char*) malloc(sizeof(char));
	}

	*p = (char) b;

	return 1;
}

extern io_reader *new_usart_reader() {
	io_reader *r = malloc(sizeof(io_reader));
	r->read = read;

	init(USART_BAUD_PRESCALE);

	return r;
}

/**
 * Writer
 */

static int write(char b) {
	// push to output buffer
	if (0 == push(obuf, (uint8_t) b)) {
		return 0;
	}

	// Activate 'Data Register Empty Interrupt Enable' (USART_UDRE)
	// in order to wait for a write
	UCSRB |= (1 << UDRIE);

	return 1;
}

extern io_writer *new_usart_writer() {
	io_writer *w = malloc(sizeof(io_writer));
	w->write = write;

	init(USART_BAUD_PRESCALE);

	return w;
}

/**
 * Called when USART completes receiving data
 *
 * It checks if there's no error, and if the character b is received,
 * and pushes into input buffer
 */
ISR(USART_RXC_vect) {
	// Fetch the received byte value to reset flag
	uint8_t b = UDR;

	// Check for error
	if (0 == (UCSRA & ((1 << FE) | (1 << DOR) | (1 << PE)))) {
		// No error occured

		// push to input buffer
		push(ibuf, b);
	}
}

/**
 * Called when the data register accepts new data
 *
 * When this occurs, we can write new data through the USART,
 * and in this case we write the first output buffer value.
 */
ISR(USART_UDRE_vect) {
	// Flush output buffer into UDR
	uint8_t b = shift(obuf);
	if (0 == b) {
		// Disable this interrupt
		UCSRB &= ~(1 << UDRIE);
	} else {
		UDR = (uint8_t) b;
	}
}
