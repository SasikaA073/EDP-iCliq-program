/* 
Custom library for push up button
*/


class Button
{
private:
    uint8_t btn;
    uint16_t state;

public:
    void begin(uint8_t button)
    {
        btn = button;
        state = 0;
        pinMode(btn, INPUT_PULLUP);
    }
    bool debounce()
    {
        state = (state << 1) | digitalRead(btn) | 0xfe00;
        return (state == 0xff00);
    }
};
