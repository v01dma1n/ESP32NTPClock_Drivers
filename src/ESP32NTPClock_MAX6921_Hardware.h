#ifndef ESP32NTPCLOCK_MAX6921_HARDWARE_H
#define ESP32NTPCLOCK_MAX6921_HARDWARE_H

#include <Arduino.h>
#include <SPI.h>

#define MAX6921_SPI_SPEED 1000000 // 1 MHz
#define MAX6921_DIGIT_COUNT 10

class DispDriverMAX6921_Hardware {
public:
    DispDriverMAX6921_Hardware(int sclkPin, int misoPin, int mosiPin, int ssPin, int blankPin,
                               const unsigned long* gridMap, int digitCount);
    ~DispDriverMAX6921_Hardware(); 

    void begin();
    void writeDigit(int digit, unsigned long segments);

private:
    void spiCmd(unsigned long data);

    int _blankPin;
    int _ssPin;
    SPIClass* _spi;

    const unsigned long* _gridMap; // To store the pointer to the VFD_GRIDS array
    int _digitCount;             // To store the VFD_DIGIT_COUNT
};

#endif // ESP32NTPCLOCK_MAX6921_HARDWARE_H