#include "inc/stdinc.h"

Touch::RawTouchPoint Touch::getRawTouchPoint()
{
    constexpr pin_t Pin_XPlus = A2;
    constexpr pin_t Pin_XMinus = A0;
    constexpr pin_t Pin_YPlus = A3;
    constexpr pin_t Pin_YMinus = A1;

    //
    // Read X position
    // - set Y axis pins as inputs
    // - provide 0->Vcc across X axis
    // - read Y pin
    //

    pinMode(Pin_YPlus, INPUT);
    pinMode(Pin_YMinus, INPUT);

    pinMode(Pin_XPlus, OUTPUT);
    pinMode(Pin_XMinus, OUTPUT);

    digitalWrite(Pin_XPlus, HIGH);
    digitalWrite(Pin_XMinus, LOW);

    delay(1);  // let voltage set up

    uint16_t const xReading = analogRead(Pin_YPlus);

    //
    // Read Y position
    // - set X axis pins as inputs
    // - provide 0->Vcc across Y axis
    // - read X pin
    //

    pinMode(Pin_XPlus, INPUT);
    pinMode(Pin_XMinus, INPUT);

    pinMode(Pin_YPlus, OUTPUT);
    pinMode(Pin_YMinus, OUTPUT);

    digitalWrite(Pin_YPlus, HIGH);
    digitalWrite(Pin_YMinus, LOW);

    delay(1);  // let voltage set up

    uint16_t const yReading = analogRead(Pin_XPlus);

    //
    // Read pressure
    // - set XPlus, YMinus pins as inputs
    // - provide 0->Vcc across XMinus->YPlus
    // - read positive pin(s)
    //

    pinMode(Pin_XPlus, INPUT);
    pinMode(Pin_YMinus, INPUT);

    pinMode(Pin_XMinus, OUTPUT);
    pinMode(Pin_YPlus, OUTPUT);

    digitalWrite(Pin_YPlus, HIGH);
    digitalWrite(Pin_XMinus, LOW);

    delay(1);  // let voltage set up

    uint16_t const zReading1 = analogRead(Pin_XPlus);
    // uint16_t const zReading2 = analogRead(Pin_YMinus); // - not required

    //
    // Evaluate
    //

    float const adcMaxValue = (1 << 12) - 1;

    // Flip X/Y coordinates since we're using the screen sideways from its native orientation
    return RawTouchPoint{yReading / adcMaxValue, xReading / adcMaxValue, zReading1 / adcMaxValue};
}
