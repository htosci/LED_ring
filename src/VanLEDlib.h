#ifndef MYLIBRARY_H  // Защита от повторного включения
#define MYLIBRARY_H

#include <Arduino.h>
#include <FastLED.h>
#include <EncButton.h> // Подключаем нашу библиотеку
//"VanLEDlib.cpp" библиотека

//возвращает синусно-размытое значение от 255 ( при position=0 ) до 0 ( при position=range )
uint8_t cos8map(
    uint8_t position,   //отдаление от точки с максимальным значением
    uint8_t range) ;    //расстояние от точки с максимальным значением, где эначение равно нулю


//Затемнение по сторонам от centerPixel(без затемнения) на range пикселей с размытием
void blurFade(
    CRGB* leds,     //массив с лентой
    uint16_t centerPixel,       //позиция ценртального пикселя, с максимальной яркостью
    uint8_t range,      //расстояние от точки с максимальным значением, до точки, где эначение равно нулю
    uint8_t maxFade,        //макстимальное затемнение (0 = совсем ярко, 255 = темно совсем)
    uint8_t num_leds) ;     //количество диодов в массиве


//Увеличение масштаба по сторонам от centerPixel(maxZoom) на range пикселей с плавным заполнением остального
void zoomRegion(    
    CRGB* leds,     //массив с лентой
    CRGBPalette16 targetPalette,        //палитра по которой заливают
    uint16_t centerPixel,       //позиция ценртального пикселя, с максимальным масштабом
    uint8_t range,      //расстояние от точки с максимальным значением, до точки, где эначение равно нулю
    uint8_t maxZoom,        //(0-255) максимальный зум * 1/16 ( 8=2:1 ; 16 = 1:1 ; 32 = 2:1 ; 255 = (15+15/16):1 )
    uint8_t num_leds);     //количество диодов в массиве
#endif

enum  mode{RIGHT, LEFT, LEFT_TAIL, RIGHT_TALE } ;

//Выбор из палитры с растущей полоской от нулевого пикселя
void choseFromPal(    
    CRGB* leds,     //массив с лентой
    uint8_t num_leds,     //количество диодов в массиве    
    CRGBPalette16 targetPalette,        //палитра из которой выбираем
    uint8_t val,        //(0-255) выбранное значение из палитры
    mode grow_mode,       // (RIGHT, LEFT, LEFT_TAIL, RIGHT_TAIL) метод роста, LEFT - голова ростёт по часовой стрелке от argetPixel )
    uint16_t targetPixel = 0);      //номер диода на ленте, который будет началом    