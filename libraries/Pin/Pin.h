#ifndef PIN_H_
#define PIN_H_

class Pin
{
 private:
  uint8_t num_;
  uint8_t mode_;
 public:
  Pin(uint8_t num, uint8_t mode);
  uint8_t getNum();
  uint8_t getMode();
  uint8_t getValue();
  bool setValue(uint8_t value);
};

#endif // PIN_H_
