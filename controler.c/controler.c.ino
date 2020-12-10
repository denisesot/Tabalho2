#include "U8glib.h" // biblioteca para uso do painel OLED
#include <Thermistor.h> // biblioteca para uso do sensor de temperatura

Thermistor temp(0); // define o sensor de temperatura na porta analogica A0

 int temperature = 0;
 int flag = 0;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC
 
void draw() 
{
  //Comandos graficos para o display devem ser colocados aqui
  
  u8g.setFont(u8g_font_fub30);
  char buf[2];
  sprintf(buf, "%d", temperature);
  u8g.drawStr( 10, 57, buf);   //mostra a temperatura 
  u8g.drawCircle(62,30,3);  //desenha o circulo de grau 
  u8g.drawStr( 64, 57, "C");  //medida da temperatura (Celsius)
  
  u8g.drawRFrame(0,18, 128, 46, 4); //moldura
}

ISR(TIMER1_COMPA_vect){
  flag = true;
}

void setup(void) 
{
  pinMode(7,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }

   //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}
 
void loop(void) 
{
  u8g.firstPage();
  if(flag){
    temperature = temp.getTemp(); 
    if (temperature < 22){
      digitalWrite(7, LOW);   
      digitalWrite(LED_BUILTIN, LOW);   
    } else if (temperature > 24){
      digitalWrite(7,HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    flag = false;
  }
 
  do {
    draw();  
  } while( u8g.nextPage() );
   
}
