//
// Created by pouyan on 6/2/18.
//

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <usart/usart.h>
#include <cli/cli.h>

// Commands
int restart(cli *c, char *argv[]);

int dim(cli *c, char *argv[]);

int help(cli *c, char *argv[]);

int main() {
	cli *c = new_cli(new_usart_reader(), new_usart_writer());

	// Add handlers
	c->add_handler(c, "restart", restart);
	c->add_handler(c, "dim", dim);
	c->add_handler(c, "reboot", restart);
	c->add_handler(c, "help", help);

	// Run CLI
	c->execute(c);

	return 0;
}

int restart(cli *c, char *argv[]) {
	c->puts(c, "Rebooting...\r\n");

	wdt_enable(WDTO_15MS);
	for (;;);
}

int dim(cli *c, char *argv[]) {
	c->puts(c, "Starting dimmer...\r\n");

	int angle = (int) strtol(argv[1], (char **) NULL, 10);
	double skip = angle * (10e-3 / 180);

	char buf[255];
	sprintf(buf, "Configure dimmer angle on: %d%lc @ %luHz: %fms\r\n", angle, 0xb0, F_CPU, skip);
	c->puts(c, buf);

	// PortC.0: output
	DDRC |= 1;

	// PortC.0 = 0
	PORTC &= 254;

	uint16_t prescaler = 1024;
	OCR0 = (uint8_t) (skip * (F_CPU / prescaler));

	// CTC Mode
	TCCR0 = (1 << WGM01);

	// enable timer0 overflow interrupt
	TIMSK |= (1 << TOIE0);

	// Enable INT0
	DDRD &= 251; // Set PortC.2 as Input
	MCUCR = (1 << ISC01) | (1 << ISC00); // Enable INT0 in Raising edge
	GICR |= (1 << INT0);

	// enable global interrupts
	sei();

	return 0;
}

int help(cli *c, char *argv[]) {
	c->puts(c, "This is PhiLUX\r\n");

	return 0;
}

ISR(INT0_vect) {
	// Enable Timer0 with 1024 Prescaler
	TCCR0 |= (1 << CS02) | (1 << CS00);
}

ISR(TIMER0_OVF_vect) {
	// Disable Timer: CS00:2 = 0
	TCCR0 &= 248;

	PORTC |= 1;
	_delay_us(500);
	PORTC &= 0;
}
