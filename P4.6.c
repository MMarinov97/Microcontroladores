/* p4.6
 * Descripción:
 * Cuando se presione Sx parpadeará el LEDx con distinta frecuencia según se trate de LED3 (1.1Hz), LED4
 * (1.25Hz), LED5 (1.375Hz) o LED6 (2Hz).
 * Consideraciones:
 * La gestión de los pulsadores y de los temporizadores se realizará por interrupciones.
 * Se usará el temporizador TA0 y el bloque 0 (TA0CCR0) para el LED3 y el bloque 1 (TA0CCR1) para el
 * LED4 en modo continuo, con la señal de reloj SMCLK por defecto ~1.1MHz y el divisor /8.
 * Se usará el temporizador TA1 y el bloque 1 (TA1CCR1) para el LED5 y el bloque 2 (TA1CCR2) para el
 * LED6 en modo continuo, con la señal de reloj SMCLK por defecto ~1.1MHz y el divisor /8
 */

#include <msp430g2533.h>
#define NUM1 62500 /* 1.1 Hz */
#define NUM2 55000 /* 1.25 Hz */
#define NUM3 50000 /* 1.375 Hz */
#define NUM4 34374 /* 2 Hz */


/* Funcion que configura todo */
void config_perifericos();


/* ____  _   _ _____ ____ _____ ___  ____
 *|  _ \| | | | ____|  _ \_   _/ _ \/ ___|
 *| |_) | | | |  _| | |_) || || | | \___ \
 *|  __/| |_| | |___|  _ < | || |_| |___) |
 *|_|    \___/|_____|_| \_\|_| \___/|____/
 */
/* Interrupcion para el puerto 1 */
#pragma vector = PORT1_VECTOR;
__interrupt void PORT1_ISR(){
    P2OUT |= BIT4; /* Encendemos el LED 3 */
    /* Habilitamos el timer
     * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
    TACTL|=MC_2;
    TA0CCTL0 |= CCIE; /* Habilitamos la interrupcion */
    TA0CCR0 = TA0R + NUM1; /* Damos el valor para la cuenta */
    P1IFG &= ~BIT4; /* Limpiamos el flag de interrupcion */
}


/* Interrupcion para el puerto 2 */
#pragma vector = PORT2_VECTOR;
__interrupt void PORT2_ISR(){
   /* Comprobamos que boton se ha pulsado y conmutamos el LED correspondiente */
    if((P2IFG & BIT1) != 0){
        P2OUT |= BIT5; /* Encendemos el LED 4 */
        TA0CCTL1 |= CCIE; /* Hablitamos la interrupcion */
        TA0CCR1 = NUM2; /* Damos el valor para la cuenta */
        P2IFG &=~ BIT1; /* Limpiamos el flag de interrupcion */
        /* Habilitamos el timer
         * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
        TA0CTL|=MC_2;
    }
    if((P2IFG & BIT2) != 0){
        P2OUT |= BIT6; /* Encendemos el LED 5 */
        TA1CCR1 = NUM3;  /* Damos el valor para la cuenta */
        TA1CCTL1 |= CCIE; /* Habilitamos la interrupcion */
        P2IFG &= ~BIT2; /* Limpiamos el flag de interrupcion */
        /* Habilitamos el timer
         * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
        TA1CTL|=MC_2;
    }
    if((P2IFG & BIT3) != 0){
        P2OUT |= BIT7; /* Encendemos el LED 6 */
        TA1CCTL2 |= CCIE; /* Habilitamos la interrupcion */
        TA1CCR2 = NUM4; /* Damos el valor para la cuenta */
        P2IFG &= ~ BIT3; /* Limpiamos el flag de interrupcion */
        /* Habilitamos el timer
         * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
        TA1CTL|=MC_2;
    }
}

/* _____ ___ __  __ _____ ____
 *|_   _|_ _|  \/  | ____|  _ \
 *  | |  | || |\/| |  _| | |_) |
 *  | |  | || |  | | |___|  _ <
 *  |_| |___|_|  |_|_____|_| \_\
 */
/* Interrupciones para el timer */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void RTI_TA0CCR0(){
    /* Interrupcion para TA0CCR0 */
    P2OUT ^= BIT4; /* Conmutamos el LED 3 */
    TA0CCR0 = TA0R + NUM1; /* Actualizamos el valor de la cuenta */
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void RTI_TA0CCR1(){
    /* Interrupcion para TA0CCR1 */
    switch(TA0IV){
    case(TA0IV_TACCR1):
            P2OUT ^= BIT5; /* Conmutamos el LED 4 */
            TA0CCR1 = TA0R + NUM2; /* Actualizamos el valor de la cuenta */
    }
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void RTI_TA0CCR12(){
    /* Interrupcion para TA1CCR1 y TA1CCR2 */
    switch(TA1IV){
    case(TA1IV_TACCR1):
            P2OUT ^= BIT6; /* Conmutamos el LED 5 */
            TA1CCR1 = TA1R + NUM3; /* Actualizamos el valor de la cuenta */
            break;
    case(TA1IV_TACCR2):
            P2OUT ^= BIT7; /* Conmutamos el LED 6 */
            TA1CCR2 = TA1R + NUM4; /* Actualizamos el valor de la cuenta */
            break;
    }
}

/* __  __    _    ___ _   _
 *|  \/  |  / \  |_ _| \ | |
 *| |\/| | / _ \  | ||  \| |
 *| |  | |/ ___ \ | || |\  |
 *|_|  |_/_/   \_\___|_| \_|
 */
int main(void){
    config_perifericos();
    while(1);
}
/* FIN DE MAIN */

/*  ____ ___  _   _ _____ ___ ____
 * / ___/ _ \| \ | |  ___|_ _/ ___|
 *| |  | | | |  \| | |_   | | |  _
 *| |__| |_| | |\  |  _|  | | |_| |
 * \____\___/|_| \_|_|   |___\____|
 */
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
    TA0CCR0 = NUM1;
    TA0CCR1 = NUM2;
    TA1CCR1 = NUM3;
    TA1CCR2 = NUM4;

    /* TASSEL_2 -> Fuente del timer es SMCLK
     * ID_3 -> Divisor del timer /8 */
    TACTL = TASSEL_2 + ID_3;

    TA1CTL = TASSEL_2 + ID_3;

    /* Habilitamos los interrupts */
    __enable_interrupt();
}
