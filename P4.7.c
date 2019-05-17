/* Descripción:
 * Cuando se presione el pulsador Sx conmutará el correspondiente LEDx.
 * Consideraciones:
 * La gestión de los pulsadores se realizará por interrupciones.
 */
#include <msp430g2533.h>

/* Funcion que configura todo */
void config_perifericos();

/* Interrupcion para el watchdog */
#pragma vector = WDT_VECTOR;
__interrupt void WDT_ISR(){
       WDTCTL = WDTPW + WDTHOLD; /* Parar el timer del watchdog */
       /* Puerto 1 */
       P1IFG &= ~(BIT4);
       P1IE |= (BIT4); /* Interrupt habilitado para el boton S3 */
       /* Puerto 2 */
       P2IFG &= ~(BIT3|BIT2|BIT1);
       P2IE |= (BIT3|BIT2|BIT1);
}
/* Interrupcion para el puerto 1 */
#pragma vector = PORT1_VECTOR;
__interrupt void PORT1_ISR(){
    if((P1IFG & BIT4) != 0){ /* Se ha pulsado S3 */
        /* Deshabilitamos la interrupcion */
        P1IE &= ~BIT4;
        /* Configuramos el watchdog */
        WDTCTL = WDT_MDLY_32;
        IFG1 &= ~(WDTIFG); /* Limpiamos el flag */
        IE1 |= WDTIE; /* Habilitamos la interrupcion del watchdog */
        /* COMPROBACION DE FLANCO */
        if((P1IES & BIT4) == BIT4){ /* Flanco de bajada */
            P2OUT ^= BIT4; /* Conmutamos el LED */
            P1IES &= ~BIT4; /* Configuramos para flanco de subida */
        } else { /* Flanco de subida */
            P1IES |= BIT4;
        }
    }
}

/* Interrupcion para el puerto 2 */
#pragma vector = PORT2_VECTOR;
__interrupt void PORT2_ISR(){
   /* Comprobamos que boton se ha pulsado y conmutara el LED correspondiente */
    if((P2IFG & BIT1) != 0){
        /* Deshabilitamos la interrupcion */
        P2IE &= ~BIT1;
        /* Configuramos el watchdog */
        WDTCTL = WDT_MDLY_32;
        IFG1 &= ~(WDTIFG); /* Limpiamos el flag */
        IE1 |= WDTIE; /* Habilitamos la interrupcion del watchdog */
        /* COMPROBACION DE FLANCO */
        if((P2IES & BIT1) == BIT1){ /* Flanco de bajada */
            P2OUT ^= BIT5; /* Conmutamos el LED */
            P2IES &= ~BIT1; /* Configuramos para flanco de subida */
        } else {
            P2IES |= BIT1;
        }
    }

    if((P2IFG & BIT2) != 0){
        /* Deshabilitamos la interrupcion */
        P2IE &= ~BIT2;
        /* Configuramos el watchdog */
        WDTCTL = WDT_MDLY_32;
        IFG1 &= ~(WDTIFG); /* Limpiamos el flag */
        IE1 |= WDTIE; /* Habilitamos la interrupcion del watchdog */
        /* COMPROBACION DE FLANCO */
        if((P2IES & BIT2) == BIT2){ /* Flanco de bajada */
            P2OUT ^= BIT6; /* Conmutamos el LED */
            P2IES &= ~BIT2; /* Configuramos para flanco de subida */
        } else {
            P2IES |= BIT2;
        }
    }
    if((P2IFG & BIT3) != 0){
        /* Deshabilitamos la interrupcion */
        P2IE &= ~BIT3;
        /* Configuramos el watchdog */
        WDTCTL = WDT_MDLY_32;
        IFG1 &= ~(WDTIFG); /* Limpiamos el flag */
        IE1 |= WDTIE; /* Habilitamos la interrupcion del watchdog */
        /* COMPROBACION DE FLANCO */
        if((P2IES & BIT3) == BIT3){ /* Flanco de bajada */
            P2OUT ^= BIT7; /* Conmutamos el LED */
            P2IES &= ~BIT3; /* Configuramos para flanco de subida */
        } else {
            P2IES |= BIT3;
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
    /* Configuracion del Watchdog */
    WDTCTL = WDTPW + WDTHOLD; /* Parar el timer del watchdog */
    IE1 |= WDTIE;

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

    /* Habilitamos los interrupts */
    __enable_interrupt();
}
