#ifndef ESP32NTPCLOCK_MAX6921_H
#define ESP32NTPCLOCK_MAX6921_H

#include <ESP32NTPClock.h>
#include <SPI.h>

class DispDriverMAX6921 : public IDisplayDriver {
public:
    DispDriverMAX6921(int displaySize);
    ~DispDriverMAX6921(); 

    // IDisplayDriver interface implementation
    void begin() override;
    int getDisplaySize() override;
    void setBrightness(uint8_t level) override;
    void clear() override;
    void setChar(int position, char character, bool dot = false) override;
    void setSegments(int position, uint16_t mask) override;
    void writeDisplay() override; // writeDisplay() is now a NO-OP (does nothing)
    void writeNextDigit() override; // function called by the high-priority displayTask
    bool needsContinuousUpdate() const override;

    unsigned long mapAsciiToSegment(char ascii_char, bool dot) override;
    void setBuffer(const std::vector<unsigned long>& newBuffer) override;
    void getFrameData(unsigned long* buffer) override;

private:
    void spiCmd(unsigned long data);

    int _displaySize;
    unsigned long* _displayBuffer; // Dynamically allocated buffer
   
};

#endif // ESP32NTPCLOCK_MAX6921_H
