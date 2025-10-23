#include "ESP32NTPClock_MAX6921.h"
#include "enc_debug.h"

// --- Class Implementation ---

DispDriverMAX6921::DispDriverMAX6921(int displaySize, IFont& fontProvider, const SevenSegmentBitmaskMap& segmentMap)
    : _displaySize(displaySize),
      _fontProvider(fontProvider),
      _segmentMap(segmentMap)
{
    _displayBuffer = new unsigned long[_displaySize]();
}

DispDriverMAX6921::~DispDriverMAX6921() {
    delete[] _displayBuffer;
}

void DispDriverMAX6921::begin() {
    clear();
}

int DispDriverMAX6921::getDisplaySize() {
    return _displaySize;
}

void DispDriverMAX6921::setBrightness(uint8_t level) {
    // MAX6921 doesn't have brightness control, but we must implement the function.
}

void DispDriverMAX6921::clear() {
    for (int i = 0; i < _displaySize; i++) {
        _displayBuffer[i] = 0;
    }
}

unsigned long DispDriverMAX6921::mapGenericMaskToHardware(uint8_t mask, bool dot) const {
    unsigned long hw_mask = 0;
    // Use the injected _segmentMap [cite: 617]
    if (mask & GENERIC_SEG_A) hw_mask |= _segmentMap.segA;
    if (mask & GENERIC_SEG_B) hw_mask |= _segmentMap.segB;
    if (mask & GENERIC_SEG_C) hw_mask |= _segmentMap.segC;
    if (mask & GENERIC_SEG_D) hw_mask |= _segmentMap.segD;
    if (mask & GENERIC_SEG_E) hw_mask |= _segmentMap.segE;
    if (mask & GENERIC_SEG_F) hw_mask |= _segmentMap.segF;
    if (mask & GENERIC_SEG_G) hw_mask |= _segmentMap.segG;
    if (dot || (mask & GENERIC_SEG_DOT)) hw_mask |= _segmentMap.segDot;
    return hw_mask;
}

unsigned long DispDriverMAX6921::mapAsciiToSegment(char ascii_char, bool dot) {
    // 1. Get generic 8-bit mask from the injected font provider
    uint8_t genericMask = _fontProvider.getSegmentMask(ascii_char);
    
    // 2. Map generic mask to this driver's specific hardware mask
    return mapGenericMaskToHardware(genericMask, dot);
}

void DispDriverMAX6921::setChar(int position, char character, bool dot) {
if (position < 0 || position >= _displaySize) return; 
    _displayBuffer[position] = mapAsciiToSegment(character, dot);
}

void DispDriverMAX6921::setBuffer(const std::vector<unsigned long>& newBuffer) {
    int sizeToCopy = std::min((int)newBuffer.size(), _displaySize);
    for (int i = 0; i < sizeToCopy; ++i) {
        _displayBuffer[i] = newBuffer[i];
    }
}

void DispDriverMAX6921::setSegments(int position, uint16_t mask) {
if (position < 0 || position >= _displaySize) return; 
    // The incoming mask is already a generic 0b(dot)gfedcba mask
    _displayBuffer[position] = mapGenericMaskToHardware((uint8_t)mask, false);
}

/**
 * @brief This function is now a NO-OP.
 * It is called by the DisplayManager but does nothing, as the
 * displayTask is now responsible for multiplexing.
 */
void DispDriverMAX6921::writeDisplay() {
    // Do nothing.
}

void DispDriverMAX6921::getFrameData(unsigned long* buffer) {
    if (buffer != nullptr && _displayBuffer != nullptr) {
        memcpy(buffer, _displayBuffer, _displaySize * sizeof(unsigned long));
    }
}

/**
 * @brief NEW function to write only the *next* digit.
 * This is called by the high-priority displayTask.
 * It is non-blocking.
 */
void DispDriverMAX6921::writeNextDigit() {
    // Do nothing.
}

bool DispDriverMAX6921::needsContinuousUpdate() const {
    return true;
}
