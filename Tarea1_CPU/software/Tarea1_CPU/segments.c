#include "segments.h"
/*
 * Funcion: display_segments.
 * Entradas: num.
 * Salidas: result.
 * Este metodo se encarga decodificar el numero decimal ingresado
 * en un numero hexadecimal con la representacion en 7 segmentos.
 */
uint8_t display_segments(int num)
{
	uint8_t result = 0;

	if (num == 0){result = 0x40;}
	else if (num == 1){result = 0x79;}
	else if (num == 2){result = 0x24;}
	else if (num == 3){result = 0x30;}
	else if (num == 4){result = 0x19;}
	else if (num == 5){result = 0x12;}
	else if (num == 6){result = 0x02;}
	else if (num == 7){result = 0x78;}
	else if (num == 8){result = 0x00;}
	else if (num == 9){result = 0x10;}

	return result;
}
