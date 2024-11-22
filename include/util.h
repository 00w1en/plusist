#pragma once

#include <vector>
#include <cstdint>
#include <Windows.h>

#define KEY_0 0x30
#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33
#define KEY_4 0x34
#define KEY_5 0x35
#define KEY_6 0x36
#define KEY_7 0x37
#define KEY_8 0x38
#define KEY_9 0x39
#define KEY_A 0x41
#define KEY_B 0x42
#define KEY_C 0x43
#define KEY_D 0x44
#define KEY_E 0x45
#define KEY_F 0x46
#define KEY_G 0x47
#define KEY_H 0x48
#define KEY_I 0x49
#define KEY_J 0x4A
#define KEY_K 0x4B
#define KEY_L 0x4C
#define KEY_M 0x4D
#define KEY_N 0x4E
#define KEY_O 0x4F
#define KEY_P 0x50
#define KEY_Q 0x51
#define KEY_R 0x52
#define KEY_S 0x53
#define KEY_T 0x54
#define KEY_U 0x55
#define KEY_V 0x56
#define KEY_W 0x57
#define KEY_X 0x58
#define KEY_Y 0x59
#define KEY_Z 0x5A

#define SIZE_OF_ARRAY(arr) sizeof(arr)/sizeof(arr[0])

constexpr uint8_t BLACK_KEYS[5] = { 1, 3, 6, 8, 10 };
constexpr uint8_t NOTES[61] = {
  KEY_1,  // C2
  KEY_1,  // C#2
  KEY_2,  // D2
  KEY_2,  // D#2
  KEY_3,  // E2
  KEY_4,  // F2
  KEY_4,  // F#2
  KEY_5,  // G2
  KEY_5,  // G#2
  KEY_6,  // A2
  KEY_6,  // A#2
  KEY_7,  // B2
  KEY_8,  // C3
  KEY_8,  // C#3
  KEY_9,  // D3
  KEY_9,  // D#3
  KEY_0,  // E3
  KEY_Q,  // F3
  KEY_Q,  // F#3
  KEY_W,  // G3
  KEY_W,  // G#3
  KEY_E,  // A3
  KEY_E,  // A#3
  KEY_R,  // B3
  KEY_T,  // C4
  KEY_T,  // C#4
  KEY_Y,  // D4
  KEY_Y,  // D#4
  KEY_U,  // E4
  KEY_I,  // F4
  KEY_I,  // F#4
  KEY_O,  // G4
  KEY_O,  // G#4
  KEY_P,  // A4
  KEY_P,  // A#4
  KEY_A,  // B4
  KEY_S,  // C5
  KEY_S,  // C#5
  KEY_D,  // D5
  KEY_D,  // D#5
  KEY_F,  // E5
  KEY_G,  // F5
  KEY_G,  // F#5
  KEY_H,  // G5
  KEY_H,  // G#5
  KEY_J,  // A5
  KEY_J,  // A#5
  KEY_K,  // B5
  KEY_L,  // C6
  KEY_L,  // C#6
  KEY_Z,  // D6
  KEY_Z,  // D#6
  KEY_X,  // E6
  KEY_C,  // F6
  KEY_C,  // F#6
  KEY_V,  // G6
  KEY_V,  // G#6
  KEY_B,  // A6
  KEY_B,  // A#6
  KEY_N,  // B6
  KEY_M   // C7
};

void TurnON(uint8_t pitch);
void TurnOFF(uint8_t pitch);
int Keyboard(int code, int flags=0);
uint32_t ReadValue(std::vector<uint8_t>::iterator& data);
bool FindInArray(const uint8_t* arr, const uint8_t n, const uint8_t val);
