/* Ejercicio 3.6
 * Se encenderá el LED5, y cuando se presione el pulsador Sx el LED4 parpadeará con un PWM de
frecuencia 50Hz y con distinto ciclo de trabajo según se presione S4 (10%), S5 (50%) o S6 (90%), y
cuando se presione S3 el LED4 se apagará.
Consideraciones:
- La gestión de los pulsadores se realizará por sondeo.
- Se usará el temporizador TA1, con la señal de reloj SMCLK por defecto ~1.1MHz, sin divisor y con el
modo OUTMOD_7
 */


#include <msp430g2533.h>
#define N1 2199;
#define N2 10999;
#define N3 19799
void config_perifericos();

int main(void){
    /* Declaramos las variables necesarias para controlar los botones */
    int S3 = 1,S4 = 1,S5 = 1,S6 = 1;

    config_perifericos(); /* Configuracion de los perifericos */


    for(;;){ /* Bucle infinito */
        /* Pulsador S3 y LED 3 */
        if((P1IN & BIT4)==0 ){ /* Comprobamos si el boton S3 esta pulsado */
            if(S3!=0){ /* Comprobamos si anteriormente estaba pulsado */
                  TA1CCTL2 &= ~OUTMOD_7;/* Quitamos el OUTMOD_7 */
                  TA1CTL &= ~ MC_1;
                  S3=0; /* Actualizamos el valor del pulsador a pulsado */
                 }
        }else{
            S3=1; /* Actualizamos el valor del pulsador a no pulsado */
        }
        /* Pulsador S4 y LED 4 */
        if((P2IN & BIT1)==0 ){/* Comprobamos si S4 esta pulsado */
            if(S4!=0){  /* Y si su estado ha cambiado */
                TA1CCR2 = N1; /* N1 = 10% */
                TA1CCTL2 |= OUTMOD_7;/* Ponemos el OUTMOD_7 */
                TA1CTL |= MC_1 + TACLR;/* Encendemos el timer */

                S4=0; /* Actualizamos el valor del pulsador a pulsado */
            }
        }else{
            S4=1; /* Actualizamos el valor del pulsador a no pulsado */
        }
        /* Pulsador 5 y LED 5 */
        if((P2IN & BIT2)==0 ){ /* Comprobamos si S5 esta pulsado */
            if(S5!=0){ /* Y si su estado ha cambiado */
                TA1CCR2 = N2; /* N2 = 50% */
                TA1CCTL2 |= OUTMOD_7; /* Ponemos el OUTMOD_7 */
                TA1CTL |= MC_1 + TACLR;/* Encendemos el timer */

                S5=0; /* Actualizamos el valor del pulsador a pulsado */
            }
        }else{
            S5=1; /* Actualizamos el valor del pulsador a no pulsado */
         }
        /* Pulsador 6 y LED 6 */
        if((P2IN & BIT3)==0 ){/* Comprobamos si S5 esta pulsado */
            if(S6!=0){ /* Y si su estado ha cambiado */
                TA1CCR2 = N3; /* N3 = 90% */
                TA1CCTL2 |= OUTMOD_7;/* Ponemos el OUTMOD_7 */
                TA1CTL |= MC_1 + TACLR;/* Encendemos el timer */

                S6=0; /* Actualizamos el valor del pulsador a pulsado */
            }
        }else{
            S6=1; /* Actualizamos el valor del pulsador a no pulsado */
        }
    }
}

void config_perifericos(){
    WDTCTL = WDTPW + WDTHOLD; /* Stop watchdog timer  */

    /* Configuracion de los pulsadores */

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
    P2OUT &=~ (BIT4|BIT5|BIT7);/* Inicializamos a APAGADO */
    P2OUT |= BIT6; /* Inicializamos a ENCENDIDO */

    /* Vinculamos el LED 4 a la señal de salida */
    P2SEL |= BIT5;
    P2SEL2 &=~ BIT5;
    /* TASSEL_2 -> Fuente del timer es SMCLK */
    TA1CTL = TASSEL_2;

    TA1CCR0 = 21999;
}
