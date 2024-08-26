
#include <stdio.h>
#include <system.h>
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_timer_regs.h>
#include "sys/alt_irq.h"
#include "segments.h"

int seconds = 0;
int minutes_0 = 0;
int minutes_1 = 0;
int hour_0 = 0;
int hour_1 = 0;
unsigned int Segments_0 = 0;
unsigned int Segments_1 = 0;
unsigned int Segments_2 = 0;
unsigned int Segments_3 = 0;

int alarm_0 = 0;
int alarm_1 = 0;
int alarm_2 = 1;
int alarm_3 = 1;
volatile int* fallof_ptr;
int switches = 0;
int alarm_flag = 0;

void init_timer_interrupt(void);
void timer_isr(void * context, alt_u32 id);
void update_time(void);
void sound_alarm();
static void handle_btn_pressed(void* context, alt_u32 id);
static void init_btn_pio();
static void set_alarm();
static void set_time();

/*
 * Funcion: init_timer_interrupt.
 * Entradas: void.
 * Salidas: No.
 * Este metodo se encarga de registrar las interrupciones realizadas por el temporizador.
 */
void init_timer_interrupt(void)
{
	alt_ic_isr_register(TIMER_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_IRQ, (void *)timer_isr, NULL, 0x0);

	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, ALTERA_AVALON_TIMER_CONTROL_START_MSK
													| ALTERA_AVALON_TIMER_CONTROL_ITO_MSK
													| ALTERA_AVALON_TIMER_CONTROL_CONT_MSK);
}
/*
 * Funcion: timer_isr.
 * Entradas: contex, id.
 * Salidas: No.
 * Este metodo se encarga de limpiar el estado de la interrupcion realizada
 * y aumentar el temporizador para el reloj.
 */
void timer_isr(void * context, alt_u32 id)
{
	//Limpiar el estado de la interrupcion
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
	update_time();
	seconds++;
}
/*
 * Funcion: handle_btn_pressed.
 * Entradas: contex, id.
 * Salidas: No.
 * Este metodo se encarga de limpiar el estado de la interrupcion realizada
 * y efectuar la operacion de acuerdo con los botones de la interrupcion (establecer alarma u hora).
 */
static void handle_btn_pressed(void* context, alt_u32 id)
{
	*fallof_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BTN_BASE);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BTN_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BTN_BASE, 0xf);

	switches = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);
	if ((switches == 1) | (switches == 9)){//Establecer alarma
		alarm_flag = 1;
		set_alarm();
	}
	else if ((switches == 2) | (switches == 10)){//Establecer hora
		set_time();
	}
	else{
		alarm_flag = 0;
	}
}
/*
 * Funcion: set_alarm.
 * Entradas: No.
 * Salidas: No.
 * Este metodo se encarga de configurar los minutos y la hora de la alarma.
 */
static void set_alarm()
{
	if (switches == 9){ //Configurando horas
		if(*fallof_ptr == 1){alarm_2++;}
		else if ((*fallof_ptr == 2) & (alarm_2 >= 0)) {alarm_2--;}

		if (alarm_2 > 9){
			alarm_3++;
			alarm_2 = 0;
		}
		if ((alarm_3 == 2) & (alarm_2 == 4)){
			alarm_3 = 0;
			alarm_2 = 0;
		}
	}
	else if (switches == 1) {//Configurando minutos
		if(*fallof_ptr == 1){alarm_0++;}
		else if ((*fallof_ptr == 2) & (alarm_0 >= 0)) {alarm_0--;}

		if (alarm_0 > 9){
			alarm_1++;
			alarm_0 = 0;
		}
		if (alarm_1 == 6){
			alarm_1 = 0;
			alarm_0 = 0;
		}
	}
	//Visuales en 7 segmentos.
	Segments_0 = display_segments(alarm_0);
	Segments_1 = display_segments(alarm_1);
	Segments_2 = display_segments(alarm_2);
	Segments_3 = display_segments(alarm_3);

	IOWR_ALTERA_AVALON_PIO_DATA(SEG_0_BASE, Segments_0);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG_1_BASE, Segments_1);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG_2_BASE, Segments_2);
	IOWR_ALTERA_AVALON_PIO_DATA(SEG_3_BASE, Segments_3);
}
/*
 * Funcion: set_time.
 * Entradas: No.
 * Salidas: No.
 * Este metodo se encarga de configurar los minutos y la hora del reloj.
 */
static void set_time()
{
	seconds = 0;
	if (switches == 10){ //Configurando horas
		if(*fallof_ptr == 1){hour_0++;}
		else if ((*fallof_ptr == 2) & (hour_0 >= 0)) {hour_0--;}

		if (hour_0 > 9){
			hour_1++;
			hour_0 = 0;
		}
		if ((hour_1 == 2) & (hour_0 == 4)){
			hour_1 = 0;
			hour_0 = 0;
		}
	}
	else if (switches == 2) {//Configurando minutos
		if(*fallof_ptr == 1){minutes_0++;}
		else if ((*fallof_ptr == 2) & (minutes_0 >= 0)) {minutes_0--;}

		if (minutes_0 > 9){
			minutes_1++;
			minutes_0 = 0;
		}
		if (minutes_1 == 6){
			minutes_1 = 0;
			minutes_0 = 0;
		}
	}
}
/*
 * Funcion: init_btn_pio.
 * Entradas: No.
 * Salidas: No.
 * Este metodo se encarga de registrar las posibles interrupciones por botones.
 */
static void init_btn_pio()
{
	alt_ic_isr_register(BTN_IRQ_INTERRUPT_CONTROLLER_ID, BTN_IRQ, (void *)handle_btn_pressed, NULL, 0x0);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BTN_BASE, 0x0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BTN_BASE, 0xf);
}
/*
 * Funcion: update_time.
 * Entradas: void.
 * Salidas: No.
 * Este metodo se encarga de actualizar la hora en el reloj de acuerdo con el
 * contador del temporizador.
 */
void update_time(void)
{
	int minutes_0max = 9;
	int minutes_1max = 6;
	int hour_0max = 9;
	int hour_1max = 2;

	if (seconds == 60){ // 60 segundos
		seconds = 0;
		minutes_0++;
	}
	if (minutes_0 > minutes_0max){ // 09 minutos
		minutes_0 = 0;
		minutes_1++;
	}
	if (minutes_1 == minutes_1max){ // 60 minutos
		minutes_1 = 0;
		hour_0++;
	}
	if (hour_0 > hour_0max){ // 09 horas
		hour_0 = 0;
		hour_1++;
	}
	if ((hour_1 == hour_1max) & (hour_0 == 4)){ // 24 horas
		hour_1 = 0;
		hour_0 = 0;
	}
	if (alarm_flag == 0){ // Flag para mostrar el reloj en los 7 segmentos y no la alarma
		Segments_0 = display_segments(minutes_0);
		Segments_1 = display_segments(minutes_1);
		Segments_2 = display_segments(hour_0);
		Segments_3 = display_segments(hour_1);

		IOWR_ALTERA_AVALON_PIO_DATA(SEG_0_BASE, Segments_0);
		IOWR_ALTERA_AVALON_PIO_DATA(SEG_1_BASE, Segments_1);
		IOWR_ALTERA_AVALON_PIO_DATA(SEG_2_BASE, Segments_2);
		IOWR_ALTERA_AVALON_PIO_DATA(SEG_3_BASE, Segments_3);
	}
	if (switches != 4){ // Flag para mantener encendida la alarma.
		sound_alarm();
	}
	else {
		IOWR_ALTERA_AVALON_PIO_DATA(BUZZER_BASE, 0);
		IOWR_ALTERA_AVALON_PIO_DATA(0x21020, 0x0);
	}
}
/*
 * Funcion: sound_alarm.
 * Entradas: No.
 * Salidas: No.
 * Este metodo se encarga de ejecutar el sonido y la animacion
 * cuando se alcanza la hora de la alarma.
 */
void sound_alarm()
{
	if ((minutes_0 == alarm_0) &
		(minutes_1 == alarm_1) &
		(hour_0 == alarm_2) &
		(hour_1 == alarm_3) &
		(seconds <= 30)){
		IOWR_ALTERA_AVALON_PIO_DATA(BUZZER_BASE, 1);
		if (seconds % 2 == 0){
			IOWR_ALTERA_AVALON_PIO_DATA(0x21020, 0x15);
		}else {
			IOWR_ALTERA_AVALON_PIO_DATA(0x21020, 0x0a);
		}
		printf("Alarma\n");
	}
	else {
		IOWR_ALTERA_AVALON_PIO_DATA(BUZZER_BASE, 0);
		IOWR_ALTERA_AVALON_PIO_DATA(0x21020, 0x0);
	}
}
/*
 * Funcion: main.
 * Entradas: void.
 * Salidas: No.
 * Funcion principal del programa.
 */
int main(void)
{
	printf("Hello from Nios II!\n");
	init_timer_interrupt();
	init_btn_pio();

	while(1)
	{

	}

	return 0;
}
