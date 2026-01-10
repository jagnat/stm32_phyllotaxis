## Phyllotaxis: Audio Responsive LED Art Project

This is the code for an audio-reactive neopixel light board art project I designed and built, in a double-spiral fibonacci phyllotaxis shape.

### Details

The controller is a WeAct Studio STM32 Black Pill with an STM32F411CEU6 MCU running at 96MHz, driving 89 LEDs via SPI. I achieve audio reactivity using an I2S MEMS microphone, the INMP441. I utilize both some HAL and LL libraries.

LEDs are driven using SPI at 3MHz with DMA. Each neopixel bit is 3 SPI bits (neopixel bit 0 = SPI 100, neopixel bit 1 = SPI 110). Driving neopixels at effectively 1MHz rather than 800KHz is out of spec, but in practice it works.

I use circular DMA to capture audio from I2S at 32KHz and use the ARM CMSIS DSP library to do FFT analysis. I process the result into logarithmic bands, and apply some automatic gain control. The bands are then used in LED sketches to make LEDs behave dynamically and respond to music.

I have a few LUTs used for programming the LEDs, the most important of which is a mapping of physical LED positions to floating point coordinates in the unit circle. This allows me to program patterns for the LEDs in a shader-esque manner.

This codebase uses Cube MX to generate a makefile-based project with the required peripheral config. I use the makefile and a manual command to flash code. I debug through Cube IDE.

### Pin Configuration
- PA7 - SPI1 MOSI LED Output
- PB10 - I2S Clock
- PB12 - I2S Word Select
- PB15 - I2S Serial Data
- PB1 - Button input
- PA4 - Analog pot input (unused currently)
- PC13 - Black Pill onboard LED (turned on in fault handler)
- PA5 - SPI1 SCK (unneeded and unused)

### Resources
Non-exhaustive list of resources I found invaluable during this project.
- [Logic Level Shifters for Driving LED Strips](https://electricfiredesign.com/2021/03/12/logic-level-shifters-for-driving-led-strips/)
- [A USB Microphone built with an INMP441 and an STM32](https://github.com/andysworkshop/usb-microphone)
- [Extracting 24-bit Audio from INMP441](https://community.st.com/t5/others-stm32-mcus-related/extracting-24-bit-audio-from-inmp441-very-low-amplitude-and-high/td-p/787339)
- [Adafruit NeoPixel Überguide](https://learn.adafruit.com/adafruit-neopixel-uberguide/the-magic-of-neopixels)
- [FastLED HSV to RGB](https://github.com/FastLED/FastLED/blob/735ea6ca8187e7b041522145daf70f9d45955d81/src/hsv2rgb.cpp)