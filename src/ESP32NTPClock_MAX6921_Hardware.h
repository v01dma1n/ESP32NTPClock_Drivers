#ifndef ESP32NTPCLOCK_MAX6921_HARDWARE_H
#define ESP32NTPCLOCK_MAX6921_HARDWARE_H

#include <Arduino.h>
#include <SPI.h>

#define MAX6921_SPI_SPEED 1000000 // 1 MHz
#define MAX6921_DIGIT_COUNT 10

class DispDriverMAX6921_Hardware {
public:
    DispDriverMAX6921_Hardware(int sclkPin, int misoPin, int mosiPin, int ssPin, int blankPin);
    ~DispDriverMAX6921_Hardware(); 

    void begin();
    void writeDigit(int digit, unsigned long segments);

private:
    void spiCmd(unsigned long data);

    int _blankPin;
    int _ssPin;
    SPIClass* _spi;
};

#endif // ESP32NTPCLOCK_MAX6921_HARDWARE_H