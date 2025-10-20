#include "ESP32NTPClock_MAX6921_Hardware.h"

// Grid masks for 10-digit display
// static const unsigned long GRIDS[] = {
//     0x00080000, // Digit 0
//     0x00100000, // Digit 1
//     0x00200000, // Digit 2
//     0x00400000, // Digit 3
//     0x00800000, // Digit 4
//     0x01000000, // Digit 5
//     0x02000000, // Digit 6
//     0x04000000, // Digit 7
//     0x08000000, // Digit 8
//     0x10000000  // Digit 9
// };

// Grids (Digits)
static const unsigned long GRIDS[] = {
    0b00000000010000000000, // GRD_01
    0b00000000100000000000, // GRD_02
    0b00000010000000000000, // GRD_03
    0b00010000000000000000, // GRD_04
    0b10000000000000000000, // GRD_05
    0b00000000000000000010, // GRD_06
    0b00000000000000000100, // GRD_07
    0b00000000000000100000, // GRD_08
    0b00000000000100000000, // GRD_09
    0b00000000001000000000  // GRD_10
};


DispDriverMAX6921_Hardware::DispDriverMAX6921_Hardware(int sclkPin, int misoPin, int mosiPin, int ssPin, int blankPin)
    : _blankPin(blankPin), _ssPin(ssPin) {
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
    _spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_ssPin, LOW);
    _spi->transfer32(data); 
    digitalWrite(_ssPin, HIGH);
    _spi->endTransaction();
}

void DispDriverMAX6921_Hardware::writeDigit(int digit, unsigned long segments) {
    if (digit < 0 || digit >= 10) return; // Guard clause

    digitalWrite(_blankPin, HIGH); // Blank display
    spiCmd(GRIDS[digit] | segments); // Send data
    digitalWrite(_blankPin, LOW);  // Unblank display
}