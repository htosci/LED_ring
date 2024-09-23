#include <Arduino.h>
#include <FastLED.h>
#include <EncButton.h>
#include "VanLEDlib.h" // Моя библиотека

#define DATA_PIN 12     // пин ленты
#define NUM_LEDS 12      // кол-во светодиодов

#define IN_PIN 34       // пин потенциометра

#define ENC_PIN_1 14      // пин энкодера 01
#define ENC_PIN_2 27       // пин энкодера 02
#define ENC_PIN_BUT 13       // пин кнопки энкодера

#define TIMER_01 100     // дней*(24 часов в сутках)*(60 минут в часе)*(60 секунд в минуте)*(1000 миллисекунд в секунде)

CRGB leds[NUM_LEDS];    //  наш массив ленты
CRGBPalette16 currentPalette;   //  текущая палитра (рабочий вариант ленты)

unsigned long my_timer;   //  переменная таймера, максимально большой целочисленный тип (он же uint32_t)


void setup() {
  Serial.begin(115200);
  currentPalette = RainbowColors_p;

  my_timer = millis();   // "сбросить" таймер
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is assumed

//  fill_rainbow_circular(leds, NUM_LEDS, 0, false);

    FastLED.show();

}

void loop() {

  if (millis() - my_timer >= TIMER_01) { // обновление кадра
    static uint8_t pos = 250; //позиция диода
    my_timer = millis();   // "сбросить" таймер
    
    pos = (pos + 1) % 255; 
    Serial.print("poss = ");
    Serial.println(pos);
    
    //fill_palette_circular(leds, NUM_LEDS, 0, RainbowColors_p); //заливаем (масштабируя) ленту по политре

//    zoomRegion(leds, RainbowColors_p , pos, 2, 200 , NUM_LEDS);
//    fill_rainbow_circular(leds, NUM_LEDS, 0, false);
//    blurFade(leds, pos * NUM_LEDS / 255, 2, 96, NUM_LEDS);
    choseFromPal(leds, NUM_LEDS, RainbowColors_p, pos, LEFT, 0); 
    FastLED.show();
  }
  
  delay(10); // this speeds up the simulation
}