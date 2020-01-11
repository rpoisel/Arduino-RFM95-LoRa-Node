#include "data.h"

constexpr uint8_t const RFM95_CS = PIN_RFM95_CS;
constexpr uint8_t const RFM95_INT = PIN_RFM95_INT;

RH_RF95 rf95(RFM95_CS, RFM95_INT);
