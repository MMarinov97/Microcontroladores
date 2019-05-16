/* Cuando se presione el pulsador Sx se encenderá el correspondiente LEDx durante 1s.
 * Se usará el temporizador TA0
 * y el bloque 1 (TA0CCR1) en modo continuo,
 * con la señal de reloj SMCLK por defecto ~1.1MHz y
 * el divisor /8 */

#include <msp430g2533.h>
#define NUM 34375

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
                /* Habilitamos el timer
                 * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
                TACTL|=MC_2;

                /* Conmutamos el LED */
                P2OUT ^= BIT4;

                /* Ponemos el contador a 0 */
                int i = 0;
                while(i < 4){
                    /* Esperamos a que termine el temporizador */
                    while(TAR!=TACCR1);
                    /* Actualizamos el valor de TACCR1 */
                    TACCR1 = TAR + NUM;
                    /* Sumamos 1 al contador */
                    i++;
                }
                /* Conmutamos el LED de nuevo */
                P2OUT ^= BIT4;

                /* Apagamos el temporizador */
                TACTL |= MC_0;

                S3=0; /* Actualizamos el valor del pulsador a pulsado */
               }
        }else{
            S3=1; /* Actualizamos el valor del pulsador a no pulsado */
        }
        /* Pulsador S4 y LED 4 */
        if((P2IN & BIT1)==0 ){/* Comprobamos si S4 esta pulsado */
            if(S4!=0){  /* Y si su estado ha cambiado */
                /* Habilitamos el timer
                 * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
                TACTL|=MC_2;

                /* Conmutamos el LED */
                P2OUT |= BIT5;

                /* Ponemos el contador a 0 */
                int i = 0;
                while(i < 4){
                    /* Esperamos a que termine el temporizador */
                    while(TAR!=TACCR1);
                    /* Actualizamos el valor de TACCR1 */
                    TACCR1 = TAR + NUM;
                    /* Sumamos 1 al contador */
                    i++;
                }
                /* Conmutamos el LED de nuevo */
                P2OUT ^= BIT5;

                /* Apagamos el temporizador */
                TACTL |= MC_0;

                S4=0; /* Actualizamos el valor del pulsador a pulsado */
               }
        }else{
            S4=1; /* Actualizamos el valor del pulsador a no pulsado */
        }
         /* Pulsador 5 y LED 5 */
         if((P2IN & BIT2)==0 ){ /* Comprobamos si S5 esta pulsado */
            if(S5!=0){ /* Y si su estado ha cambiado */
                /* Habilitamos el timer
                 * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
                TACTL|=MC_2;

                /* Conmutamos el LED */
                P2OUT |= BIT6;

                /* Ponemos el contador a 0 */
                int i = 0;
                while(i < 4){
                    /* Esperamos a que termine el temporizador */
                    while(TAR!=TACCR1);
                    /* Actualizamos el valor de TACCR1 */
                    TACCR1 = TAR + NUM;
                    /* Sumamos 1 al contador */
                    i++;
                }
                /* Conmutamos el LED de nuevo */
                P2OUT ^= BIT6;

                /* Apagamos el temporizador */
                TA0CTL &= ~MC_2;

                S5=0; /* Actualizamos el valor del pulsador a pulsado */
            }
         }else{
             S5=1; /* Actualizamos el valor del pulsador a no pulsado */
         }
         /* Pulsador 6 y LED 6 */
         if((P2IN & BIT3)==0 ){/* Comprobamos si S5 esta pulsado */
             if(S6!=0){ /* Y si su estado ha cambiado */
                 /* Habilitamos el timer
                  * MC_2 -> Modo continuo, cuenta desde 0 hasta el valor maximo de TAR repetidamente (0xFFFF+1) */
                 TACTL|=MC_2;

                 /* Conmutamos el LED */
                 P2OUT ^= BIT7;

                 /* Ponemos el contador a 0 */
                 int i = 0;

                 while(i < 4){
                     /* Esperamos a que termine el temporizador */
                     while(TAR!=TACCR1);
                     /* Actualizamos el valor de TACCR1 */
                     TACCR1 = TAR + NUM;
                     /* Sumamos 1 al contador */
                     i++;
                 }

                 /* Conmutamos el LED de nuevo */
                 P2OUT ^= BIT7;

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

    /* Configuracion de los LEDs */

    P2SEL &=~ (BIT6|BIT7);
    P2SEL2 &=~ (BIT6|BIT7);

    P2DIR |= (BIT4|BIT5|BIT6|BIT7);/* Los habilitamos como SALIDA */
    P2OUT &=~ (BIT4|BIT5|BIT6|BIT7);/* Inicializamos a APAGADO */

    /* Configuracion del timer */
    TACCR1 = NUM; /* NUM = Ciclos que tiene que contar el timer */

    /* TASSEL_2 -> Fuente del timer es SMCLK
     * ID_3 -> Divisor del timer /8 */
    TACTL = TASSEL_2 + ID_3;
}
