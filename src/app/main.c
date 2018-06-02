//
// Created by pouyan on 6/2/18.
//

#include <usart/usart.h>
#include <cli/cli.h>

int main() {
	cli *c = new_cli(new_usart_reader(), new_usart_writer());

	// Main loop
	c->execute(c);

	return 0;
}
