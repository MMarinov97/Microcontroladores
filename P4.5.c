/* Descripción:
 * Cuando se presione el pulsador Sx se encenderá el correspondiente LEDx durante 1s.
 * Consideraciones:
 * La gestión de los pulsadores y de los temporizadores se realizará por interrupciones.
 * Se usará el temporizador TA0 y el bloque 1 (TA0CCR1) en modo continuo, con la señal de reloj
 * SMCLK por defecto ~1.1MHz y el divisor /8.
 */

#include <msp430g2533.h>
#define NUM 34375


/* Funcion que configura todo */
void config_perifericos();
/* Interrupcion para el puerto 1 */
#pragma vector = PORT1_VECTOR;
__interrupt void PORT1_ISR(){
    P2OUT |= BIT4;
    /* Habilitamos el timer
     * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
    TACTL|=MC_2;
    P1IFG &= ~BIT4; /* Limpiamos el flag de interrupcion */
}
/* Interrupcion para el puerto 2 */
#pragma vector = PORT2_VECTOR;
__interrupt void PORT2_ISR(){
   /* Comprobamos que boton se ha pulsado y conmutamos el LED correspondiente */
    switch(P2IFG){
    case BIT1:
        P2OUT |= BIT5;
		P2IFG &= ~BIT1; /* Limpiamos el flag de interrupcion */
        /* Habilitamos el timer
         * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
        TACTL|=MC_2;
        break;
    case BIT2:
        P2OUT |= BIT6;
		P2IFG &= ~BIT2; /* Limpiamos el flag de interrupcion */
        /* Habilitamos el timer
         * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
        TACTL|=MC_2;
        break;
    case BIT3:
        P2OUT |= BIT7;
		P2IFG &= ~ BIT3; /* Limpiamos el flag de interrupcion */
        /* Habilitamos el timer
         * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
        TACTL|=MC_2;
    }
}
/* Interrupcion para el timer */
#pragma vector = TIMER0_A1_VECTOR
__interrupt void RTI_TA0CCR12(){
    static int cont = 1;
    switch(TA0IV){ /* Al acceder al registro TAIV  limpiamos los flags de interrupcion.
                    * Es importante hacerlo ya que sino entraria en un bucle infinito y bloquearia
                    * la CPU */
    case(TA0IV_TACCR1):
            if(cont == 4){
                P2OUT &=~ (BIT4|BIT5|BIT6|BIT7);/* Apagamos los LEDs */
                /* Apagamos el temporizador */
                /* Actualizamos el valor de TACCR1 para la siguiente cuenta */
                TACTL &=~ MC_2; /* Apagamos el timer ASI!!!!!!!!! */
                cont = 1;
            } else {
                cont = cont + 1;
                TACTL &=~ MC_2; /* Apagamos el timer ASI!!!!!!!!! */
                TACCR1 = TAR + NUM; /* Actualizamos el valor de TACCR1 */
                TACTL|=MC_2; /* Volvemos a poner el timer */
            }

    }
}

/* MAIN */
int main(void){
    config_perifericos();
    while(1);
}
/* FIN DE MAIN */

void config_perifericos(){
    WDTCTL =WDTPW + WDTHOLD; /* Parar el timer del watchdog */

    /* Configuracion de los pulsadores */
    P1IE |= BIT4; /* Interrupt habilitado para el boton S3 */
	P1IES |= BIT4; /* Detectamos flanco de bajada */
    P1IFG &=~ BIT4; /*  P1.4 IFG limpiado */

    P2IE |= (BIT1|BIT2|BIT3); /* Interrupt habilitado para los botones S4, S5 y S6 */
	P2IES |= (BIT1|BIT2|BIT3); /* Detectamos flanco de bajada */
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

    /* Configuracion del timer */
    TA0CCTL1 = CCIE;

    TACCR1 = NUM; /* NUM = Ciclos que tiene que contar el timer */

    /* TASSEL_2 -> Fuente del timer es SMCLK
     * ID_3 -> Divisor del timer /8 */
    TACTL = TASSEL_2 + ID_3 + TAIE;

    /* Habilitamos los interrupts */
    __enable_interrupt();
}
