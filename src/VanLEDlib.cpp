#include "VanLEDlib.h" // Моя библиотека

//возвращает синусно-размытое значение от 255 ( при position=0 ) до 0 ( при position=range )
uint8_t cos8map(
    uint8_t position,   //отдаление от точки с максимальным значением
    uint8_t range) {    //расстояние от точки с максимальным значением, до точки, где эначение равно нулю
        uint8_t distance = position * 128 / range;
        uint8_t smuthedVal = cos8(distance);
    return smuthedVal;
}

//Затемнение по сторонам от centerPixel(без затемнения) на range пикселей с размытием
void blurFade(
    CRGB* leds,     //массив с лентой
    uint16_t centerPixel,       //позиция ценртального пикселя, с максимальной яркостью
    uint8_t range,      //расстояние от точки с максимальным значением, до точки, где эначение равно нулю
    uint8_t maxFade,        //макстимальное затемнение (0 = совсем ярко, 255 = темно совсем)
    uint8_t num_leds) {     //количество диодов в массиве

    uint16_t startIndex = (centerPixel + num_leds - range) % num_leds;
    uint16_t endIndex = (centerPixel + range) % num_leds;

    if (startIndex < endIndex) {
        if (0 < startIndex) {
            CRGBSet FadeReg1 = CRGBSet(leds, 0, startIndex-1);
            FadeReg1.nscale8_video(maxFade);
/*             Serial.print("FadeReg1 = 0  >>  ");
            Serial.println(startIndex-1); */
        }
        if (endIndex + 1 < num_leds) {
            CRGBSet FadeReg2 = CRGBSet(leds, endIndex + 1,num_leds);
            FadeReg2.nscale8_video(maxFade);
/*             Serial.print("  >>  FadeReg2 = 0  >>  ");
            Serial.print(endIndex + 1);
            Serial.print("  >>  ");
            Serial.println( num_leds); */
        }
    }else {
        if (endIndex + 1 < startIndex-1) {
            CRGBSet FadeReg = CRGBSet(leds, endIndex + 1 , startIndex-1);
             FadeReg.nscale8_video(maxFade);
/*            Serial.print("  >>  FadeReg = ");
            Serial.print( endIndex + 1 );
            Serial.print("  >>  ");
            Serial.println( startIndex-1); */
        } 
    }

    for (uint8_t i = 0; i <= range; i++){
        uint8_t fade = cos8map(i, range);
/*         Serial.print("cos8map(i, range) = ");
        Serial.print(fade);  */  
        fade = map8(fade, maxFade, 255);              

        uint16_t pixelIndex = (centerPixel + i) % num_leds;
        //leds[pixelIndex].maximizeBrightness();
        leds[pixelIndex].nscale8_video(fade);

/*         Serial.print("  >>  pixelIndex = ");
        Serial.print(pixelIndex);
        Serial.print("  >>  fade = ");
        Serial.println(fade); */

        pixelIndex = (centerPixel + num_leds - i) % num_leds;
        //leds[pixelIndex].maximizeBrightness();
        leds[pixelIndex].nscale8_video(fade);

/*         Serial.print("  >>  pixelIndex = ");
        Serial.print(pixelIndex);
        Serial.print("  >>  fade = ");
        Serial.println(fade); */
    }
}

//Увеличение масштаба по сторонам от centerPixel(maxZoom) на range пикселей с плавным заполнением остального
void zoomRegion(    
    CRGB* leds,     //массив с лентой
    CRGBPalette16 targetPalette,        //палитра по которой заливают
    uint16_t centerPixel,       //позиция ценртального пикселя, с максимальным масштабом
    uint8_t range,      //расстояние от точки с максимальным значением, до точки, где эначение равно нулю
    uint8_t maxZoom,        //(0-255) максимальный зум * 1/16 ( 8=2:1 ; 16 = 1:1 ; 32 = 2:1 ; 255 = (15+15/16):1 )
    uint8_t num_leds) {     //количество диодов в массиве

    uint16_t startIndex = (centerPixel + num_leds - range) % num_leds;
    uint16_t endIndex = (centerPixel + range) % num_leds;
    uint8_t mapLedInPallette[num_leds];
    uint16_t mapSum = 0;
    uint16_t foolLedDist = 0;

        Serial.print("startIndex = ");
        Serial.println(startIndex);

        Serial.print("endIndex = ");
        Serial.println(endIndex);


    Serial.println("For - 01");
    for (uint8_t i = 0; i <= range; i++){
        uint8_t scale = cos8map( i, range);
        scale = map8(scale, 16, maxZoom);
        uint8_t halfLedDist = 8*16/scale;

        uint16_t pixelIndex = (centerPixel + i) % num_leds;
        mapLedInPallette[pixelIndex] = halfLedDist;
        Serial.print("mapLedInPallette[");
        Serial.print(pixelIndex);
        Serial.print("] = ");
        Serial.println(mapLedInPallette[pixelIndex]);

        pixelIndex = (centerPixel + num_leds - i) % num_leds;
        mapLedInPallette[pixelIndex] = halfLedDist;
        Serial.print("mapLedInPallette[");
        Serial.print(pixelIndex);
        Serial.print("] = ");
        Serial.print(mapLedInPallette[pixelIndex]);

        mapSum += halfLedDist*4;
        Serial.print(";  >>  mapSum = ");
        Serial.println(mapSum);
    }
        mapSum -= 8*16/maxZoom;

        
    delay(100);
    Serial.println("For - 02");
    for (uint16_t i = (endIndex + 1) % num_leds; i != startIndex; i = ( i + 1 ) % num_leds){
        uint8_t scale = 16;
        uint8_t halfLedDist = 8 * 16 / scale;
        mapLedInPallette[i] = halfLedDist;
        mapSum += halfLedDist * 2;

        Serial.print("mapLedInPallette[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.print(mapLedInPallette[i]);
        Serial.print(";  >>  mapSum = ");
        Serial.println(mapSum);
    }


    delay(100);
    Serial.println("For - 03");
    foolLedDist += map(centerPixel,0,num_leds,0,mapSum);
    for (uint16_t i = 0 ; i < num_leds ; i++){
        uint16_t pixIndex = ( i + centerPixel ) % num_leds;
        uint16_t nextIndex = ( i + 1 + centerPixel ) % num_leds;
        uint8_t mapLedDist = map(foolLedDist,0,mapSum,0,255);
        Serial.print("pixIndex = ");
        Serial.print(pixIndex);
        Serial.print("  >>  foolLedDist = ");
        Serial.print(foolLedDist);

        foolLedDist = (foolLedDist + mapLedInPallette[ pixIndex] + mapLedInPallette[ nextIndex]) % mapSum ;

        mapLedInPallette[pixIndex] = mapLedDist;
        
        Serial.print("  >>  mapLedInPallette[");
        Serial.print(pixIndex);
        Serial.print("] = ");
        Serial.println(mapLedInPallette[pixIndex]);
    }
    delay(100);

        map_data_into_colors_through_palette(mapLedInPallette, num_leds, leds,targetPalette);
}


//Выбор из палитры с растущей полоской от нулевого пикселя
void choseFromPal(    
    CRGB* leds,     //массив с лентой
    uint8_t num_leds,     //количество диодов в массиве    
    CRGBPalette16 targetPalette,        //палитра из которой выбираем
    uint8_t val,        //(0-255) выбранное значение из палитры
    mode grow_mode,       // (RIGHT, LEFT, LEFT_TAIL, RIGHT_TAIL) метод роста, LEFT - голова ростёт на лево от argetPixel )
    uint16_t targetPixel){      //номер диода на ленте, который будет началом       

    uint8_t scale = (255 / num_leds); 
    uint16_t tailLenght = (val * num_leds / 255); //3

    uint16_t headeIndex = 0;
    int8_t lightDirection = 1;
    int8_t tailDirection = 1;

    switch (grow_mode)
    {
    case RIGHT:
        lightDirection = 1;
        tailDirection = 0;
        headeIndex = (targetPixel + tailLenght * lightDirection + num_leds) % num_leds;
        break;
    case LEFT:
        lightDirection = -1;
        tailDirection = 0;
        headeIndex = (targetPixel + tailLenght * lightDirection + num_leds) % num_leds;
        break;
    case RIGHT_TALE:
        lightDirection = -1;
        tailDirection = 1;
        headeIndex = targetPixel;
        break;
    case LEFT_TAIL:
        lightDirection = 1;
        tailDirection = -1;
        headeIndex = targetPixel ;
        break;
    default:
        break;
    }

    fill_solid(leds, num_leds, CRGB::Black);

    for(uint8_t i = 0; i < tailLenght ; i++ ){
        uint8_t iIndex = (lightDirection * i  + targetPixel + num_leds + tailDirection * tailLenght) % num_leds;

        Serial.print("i = ");
        Serial.print(i);
        Serial.print(" > iIndex = ");
        Serial.print(iIndex);
        Serial.print(" > color = ");
        Serial.println( i * 255 / num_leds);

        leds[iIndex] = ColorFromPalette(targetPalette, ( i * 255 / num_leds) );
    }
    leds[headeIndex] = ColorFromPalette(targetPalette, val);

    Serial.print("val = ");
    Serial.print(val);
    Serial.print("  >>  headeIndex = ");
    Serial.print(headeIndex);
    Serial.print("  >>  tailLenght = ");
    Serial.print(tailLenght);
    Serial.print("  >>  mode_my = ");
    Serial.println(grow_mode);
    }