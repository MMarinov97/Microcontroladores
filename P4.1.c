/* Descripción:
 * Cuando se presione el pulsador Sx conmutará el correspondiente LEDx.
 * Consideraciones:
 * La gestión de los pulsadores se realizará por interrupciones.
 */
#include <msp430g2533.h>

/* Funcion que configura todo */
void config_perifericos();

/* Interrupcion para el puerto 1 */
#pragma vector = PORT1_VECTOR;
__interrupt void PORT1_ISR(){
    P2OUT ^= BIT4; /* Conmutamos el LED */
    P1IFG = 0; /* Limpiamos el flag de interrupcion */
}

/* Interrupcion para el puerto 2 */
#pragma vector = PORT2_VECTOR;
__interrupt void PORT2_ISR(){
   /* Comprobamos que boton se ha pulsado y conmutara el LED correspondiente */
    switch(P2IFG){
		case BIT1:
			P2OUT ^= BIT5;
			break;
		case BIT2:
			P2OUT ^= BIT6;
			break;
		case BIT3:
			P2OUT ^= BIT7;
    }
    P2IFG = 0; /* Limpiamos el flag de interrupcion */
}

/* MAIN */
int main(void){
    config_perifericos();
    while(1);
}
/* FIN DE MAIN */

void config_perifericos(){
    WDTCTL = WDTPW + WDTHOLD; /* Parar el timer del watchdog */

    /* Configuracion de los pulsadores */
    P1IE |= BIT4; /* Interrupt habilitado para el boton S3 */
    P1IFG &=~ BIT4; /*  P1.4 IFG limpiado */

    P2IE |= (BIT1|BIT2|BIT3); /* Interrupt habilitado para los botones S4, S5 y S6 */
    P2IFG &=~ (BIT1|BIT2|BIT3); /* P2.1, P2.2 y P2.3 IFG limpiado */

    /* Boton S3 */
    P1DIR &=~BIT4;/* Lo habilitamos como entrada */
    P1REN |= BIT4;/* Habilitamos la resistencia */
    P1OUT |= BIT4;/* Iniciamos a no pulsado*/

    /* Botones S4, S5 y S6 */
    P2DIR &=~(BIT1|BIT2|BIT3);
    P2REN |= (BIT1|BIT2|BIT3);
    P2OUT |= (BIT1|BIT2|BIT3);
    /* Configuracion de los LEDs */

    P2SEL &=~ (BIT6|BIT7);
    P2SEL2 &=~ (BIT6|BIT7);

    P2DIR |= (BIT4|BIT5|BIT6|BIT7);/* Los habilitamos como SALIDA */
    P2OUT &=~ (BIT4|BIT5|BIT6|BIT7);/* Inicializamos a APAGADO */

    /* Habilitamos los interrupts */
    __enable_interrupt();
}