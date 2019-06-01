#include <16F877A.h>
#FUSES XT, NOWDT, NOPUT, NOBROWNOUT, NOLVP, NOCPD, NOWRT
#use delay(clock=4000000)

void main() {
    int16 temp;
    //int1 flag = 0;
    Port_b_pullups(true);
    temp = 2000;
    BTN = input(pin_b0);
    SN = input(pin_b1);

    //Inicializando valvulas
    // Fechando todas
    output_Low(PIN_D0);
    output_Low(PIN_D1);
    output_Low(PIN_D2);
    output_Low(PIN_D3);

    while(true) {
        //Se o botao foi apertado e o Sensor de Nivel estivel em 0 libera Y1 e Y2
        if(BTN == 0 && SN == 1) {
            output_High(PIN_D0);
            output_High(PIN_D1);
            //delay_ms(t2);
            //Verifica se o sensor de nivel foi acionado
            if(SN == 0) {
                //Se sim eu fecho as valvulas Y1 e Y2
                output_Low(PIN_D0);
                output_Low(PIN_D1);
                // Mantenho Y3 Fechado
                // Ligo o motor M1
                output_High(PIN_D2);
                // Deixo bater por 2 minutos
                delay_ms(temp);
                // Abro as valvulas Y3
                output_High(PIN_D3);
                delay_ms(temp);
            }
        }
    }
}
