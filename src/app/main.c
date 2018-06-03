//
// Created by pouyan on 6/2/18.
//

#include <stdarg.h>

#include <avr/io.h>
#include <avr/wdt.h>

#include <usart/usart.h>
#include <cli/cli.h>

// Commands
int restart(cli* c, const char* name, ...);
int dim(cli* c, const char* name, ...);

int main() {
	cli *c = new_cli(new_usart_reader(), new_usart_writer());

	// Add handlers
	c->add_handler(c, "restart", restart);
	c->add_handler(c, "dim", dim);
	c->add_handler(c, "aim", dim);
	c->add_handler(c, "reboot", restart);
	c->add_handler(c, "bimax", restart);
	c->add_handler(c, "zootopia", restart);
	c->add_handler(c, "climax", restart);

	// Run CLI
	c->execute(c);

	return 0;
}

int restart(cli* c, const char* name, ...) {
	c->puts(c, "Rebooting...");

	wdt_enable(WDTO_15MS);
	for (;;);
}

int dim(cli* c, const char* name, ...) {
	c->puts(c, "Starting dimmer...");

	return 0;
}
