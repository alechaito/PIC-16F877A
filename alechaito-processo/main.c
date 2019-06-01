/* 
// Universidade Federal de Santa Catarina - Microcontroladores
// Desenvolvedor: Ale Chaito
// Data: 04/11/2018
// Objetivo: Processo Industrial com misturador, tanque e bomba
// Notas: Timers inconsistentes...
*/ 

#include <main.h>

int flag_timer;

#int_timer1
void routine_t1 () {
   //Tratamento da finalizacao do processo, encerrando e reinicializando status
   flag_timer = flag_timer + 1;
   // Encerro o processo
   if(flag_timer == 20) {
      output_High(PIN_D1); // Valvul = PIN D1
      output_Low(PIN_D2); // Misturador = PIN D2 
   }
   else {
      // Carrego novamente o timer
      set_timer1(3036);
   }
}

#int_ext
void routine_ext() {
   // Tratamento do sensor de temperatura que atingiu 40 graus
   flag_timer = 0;
   output_High(PIN_D2); // Misturador = PIN D2
   output_Low(PIN_D7); // Ligando resistencia
   //Ativo o contador de 40 segundos para aguardar o misturador
   //Reinicio o processo com uma interrupcao
   // Timer1
   // 4Mhz/4 = 1Mhz
   // 1Mhz/8 = 125 khz 
   // 125.000 pulsos significa 1s
   // Inicia o timer em 34286 (65.536-31.250)
   // Se estourar uma vez passaram 0.5 segundos
   //setup_timer_1(RTCC_DIV_1);
   setup_timer_1(T1_INTERNAL | RTCC_DIV_8);
   set_timer1(3036);
   enable_interrupts(int_timer1);
   // Fico aqui at� estourar 20 vezes o timer significando 20 * 0.5 segundos = 10 segundos de mistura
   while(flag_timer < 20);

}

void main() {
   // Ativando interrupcoes
   enable_interrupts(global);
   enable_interrupts(int_ext);
   ext_int_edge(H_TO_L);
   
   // Declarando variaveis
   int valvula, bomba, resist, flag_cheio;
   int1 sn;
   
   // Inicializando
   bomba = PIN_D0;
   valvula = PIN_D1;
   resist = PIN_D7;
   sn = input(pin_b1);
   flag_cheio = 0;
   
   Port_b_pullups(true);
   while(TRUE) {
      // Representaremos o n�vel do tanque baseado em apenas um sensor
      // BIT: 0 - sensor ativado (tanque cheio) | 1 - sensor desativado (tanque abaixo do n�vel)
      sn = input(pin_b1);
      if(sn == 1) {
         output_High(bomba);
         while(input(pin_b1) == 1);
         flag_cheio = 1;
      }
      
      // Tanque cheio tratamento, inicio do processo.
      if(flag_cheio == 1) {
         output_Low(bomba); 
         output_High(resist); // Ligando resistencia
         delay_ms(100);
         flag_cheio == 0;
      }
      
   }


}
