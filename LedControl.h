#include <Adafruit_NeoPixel.h>
#include <math.h>

#define LED_COUNT 48
#define LED_PIN 4

int BRIGHTNESS = 50;
uint32_t pixels[LED_COUNT];
uint32_t prevBuffer[LED_COUNT];
uint32_t nextBuffer[LED_COUNT];

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ400);

void setPixel(int index, uint32_t color) {
    uint8_t redChannel = (uint8_t)((color & 0x00FF0000) >> 16);
    uint8_t blueChannel = (uint8_t)((color & 0x0000FF00) >> 8);
    uint8_t greenChannel = (uint8_t)(color & 0x000000FF);
    strip.setPixelColor(index, redChannel, blueChannel, greenChannel);
}

void draw() {
    for(int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, pixels[i]);
    }
    strip.show();
}

void initialize() {
    strip.begin();
    strip.show();
    strip.setBrightness(BRIGHTNESS);
    pixels[0] = 0xFF0000;
    pixels[1] = 0xFF0000;
    pixels[2] = 0xFF0000;
    draw();
}

int calculateDistance(float percent) {
    return (int)roundf(percent * LED_COUNT) % LED_COUNT;
}

void advance(int distance)
{
    if (distance <= 0) {
        return;
    }

    for(int i = LED_COUNT - distance; i < LED_COUNT; i++)
    {
        int idx = i - LED_COUNT + distance;
        prevBuffer[idx] = pixels[i];
    }

    for(int i = 0; i < LED_COUNT; i += distance)
    {
        int jIdx = 0;
        while (jIdx < distance && i + jIdx < LED_COUNT)
        {
            nextBuffer[jIdx] = pixels[i + jIdx];
            int nextPixelIndex = i + jIdx;
            uint32_t nextColor = prevBuffer[jIdx];
            pixels[nextPixelIndex] = nextColor;
            strip.setPixelColor(i, pixels[i]);
            jIdx++;
        }

        for(int j = 0; j < distance; j++) {
        prevBuffer[j] = nextBuffer[j];
        }
    }
    strip.show();
}

