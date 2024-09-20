#ifndef MYLIBRARY_H  // Защита от повторного включения
#define MYLIBRARY_H

#include <Arduino.h>
#include <FastLED.h>
#include <EncButton.h> // Подключаем нашу библиотеку
//#include "VanLEDlib.cpp" // Моя библиотека

//возвращает синусно-размытое значение от 255 ( при position=0 ) до 0 ( при position=range )
uint8_t cos8map(
    uint8_t position,   //отдаление от точки с максимальным значением
    uint8_t range) ;    //расстояние от точки с максимальным значением, где эначение равно нулю

//Затемнение по сторонам от centerPixel(без затемнения) на range пикселей с размытием
void blurFade(CRGB* leds, uint16_t centerPixel, uint8_t range, uint8_t maxFade, uint8_t num_leds) ; 

//Увеличение масштаба по сторонам от centerPixel(maxZoom) на range пикселей с плавным заполнением остального
void zoomRegion(    
    CRGB* leds, 
    CRGBPalette16 targetPalette, 
    uint16_t centerPixel, 
    uint8_t range, 
    uint8_t maxZoom,  //(0-255) максимальный зум * 1/8 ( 8 = 1:1 ; 16 = 2:1 ; 255 = (31+7/8):1 )
    uint8_t num_leds);
#endif