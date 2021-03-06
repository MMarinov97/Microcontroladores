#include <msp430g2533.h>

#define ESTADO_A 0
#define ESTADO_B 1
#define ESTADO_C 2

#define EVENTO_S3 BIT0
#define EVENTO_S4 BIT1
#define EVENTO_S5 BIT2
#define EVENTO_S6 BIT3

#define SET(ev) (eventos |= (ev))
#define RESET(ev) (eventos &= ~(ev))
#define TEST(ev) (eventos & (ev))

unsigned char eventos;
/* Funcion de configuracion de los perifericos */
void config_perifericos();

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

        /* Configuramos el Watchdog
         * WDT_MDLY_32 -> WDTPW + WDTTMSEL + WDTCNTCL
         * WDTPW = Clave de seguridad
         * WDTTMSEL = Modo Intervalo
         * WDTCNTCL = Empezar cuenta con reset
         * La señal de reloj es SMCLK, modo TIMER y tiempo de 32ms */

        WDTCTL = WDT_MDLY_32;
        IFG1 &= ~(WDTIFG); /* Limpiamos el flag */
        IE1 |= WDTIE; /* Habilitamos la interrupcion del Watchdog */

        /* COMPROBACION DE FLANCO */
        if((P1IES & BIT4) == BIT4){ /* Flanco de BAJADA */
            SET(EVENTO_S3);
        }
        /* Conmutamos el flanco */
        P1IES ^= BIT4;
    }
    /* Comprobamos si hay eventos pendientes */
    if(eventos){
        __low_power_mode_off_on_exit();
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
        } else { /* Flanco de SUBIDA */
            SET(EVENTO_S4);
        }
        /* Conmutamos flanco */
        P2IES ^= BIT1;
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
            SET(EVENTO_S5);
        }
        P2IES ^= BIT2;
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
        } else { /* Flanco de SUBIDA */
            SET(EVENTO_S6);
        }
        P2IES ^= BIT3;
    }
    if(eventos){
        __low_power_mode_off_on_exit();
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

    unsigned char estado = ESTADO_A;

    while(1){
        __low_power_mode_0();

        while(eventos){ /* Mientras que queden eventos pendientes... */
            switch(estado){ /* Haremos algo distinto dependiendo del estado en el que estemos */
                case ESTADO_A:          /* ESTADO A */
                    if(TEST(EVENTO_S3)){
                        RESET(EVENTO_S3);

                        P2OUT |= (BIT4|BIT6);
                        P2OUT &= ~(BIT5|BIT7);

                        estado = ESTADO_B;
                    }

                    if(TEST(EVENTO_S4)){
                        RESET(EVENTO_S4);
                    }

                    if(TEST(EVENTO_S5)){
                        RESET(EVENTO_S5);
                    }

                    if(TEST(EVENTO_S6)){
                        RESET(EVENTO_S6);
                    }
                    break;

                case ESTADO_B:      /* ESTADO_B */
                    if(TEST(EVENTO_S3)){
                        RESET(EVENTO_S3);
                    }

                    if(TEST(EVENTO_S4)){
                        RESET(EVENTO_S4);
                        P2OUT |= (BIT5|BIT7);
                        P2OUT &= ~(BIT4|BIT6);

                        estado = ESTADO_C;
                    }

                    if(TEST(EVENTO_S5)){
                        RESET(EVENTO_S5);
                    }

                    if(TEST(EVENTO_S6)){
                        RESET(EVENTO_S6);

                        P2OUT &= ~(BIT4|BIT4|BIT5|BIT6|BIT7);

                        estado = ESTADO_A;
                    }
                    break;

                case ESTADO_C:
                    if(TEST(EVENTO_S3)){
                        RESET(EVENTO_S3);
                    }

                    if(TEST(EVENTO_S4)){
                        RESET(EVENTO_S4);
                    }

                    if(TEST(EVENTO_S5)){
                        RESET(EVENTO_S5);

                        P2OUT |= (BIT4|BIT6);
                        P2OUT &= ~(BIT5|BIT7);

                        estado = ESTADO_B;
                    }

                    if(TEST(EVENTO_S6)){
                        RESET(EVENTO_S6);
                    }
                    break;
            }
        }
    }
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
    DCOCTL = 0;             /* Frecuencia DCO (1MHz) */
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
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
    P2IES |= (BIT1|BIT2|BIT3); /* Detectamos flanco de BAJADA para S4, S5 y S6 */

    /*Configuracion de los LEDs */
    P2SEL &=~ (BIT6|BIT7);
    P2SEL2 &=~ (BIT6|BIT7);

    P2DIR |= (BIT4|BIT5|BIT6|BIT7);/* Los habilitamos como SALIDA */
    P2OUT &=~ (BIT4|BIT5|BIT6|BIT7);/* Inicializamos a APAGADO */
}
