#include "ESP32NTPClock_HCS12SS59T.h"

// VFD Controller Commands
#define VFD_DCRAM_WR            0x10
#define VFD_CGRAM_WR            0x20
#define VFD_ADRAM_WR            0x30
#define VFD_DUTY                0x50
#define VFD_NUMDIGIT            0x60
#define VFD_LIGHTS              0x70
#define VFD_LIGHTS_NORMAL       0x00

DispDriverHCS12SS59T::DispDriverHCS12SS59T(int displaySize, int resetPin, int csPin, int vdonPin) :
    _displaySize(displaySize > 12 ? 12 : displaySize), // Clamp max size to 12
    _resetPin(resetPin),
    _csPin(csPin),
    _vdonPin(vdonPin),
    _spiSettings(2000000, LSBFIRST, SPI_MODE3)
{
    _displayBuffer = new uint8_t[_displaySize]();
}

void DispDriverHCS12SS59T::begin() {
    pinMode(_resetPin, OUTPUT);
    pinMode(_csPin, OUTPUT);
    pinMode(_vdonPin, OUTPUT);

    digitalWrite(_resetPin, HIGH);
    digitalWrite(_csPin, HIGH);
    digitalWrite(_vdonPin, HIGH); // Supply OFF initially

    _supplyOn();
    SPI.begin();

    // Reset sequence
    digitalWrite(_resetPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_resetPin, HIGH);
    delayMicroseconds(2);

    // Initialize display controller
    _sendCmd(VFD_NUMDIGIT, _displaySize);
    setBrightness(8); // Default brightness
    _sendCmd(VFD_LIGHTS, VFD_LIGHTS_NORMAL);
    clear();
    writeDisplay();
}

void DispDriverHCS12SS59T::_supplyOn() {
    digitalWrite(_vdonPin, LOW); // Vdisp ON
    delay(1);
}

int DispDriverHCS12SS59T::getDisplaySize() {
    return _displaySize;
}

void DispDriverHCS12SS59T::setBrightness(uint8_t level) {
    if (level > 15) level = 15;
    _sendCmd(VFD_DUTY, level);
}

void DispDriverHCS12SS59T::clear() {
    for (int i = 0; i < _displaySize; i++) {
        _displayBuffer[i] = _getCode(' '); // Fill with blank character code
    }
}

uint8_t DispDriverHCS12SS59T::_getCode(char c) {
    // This character mapping is specific to the HCS-12SS59T
    if (c >= '@' && c <= '_')
        c -= 48;
    else if (c >= ' ' && c <= '?')
        c += 16;
    else if (c >= 'a' && c <= 'z')
        c -= 80;
    else
        c = 79; // '?' for unknown characters
    return c;
}

void DispDriverHCS12SS59T::setChar(int position, char character, bool dot) {
    if (position < 0 || position >= _displaySize) return;
    // This VFD's font map doesn't support dots, so we ignore the dot parameter.
    _displayBuffer[position] = _getCode(character);
}

void DispDriverHCS12SS59T::setSegments(int position, uint16_t mask) {
    // This VFD uses a character RAM, not direct segment control.
    // We can leave this function empty or try to map 7-segment masks to characters.
    // For now, we will set it to a blank space.
    setChar(position, ' ');
}

void DispDriverHCS12SS59T::_sendCmd(uint8_t cmd, uint8_t arg) {
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    delayMicroseconds(1);
    SPI.transfer(cmd | arg);
    delayMicroseconds(8);
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();
}

void DispDriverHCS12SS59T::writeDisplay() {
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    delayMicroseconds(1);
    
    // Command to write to Display RAM starting at address 0
    SPI.transfer(VFD_DCRAM_WR | 0);
    delayMicroseconds(8);

    // Write the buffer contents to the display RAM
    // The display is updated right-to-left, so we send the buffer in reverse.
    for (int i = _displaySize - 1; i >= 0; i--) {
        SPI.transfer(_displayBuffer[i]);
        delayMicroseconds(8);
    }

    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();
}
