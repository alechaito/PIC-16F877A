/* 
// Universidade Federal de Santa Catarina - Microcontroladores
// Desenvolvedor: Ale Chaito
// Data: 18/11/2018
// Objetivo: Mesa de bilhar microcontrolada
*/ 
#include <main.h>

int _BALLS;
int _FLAG_INIT;

// Interrompendo programa para verificar o sensor de luz
#int_timer1
void check_light() { 
      int digital_value;
      float analogic_value;
      set_adc_channel(0);
      delay_us(10);              //tempo necess?rio para o hardware ficar pronto
      digital_value = read_adc();
      analogic_value = (5*digital_value)/255.0;
      if(analogic_value < 0.5) {
         output_High(NIGHT_LED);
      }
      else {
         output_Low(NIGHT_LED);
      }
      set_timer1(0);             
}

#int_ext
// FUNCAO PARA DECREMENTAR A QUANTIA DE BOLAS INICIAS E MOSTRAR NO DISPLAY
void player_point() {
      if(_FLAG_INIT == 1) { //Evitar numero de bolas negativo
         output_High(BUZZER);
         delay_ms(150);
         output_Low(BUZZER);
         _BALLS = _BALLS - 1;
         lcd_gotoxy(1,1);
         printf(lcd_putc, "\f Snooker ON");
         lcd_gotoxy(2,2);
         printf(lcd_putc, "Total: %d balls", _BALLS);
         delay_ms(300);
         if(_BALLS == 0) {
            delay_ms(1000);
            lcd_gotoxy(1,1);
            printf(lcd_putc, "\f Snooker ON");
            lcd_gotoxy(2,2);
            printf(lcd_putc, "Finished Game");
            _FLAG_INIT = 0;
         }
      }
   }

// Rotina de processamento
//1-Bolas estao todas guardadas
//2-Aperta o botao de start da partida
//3-Inicia o placar em 0
//4-Liga o motor para jogar as bolinhas
//5-Apita pra dizer que a partida ta pronta pra iniciar
//6-Fica aguardando alguma bola cair na capaca
//7-Quando cair ativa a interrupcao, aciona o buzzer
//8-Timer fica verificando o check_adc para ligar ou nao a luz


void main() {
      _BALLS = 5;
      _FLAG_INIT = 0;
      //Setup ADC 
      setup_adc(ADC_CLOCK_INTERNAL);
      set_adc_channel(0);
      setup_adc_ports(AN0);
      
      // Setup interrupts
      enable_interrupts(global);
      enable_interrupts(int_ext);
      ext_int_edge(H_TO_L);
      
      lcd_init();
      Port_b_pullups(true);
      
      //Timer de interrupcao
      setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
      set_timer1(0);
      enable_interrupts(int_timer1);
      
      lcd_gotoxy(1,1);
      printf(lcd_putc, "Snooker ON");
      lcd_gotoxy(1,2);
      printf(lcd_putc, "Play press start");
      
      //setup_wdt(WDT_2S);
      
      
      while(TRUE) {
            //restart_wdt();
            // O sistema so inicia se estiver com moeda e flag de inicio 0 para previnir 2 jogos ao mesmo tempo
            if(input(START) == 0 && input(COIN) == 0 && _FLAG_INIT == 0) {
               setup_game();
            }
            // Verificacao dos inputs da geladeira
            if(input(GEL_START) == 1) {
               output_low(GEL_MOTOR);
               output_low(GEL_LIGHT);
            }
            if(input(GEL_START) == 0) {
               output_high(GEL_MOTOR);
               output_high(GEL_LIGHT);
            }
      
      }
}

// Prepara o jogo de bilhar para ser iniciado
void setup_game() {
      open_drawer();
      //-----------------------
      output_High(BUZZER);
      delay_ms(150);
      output_Low(BUZZER);
      //----------------------
      _FLAG_INIT = 1; // Setando flag de inicio de partida pro sistema
      lcd_gotoxy(1,1);
      printf(lcd_putc, "\f Snooker ON");
      lcd_gotoxy(2,2);
      printf(lcd_putc, "Total: 5 balls");
}

void open_drawer() {
   lcd_gotoxy(1,1);
   printf(lcd_putc, "\f Snooker ON");
   lcd_gotoxy(1,2);
   printf(lcd_putc, "Wait the drawer");
   
   output_low(M3);
   output_high(M0);
   delay_ms(1000);
   
   output_low(M0);
   output_high(M1);
   delay_ms(1000);
   
   output_low(M1);
   output_high(M2);
   delay_ms(1000);
   
   output_low(M2);
   output_high(M3);
   delay_ms(1000);
   
   lcd_gotoxy(1,1);
   printf(lcd_putc, "\f Snooker ON");
   lcd_gotoxy(1,2);
   printf(lcd_putc, "Press start");
   
   // Aguarda o botao de start pra iniciar
   while(input(START) == 1);
}

