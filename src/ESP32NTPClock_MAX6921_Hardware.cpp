#include "ESP32NTPClock_MAX6921_Hardware.h"

DispDriverMAX6921_Hardware::DispDriverMAX6921_Hardware(int sclkPin, int misoPin, int mosiPin, int ssPin, int blankPin,
    const unsigned long* gridMap, int digitCount)
    : _blankPin(blankPin), _ssPin(ssPin),
      _gridMap(gridMap), _digitCount(digitCount) {
    // Use VSPI
    _spi = new SPIClass(VSPI);
    _spi->begin(sclkPin, misoPin, mosiPin, ssPin); 
}

DispDriverMAX6921_Hardware::~DispDriverMAX6921_Hardware() {
    if (_spi) {
        _spi->end();
        delete _spi;
    }
} 

void DispDriverMAX6921_Hardware::begin() {
    pinMode(_blankPin, OUTPUT);
    pinMode(_ssPin, OUTPUT);
    digitalWrite(_ssPin, HIGH);
    digitalWrite(_blankPin, HIGH); // Display blanked initially
}

void DispDriverMAX6921_Hardware::spiCmd(unsigned long data) {
    _spi->beginTransaction(SPISettings(MAX6921_SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_ssPin, LOW);
    _spi->transfer32(data); 
    digitalWrite(_ssPin, HIGH);
    _spi->endTransaction();
}

void DispDriverMAX6921_Hardware::writeDigit(int digit, unsigned long segments) {
    if (digit < 0 || digit >= _digitCount) return; 

    digitalWrite(_blankPin, HIGH); // Blank display
    spiCmd(_gridMap[digit] | segments); // Send data
    digitalWrite(_blankPin, LOW);  // Unblank display
}