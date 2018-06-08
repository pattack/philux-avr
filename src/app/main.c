//
// Created by pouyan on 6/2/18.
//

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include <usart/usart.h>
#include <cli/cli.h>

// Commands
int restart(cli *c, char *argv[]);

int dim(cli *c, char *argv[]);

int help(cli *c, char *argv[]);

cli *debug;

int main() {
	cli *c = new_cli(new_usart_reader(), new_usart_writer());
	debug = c;

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

uint8_t timer0_cs0 = (1 << CS02) | (1 << CS00);

int dim(cli *c, char *argv[]) {
	uint8_t angle = (uint8_t) (strtol(argv[1], (char **) NULL, 10) % 180);
	double time_shift_ms = angle / 18.0;

	timer0_cs0 = (1 << CS02) | (1 << CS00);
	uint16_t prescaler = 1024;
	if (angle < 5) {
		timer0_cs0 = (1 << CS01);
		prescaler = 8;
	} else if (angle < 37) {
		timer0_cs0 = (1 << CS01) | (1 << CS00);
		prescaler = 64;
	} else if (angle < 147) {
		timer0_cs0 = (1 << CS02);
		prescaler = 256;
	}

	uint8_t timer_ceil = (uint8_t) (time_shift_ms * ((F_CPU / prescaler) / 1000.0));

	char buf[255];
	sprintf(buf, "Configure dimmer angle on: %d%lc @ %luHz: ~%dms --> %d timer ticks (CLK/%d)\r\n", angle, 0xb0, F_CPU, (int) time_shift_ms, timer_ceil, prescaler);
	c->puts(c, buf);

	// Setup Trigger Signaller
	DDRC |= (1 << DDC0); // PortC.0: output
	PORTC &= ~(1 << PORTC0); // PortC.0 = 0

	// Setup Timer
	TCCR0 = (1 << WGM01); // Disable Timer0 and Set for CTC Mode
	TCNT0 = 0; // Reset Timer0 Value
	OCR0 = timer_ceil; // Set Timer0 Max Value
	TIMSK |= (1 << TOIE0); // Enable Timer0 overflow interrupt

	// External Interrupt Setup
	DDRD &= ~(1 << DDC2); // Set PinC.2 as Input
	MCUCR &= ~((1 << ISC01) | (1 << ISC00)); // Enable INT0 on Low Level
	GICR |= (1 << INT0); // Enable INT0

	// enable global interrupts
	sei();

	return 0;
}

int help(cli *c, char *argv[]) {
	c->puts(c, "This is PhiLUX\r\n");
	c->puts(c, "Available commands:\r\n");
	c->puts(c, "- dim angle\r\n");
	c->puts(c, "- reboot\r\n");

	return 0;
}

ISR(INT0_vect) {
	// Enable Timer0 with Prescaler
	TCCR0 |= timer0_cs0;

	// PortC.0 = 1
	PORTC |= (1 << PORTC0);
}

ISR(TIMER0_OVF_vect) {
	// Disable Timer: CS00:2 = 0
	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));

	// PortC.0 = 0
	PORTC &= ~(1 << PORTC0);
}
