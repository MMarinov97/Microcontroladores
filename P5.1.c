#include <msp430g2533.h>

#define ESTADO_A 0
#define ESTADO_B 1
#define ESTADO_C 2

#define EVENTO_S3 3
#define EVENTO_S4 4
#define EVENTO_S5 5
#define EVENTO_S6 6
int estado_actual = ESTADO_A;

/* Funcion de configuracion de los perifericos */
void config_perifericos();

/* Funcion que manejara los leds */
void manejar_LEDs(int estado);

/* Funcion que manejara los estados */
void manejar_estados(int evento);

/*
 __        __    _       _         _
 \ \      / /_ _| |_ ___| |__   __| | ___   __ _
  \ \ /\ / / _` | __/ __| '_ \ / _` |/ _ \ / _` |
   \ V  V / (_| | || (__| | | | (_| | (_) | (_| |
    \_/\_/ \__,_|\__\___|_| |_|\__,_|\___/ \__, |
                                           |___/
*/

/* Interrupcion para el Watchdog */
#pragma vector = WDT_VECTOR;
__interrupt void WDT_ISR(){
    WDTCTL = WDTPW + WDTHOLD; /* Paramos el Watchdog */

    /* Puerto 1 */
    /* Limpiamos el flag de interrupcion */
    P1IFG &= ~BIT4;
    /* Habilitamos de nuevo la interrupcion */
    P1IE |= BIT4;

    /* Puerto 2 */
    /* Limpiamos el flag de interrupcion */
    P2IFG &= ~(BIT1|BIT2|BIT3);
    /* Habilitamos de nuevo la interrupcion */
    P2IE |= (BIT1|BIT2|BIT3);
}

/*
  ____  _   _ _____ ____ _____ ___    _
 |  _ \| | | | ____|  _ \_   _/ _ \  / |
 | |_) | | | |  _| | |_) || || | | | | |
 |  __/| |_| | |___|  _ < | || |_| | | |
 |_|    \___/|_____|_| \_\|_| \___/  |_|

*/

/* Interrupcion para el puerto 1 */
#pragma vector = PORT1_VECTOR;
__interrupt void PORT1_ISR(){
    if((P1IFG & BIT4) != 0){ /* Se ha pulsado S3 */
        /* Deshabilitamos la interrupcion */
        P1IE &= ~BIT4;

        /* Configuramos el Watchdog */
        WDTCTL = WDT_MDLY_32;
        IFG1 &= ~(WDTIFG); /* Limpiamos el flag */
        IE1 |= WDTIE; /* Habilitamos la interrupcion del Watchdog */

        /* COMPROBACION DE FLANCO */
        if((P1IES & BIT4) == BIT4){ /* Flanco de BAJADA */
            manejar_estados(EVENTO_S3);
            P1IES &= ~BIT4; /* Configuramos para flanco de subida */
        } else { /* Flanco de SUBIDA */
            P1IES |= BIT4; /* Configuramos para el flanco de subida */
        }
    }
}

/*
  ____  _   _ _____ ____ _____ ___    ____
 |  _ \| | | | ____|  _ \_   _/ _ \  |___ \
 | |_) | | | |  _| | |_) || || | | |   __) |
 |  __/| |_| | |___|  _ < | || |_| |  / __/
 |_|    \___/|_____|_| \_\|_| \___/  |_____|
*/
/* Interrupcion para el puerto 2 */
#pragma vector = PORT2_VECTOR;
__interrupt void PORT2_ISR(){
    if((P2IFG & BIT1) != 0){ /* Se ha pulsado S4 */
        /* Deshabilitamos la interrupcion */
        P2IE &= ~BIT1;

        /* Configuramos el Watchdog */
        WDTCTL = WDT_MDLY_32;
        IFG1 &= ~(WDTIFG); /* Limpiamos el flag */
        IE1 |= WDTIE; /* Habilitamos la interrupcion del Watchdog */

        /* COMPROBACION DE FLANCO */
        if((P2IES & BIT1) == BIT1){ /* Flanco de BAJADA */
            P2IES &= ~BIT1; /* Configuramos para flanco de subida */
        } else { /* Flanco de SUBIDA */
            manejar_estados(EVENTO_S4);
            P2IES |= BIT1; /* Configuramos para el flanco de bajada */
        }
    }

    if((P2IFG & BIT2) != 0){ /* Se ha pulsado S5 */
        /* Deshabilitamos la interrupcion */
        P2IE &= ~BIT2;

        /* Configuramos el Watchdog */
        WDTCTL = WDT_MDLY_32;
        IFG1 &= ~(WDTIFG); /* Limpiamos el flag */
        IE1 |= WDTIE; /* Habilitamos la interrupcion del Watchdog */

        /* COMPROBACION DE FLANCO */
        if((P2IES & BIT2) == BIT2){ /* Flanco de BAJADA */
            manejar_estados(EVENTO_S5);
            P2IES &= ~BIT2; /* Configuramos para flanco de subida */
        } else { /* Flanco de SUBIDA */
            P2IES |= BIT2; /* Configuramos para el flanco de bajada */
        }
    }

    if((P2IFG & BIT3) != 0){ /* Se ha pulsado S6 */
        /* Deshabilitamos la interrupcion */
        P2IE &= ~BIT3;

        /* Configuramos el Watchdog */
        WDTCTL = WDT_MDLY_32;
        IFG1 &= ~(WDTIFG); /* Limpiamos el flag */
        IE1 |= WDTIE; /* Habilitamos la interrupcion del Watchdog */

        /* COMPROBACION DE FLANCO */
        if((P2IES & BIT3) == BIT3){ /* Flanco de BAJADA */
            P2IES &= ~BIT3; /* Configuramos para flanco de subida */
        } else { /* Flanco de SUBIDA */
            manejar_estados(EVENTO_S6);
            P2IES |= BIT3; /* Configuramos para el flanco de bajada */
        }
    }
}

/*
  __  __    _    ___ _   _
 |  \/  |  / \  |_ _| \ | |
 | |\/| | / _ \  | ||  \| |
 | |  | |/ ___ \ | || |\  |
 |_|  |_/_/   \_\___|_| \_|
*/
int main(){
    config_perifericos();
    while(1);
}
/*
   ____ ___  _   _ _____ ___ ____
  / ___/ _ \| \ | |  ___|_ _/ ___|
 | |  | | | |  \| | |_   | | |  _
 | |__| |_| | |\  |  _|  | | |_| |
  \____\___/|_| \_|_|   |___\____|
*/
void config_perifericos(){
    /* Configuracion del Watchdog */
    WDTCTL = WDTPW  + WDTHOLD; /* Paramos el Watchdog */
    IE1 |= WDTIE; /* Habilitamos la interrupcion para el Watchdog */

    /* Configuracion de los pulsadores */
    /* Pulsador S3 */
    P1DIR &=~BIT4;/* Lo habilitamos como entrada */
    P1REN |= BIT4;/* Habilitamos la resistencia */
    P1OUT |= BIT4;/* Iniciamos a no pulsado */

    P1IFG &= ~BIT4; /* Flag de interrupcion limpiado */
    P1IE |= BIT4; /* Interrupt habilitado para S3 */
    P1IES |= BIT4; /* Detectamos flanco de BAJADA */

    /* Pulsadores S4, S5 y S6 */
    P2DIR &=~(BIT1|BIT2|BIT3); /* Los habilitamos como entrada */
    P2REN |= (BIT1|BIT2|BIT3); /* Habilitamos la resistencia */
    P2OUT |= (BIT1|BIT2|BIT3); /* Iniciamos a no pulsado */

    P2IFG &= ~(BIT1|BIT2|BIT3); /* Flags de interrupcion limpiados */
    P2IE |= (BIT1|BIT2|BIT3); /* Interrupt habilitado para S4, S5 y S6 */
    P1IES |= (BIT1|BIT2|BIT3); /* Detectamos flanco de SUBIDA para S4 y S6 */
  //  P1IES |= BIT2; /* Detectamos flanco de BAJADA para S5 */

    /*Configuracion de los LEDs */
    P2SEL &=~ (BIT6|BIT7);
    P2SEL2 &=~ (BIT6|BIT7);

    P2DIR |= (BIT4|BIT5|BIT6|BIT7);/* Los habilitamos como SALIDA */
    P2OUT &=~ (BIT4|BIT5|BIT6|BIT7);/* Inicializamos a APAGADO */

    /* Habilitamos los interrupts */
    __enable_interrupt();
}


/*
  _____ ____ _____  _    ____   ___  ____
 | ____/ ___|_   _|/ \  |  _ \ / _ \/ ___|
 |  _| \___ \ | | / _ \ | | | | | | \___ \
 | |___ ___) || |/ ___ \| |_| | |_| |___) |
 |_____|____/ |_/_/   \_\____/ \___/|____/
*/
void manejar_LEDs(int estado){
    switch(estado){
        case ESTADO_A:
            P2OUT &= ~(BIT4|BIT5|BIT6|BIT7);
            break;
        case ESTADO_B:
            P2OUT &= ~(BIT5|BIT7);
            P2OUT |= (BIT4|BIT6);
            break;
        case ESTADO_C:
            P2OUT &= ~(BIT4|BIT6);
            P2OUT |= (BIT5|BIT7);
    }
}


void manejar_estados(int evento){
    switch(estado_actual){
        case ESTADO_A:
            if(evento == EVENTO_S3){
                estado_actual = ESTADO_B;
            }
            break;
        case ESTADO_B:
            if(evento == EVENTO_S4){
                estado_actual = ESTADO_C;
            }
            if(evento == EVENTO_S6){
                estado_actual = ESTADO_A;
            }
            break;
        case ESTADO_C:
            if(evento == EVENTO_S5){
                estado_actual = ESTADO_B;
            }
    }
    manejar_LEDs(estado_actual);
}
