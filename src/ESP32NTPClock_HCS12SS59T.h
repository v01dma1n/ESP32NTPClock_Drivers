#ifndef ESP32NTPCLOCK_HCS12SS59T_H
#define ESP32NTPCLOCK_HCS12SS59T_H

#include <ESP32NTPClock.h>
#include <SPI.h>

class DispDriverHCS12SS59T : public IDisplayDriver {
public:
    DispDriverHCS12SS59T(int displaySize, int resetPin, int csPin, int vdonPin);

    // IDisplayDriver interface implementation
    void begin() override;
    int getDisplaySize() override;
    void setBrightness(uint8_t level) override;
    void clear() override;
    void setChar(int position, char character, bool dot = false) override;
    void setSegments(int position, uint16_t mask) override;
    void setDot(int position, bool on) override;
    void writeDisplay() override;

private:
    void _supplyOn();
    void _sendCmd(uint8_t cmd, uint8_t arg);
    uint8_t _getCode(char c);

    int _displaySize;
    int _resetPin;
    int _csPin;
    int _vdonPin;

    SPISettings _spiSettings;
    uint8_t* _displayBuffer; // Dynamically allocated buffer
};

#endif // ESP32NTPCLOCK_HCS12SS59T_H
