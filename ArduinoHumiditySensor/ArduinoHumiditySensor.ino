#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>


//Programa: Arduino e DHT11 controle de rele por umidita
//Autor: Arduino e Cia

#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

int posicao = 0;
int temp_max = 50;

#define pino_aumenta 5
#define pino_diminui 4
#define pino_rele1 7 
#define pino_rele2 8
#define pino_DHT A0
#define DHTTYPE DHT11

DHT dht(pino_DHT, DHTTYPE);

int umidita;
int aumenta, diminui;

unsigned long previousMillis = 0;
//Intervalo de medicao umidita
const long interval = 2000;

void draw()
{
  //Retangulo umidita atual
  u8g.drawRFrame(0, 17, 84, 46, 2);
  u8g.setFont(u8g_font_fub20);
  //Atualiza a umidita no display
  u8g.setPrintPos(13, 50);
  u8g.print(umidita);
  //Circulo grau
  u8g.drawCircle(51, 33, 3);
  u8g.drawStr( 56, 50, "%");

  //Box superior amarelo
  u8g.setFont(u8g_font_8x13B);
  u8g.drawRBox(0, 0, 127, 16, 2);
  u8g.setColorIndex(0);
  u8g.drawStr( 34, 13, "UMIDITA'");  // Default 21 13
  u8g.setColorIndex(1);

  //box umidita maxima
  u8g.drawRFrame(86, 17, 42, 46, 2);
  if (temp_max <= umidita)
  {
    //umidita maior do que a temp. maxima
    u8g.drawRBox(86, 17, 42, 22, 2);
    u8g.setColorIndex(0);
    u8g.drawStr(99, 33, "ON");  // default 96 33
    u8g.setColorIndex(1);
    //Aciona saida do rele
    digitalWrite(pino_rele1, LOW);
    digitalWrite(pino_rele2, LOW);
  }
  else
  {
    //umidita menor do que a temp. maxima
    u8g.drawRFrame(86, 17, 42, 22, 2);
    u8g.drawStr(99, 33, "ON");  // default 96 33
    //Desliga saida do rele
    digitalWrite(pino_rele1, HIGH);
    digitalWrite(pino_rele2, HIGH);
  }
  //Atualiza na tela o valor da temp. maxima
  u8g.setPrintPos(100, 55);
  u8g.print(temp_max);
  u8g.drawCircle(120, 47, 2);
  u8g.setColorIndex(1);
}

void setup(void)
{
  Serial.begin(9600);
  pinMode(pino_rele1, OUTPUT);
  pinMode(pino_rele2, OUTPUT);
  pinMode(pino_aumenta, INPUT);
  pinMode(pino_diminui, INPUT);
  dht.begin();
}

void loop(void)
{
  Serial.println(dht.readHumidity());
  unsigned long currentMillis = millis();
  //Timer para ler o valor da umidita
  if (currentMillis - previousMillis >= interval)
  {
    umidita = dht.readHumidity();
    previousMillis = currentMillis;
  }

  //Testa botao aumenta umidita
  aumenta = digitalRead(pino_aumenta);
  if (aumenta == 1)
  {temp_max++;}
  while (digitalRead(pino_aumenta) == 1)
  {delay(10);}

  //testa botao diminui umidita
  diminui = digitalRead(pino_diminui);
  if (diminui == 1)
  {temp_max--;}
  while (digitalRead(pino_diminui) == 1)
  {delay(10);}

  u8g.firstPage();
  // Chama a rotina de desenho na tela
  do
  {
    draw();
  }
  while ( u8g.nextPage() );
  delay(50);
}
