/* Descripción:
 * Cuando se presione el pulsador Sx se encenderá el correspondiente LEDx durante 1s.
 * Consideraciones:
 * La gestión de los pulsadores y de los temporizadores se realizará por interrupciones.
 * Se usará el temporizador TA0 y el bloque 0 (TA0CCR0) en modo ascendente, con la señal de reloj
 * SMCLK por defecto ~1.1MHz y el divisor /8
 */

#include <msp430g2533.h>
#define NUM 34374
/* Funcion que configura todo */
void config_perifericos();
/* Interrupcion para el puerto 1 */
#pragma vector = PORT1_VECTOR;
__interrupt void PORT1_ISR(){
    if((P1IFG & BIT4) != 0){
        P2OUT |= BIT4; /* Encendemos el LED */
        /* Habilitamos el timer
         * MC_1 -> Modo de operacion ascendente, cuenta de 0 a TACCR0
         * TACLR -> Resetea el timer a 0 */
        TA0CTL |= MC_1 + TACLR;
        P1IFG &= ~BIT4; /* Limpiamos el flag de interrupcion */
    }
}
/* Interrupcion para el puerto 2 */
#pragma vector = PORT2_VECTOR;
__interrupt void PORT2_ISR(){
    /* Comprobamos que boton se ha pulsado y conmutara el LED correspondiente */
     if((P2IFG & BIT1) != 0){
         /* Habilitamos el timer
          * MC_1 -> Modo de operacion ascendente, cuenta de 0 a TACCR0
          * TACLR -> Resetea el timer a 0 */
         TA0CTL |= MC_1 + TACLR;
         P2IFG &= ~BIT1; /* Limpiamos el flag de interrupcion */
         P2OUT |= BIT5;
     }

     if((P2IFG & BIT2) != 0){
         P2OUT |= BIT6;
         P2IFG &= ~BIT2; /* Limpiamos el flag de interrupcion */
         TA0CTL |= MC_1 + TACLR;
     }
     if((P2IFG & BIT3) != 0){
         P2OUT |= BIT7;
         P2IFG &= ~BIT3; /* Limpiamos el flag de interrupcion */
         TA0CTL |= MC_1 + TACLR;
     }

}
/* Interrupcion para el timer */

#pragma vector = TIMER0_A0_VECTOR
__interrupt void RTI_TA0CCR12(){
    static int cont = 0;
    cont = cont + 1;
    if(cont == 4){
        /* Apagamos el temporizador */
        TA0CTL &= ~MC_1;
        P2OUT &=~ (BIT4|BIT5|BIT6|BIT7); /* Apagamos los LEDs */
        cont = 0; /* Reseteamos el contador */
    }
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

    /* Boton S3 */
    P1DIR &=~BIT4;/* Lo habilitamos como entrada */
    P1REN |= BIT4;/* Habilitamos la resistencia */
    P1OUT |= BIT4;/* Iniciamos a no pulsado*/

    /* Botones S4, S5 y S6 */
    P2DIR &=~(BIT1|BIT2|BIT3);
    P2REN |= (BIT1|BIT2|BIT3);
    P2OUT |= (BIT1|BIT2|BIT3);
    /* Interrupciones Botones */
    P1IFG &=~ BIT4; /*  P1.4 IFG limpiado */
    P1IE |= BIT4; /* Interrupt habilitado para el boton S3 */
    P1IES |= BIT4; /* Detectamos flanco de bajada */

    P2IFG &=~ (BIT1|BIT2|BIT3); /* P2.1, P2.2 y P2.3 IFG limpiado */
    P2IE |= (BIT1|BIT2|BIT3); /* Interrupt habilitado para los botones S4, S5 y S6 */
    P2IES |= (BIT1|BIT2|BIT3); /* Detectamos flanco de bajada */
    /* Configuracion de los LEDs */

    P2SEL &=~ (BIT6|BIT7);
    P2SEL2 &=~ (BIT6|BIT7);

    P2DIR |= (BIT4|BIT5|BIT6|BIT7);/* Los habilitamos como SALIDA */
    P2OUT &=~ (BIT4|BIT5|BIT6|BIT7);/* Inicializamos a APAGADO */

    /* Configuracion del timer */
    TA0CCTL0 = CCIE;
    TA0CCTL0 &= ~CCIFG;
    TACCR0 = NUM; /* Los ciclos que tiene que contar el timer */

    /* TASSEL_2 -> Fuente del timer es SMCLK
     * ID_3 -> Divisor del timer /8 */
    TA0CTL=TASSEL_2+ID_3;

    /* Habilitamos los interrupts */
    __enable_interrupt();
}