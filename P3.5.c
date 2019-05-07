/*Descripción:
Cuando se presione el pulsador Sx conmutará el Zumbador, generando una nota de frecuencia 440Hz y ciclo de trabajo 50%.
Consideraciones:
 La gestión de los pulsadores se realizará por sondeo.
 Se usará el temporizador TA1, con la señal de reloj SMCLK por defecto ~1.1MHz y sin divisor.
*/


#include <msp430g2533.h>
#define NUM 1250

void config_perifericos();

int main(void){
    /* Declaramos las variables necesarias para controlar los botones */
    int S3 = 1,S4 = 1,S5 = 1,S6 = 1;

    WDTCTL = WDTPW + WDTHOLD; /* Stop watchdog timer  */
    config_perifericos(); /* Configuracion de los perifericos */

    for(;;){ /* Bucle infinito */
        /* Pulsador S3 y LED 3 */
        if((P1IN & BIT4)==0 ){ /* Comprobamos si el boton S3 esta pulsado */
            if(S3!=0){ /* Comprobamos si anteriormente estaba pulsado */
                  TA1CTL ^= MC_1; /* Habilitamos el timer */

                  TA1CCTL0 ^= OUTMOD_0;
                  S3=0; /* Actualizamos el valor del pulsador a pulsado */
                 }
        }else{
            S3=1; /* Actualizamos el valor del pulsador a no pulsado */
        }
        /* Pulsador S4 y LED 4 */
        if((P2IN & BIT1)==0 ){/* Comprobamos si S4 esta pulsado */
            if(S4!=0){  /* Y si su estado ha cambiado */
                TA1CTL ^= MC_1; /* Habilitamos el timer */

                TA1CCTL0 ^= OUTMOD_0;

                S4=0; /* Actualizamos el valor del pulsador a pulsado */
            }
        }else{
            S4=1; /* Actualizamos el valor del pulsador a no pulsado */
        }
        /* Pulsador 5 y LED 5 */
        if((P2IN & BIT2)==0 ){ /* Comprobamos si S5 esta pulsado */
            if(S5!=0){ /* Y si su estado ha cambiado */
                TA1CTL ^= MC_1; /* Habilitamos el timer */

                TA1CCTL0 ^= OUTMOD_0;

                S5=0; /* Actualizamos el valor del pulsador a pulsado */
            }
        }else{
            S5=1; /* Actualizamos el valor del pulsador a no pulsado */
         }
        /* Pulsador 6 y LED 6 */
        if((P2IN & BIT3)==0 ){/* Comprobamos si S5 esta pulsado */
            if(S6!=0){ /* Y si su estado ha cambiado */
                TA1CTL ^= MC_1; /* Habilitamos el timer */

                TA1CCTL0 ^= OUTMOD_0;

                S6=0; /* Actualizamos el valor del pulsador a pulsado */
            }
        }else{
            S6=1; /* Actualizamos el valor del pulsador a no pulsado */
        }
    }
}


void config_perifericos(){
    /* Configuracion de los pulsadores */

    /* Boton S3 */
    P1DIR &=~BIT4;/* Lo habilitamos como entrada */
    P1REN |= BIT4;/* Habilitamos la resistencia */
    P1OUT |= BIT4;/* Iniciamos a no pulsado*/

    /* Botones S4, S5 y S6 */
    P2DIR &=~(BIT1|BIT2|BIT3);
    P2REN |= (BIT1|BIT2|BIT3);
    P2OUT |= (BIT1|BIT2|BIT3);

    /* Configuracion del zumbador */
    P2DIR |= BIT0; /* Lo habilitamos como SALIDA */
    P2OUT &=~ BIT0;/* Inicializamos a APAGADO */

    /* Vinculamos el pin del zumbador a la salida de la señal */
    P2SEL |= BIT0;
    P2SEL2 &=~ BIT0;

    /* Configuracion del timer */
    TA1CCR0=NUM; /* NUM = Ciclso que tiene que contar el timer */

    /* TASSEL_2 -> Fuente del timer es SMCLK */
    TA1CTL = TASSEL_2;

    /* OUTMOD_4 -> Modo de salida Toggle. Cada vez que la cuenta
     * llega al valor de TA1CCR0 hay un cambio de flanco.  */
    TA1CCTL0 ^= OUTMOD_4;
}
