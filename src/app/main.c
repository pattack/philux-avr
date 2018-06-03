//
// Created by pouyan on 6/2/18.
//

#include <stdarg.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/wdt.h>

#include <usart/usart.h>
#include <cli/cli.h>

// Commands
int restart(cli* c, char *argv[]);
int dim(cli* c, char *argv[]);

int main() {
	cli *c = new_cli(new_usart_reader(), new_usart_writer());

	// Add handlers
	c->add_handler(c, "restart", restart);
	c->add_handler(c, "dim", dim);
	c->add_handler(c, "reboot", restart);

	// Run CLI
	c->execute(c);

	return 0;
}

int restart(cli* c, char *argv[]) {
	c->puts(c, "Rebooting...\r\n");

	wdt_enable(WDTO_15MS);
	for (;;);
}

int dim(cli* c, char *argv[]) {
	c->puts(c, "Starting dimmer...\r\n");

	char buf[64];
	sprintf(buf, "Args: %s %s\r\n", argv[0], argv[1]);
	c->puts(c, buf);

	return 0;
}
