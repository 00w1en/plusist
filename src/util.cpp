#include <util.h>


void TurnON(uint8_t pitch)
{
  if (FindInArray(BLACK_KEYS, SIZE_OF_ARRAY(BLACK_KEYS), (pitch % 12))) {
    Keyboard(VK_SHIFT);
  }
  Keyboard(NOTES[pitch-36]);
}


void TurnOFF(uint8_t pitch)
{
  if (FindInArray(BLACK_KEYS, SIZE_OF_ARRAY(BLACK_KEYS), (pitch % 12))) {
    Keyboard(VK_SHIFT, KEYEVENTF_KEYUP);
  }
  Keyboard(NOTES[pitch-36], KEYEVENTF_KEYUP);
}


int Keyboard(int code, int flags)
{
  INPUT input = {};
  input.ki.wVk = code;
  input.ki.dwFlags = flags;
  input.type = INPUT_KEYBOARD;
  return SendInput(1, &input, sizeof(INPUT));
}


uint32_t ReadValue(std::vector<uint8_t>::iterator& data)
{
    uint8_t byte = 0x80;
    uint32_t value = *data; data++;

    // Check MSB, if set, more bytes need reading
    if (value & 0x80) {
      value &= 0x7F;  // Mask out the MSB
      while (byte & 0x80) {
        byte = *data; data++;
        value = (value << 7) | (byte & 0x7F);  // Shift left and add the next 7 bits
      }
    }

    // Return final construction
    return value;
}


bool FindInArray(const uint8_t* arr, const uint8_t n, const uint8_t val)
{
  for (uint8_t i = 0; i < n; i++) {
    if (arr[i] == val) {
      return true;
    }
  }
  return false;
}
