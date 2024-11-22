#pragma once

#include <util.h>
#include <string>

// MIDI Event Types
constexpr uint8_t NOTE_OFF = 0x80;
constexpr uint8_t NOTE_ON = 0x90;
constexpr uint8_t KEY_PRESSURE = 0xA0;
constexpr uint8_t CONTROL_CHANGE = 0xB0;
constexpr uint8_t PROGRAM_CHANGE = 0xC0;
constexpr uint8_t CHANNEL_PRESSURE = 0xD0;
constexpr uint8_t PITCH_BEND = 0xE0;
constexpr uint8_t SYSTEM_EXCLUSIVE_START = 0xF0;
constexpr uint8_t SYSTEM_EXCLUSIVE_END = 0xF7;

// Meta Event Types
constexpr uint8_t META_EVENT = 0xFF;
constexpr uint8_t SEQUENCE_NUMBER = 0x00;
constexpr uint8_t TEXT_EVENT = 0x01;
constexpr uint8_t COPYRIGHT_NOTICE = 0x02;
constexpr uint8_t TRACK_NAME = 0x03;
constexpr uint8_t INSTRUMENT_NAME = 0x04;
constexpr uint8_t LYRICS = 0x05;
constexpr uint8_t MARKER = 0x06;
constexpr uint8_t CUE_POINT = 0x07;
constexpr uint8_t MIDI_CHANNEL_PREFIX = 0x20;
constexpr uint8_t MIDI_PORT = 0x21;
constexpr uint8_t END_OF_TRACK = 0x2F;
constexpr uint8_t TEMPO_CHANGE = 0x51;
constexpr uint8_t SMPTE_OFFSET = 0x54;
constexpr uint8_t TIME_SIGNATURE = 0x58;
constexpr uint8_t KEY_SIGNATURE = 0x59;
constexpr uint8_t SEQUENCER_SPECIFIC = 0x7F;

// Event Lengths (in bytes) for each MIDI Event Type
constexpr uint8_t EVENTS[7] = {
  2,  // NOTE_OFF
  2,  // NOTE_ON
  2,  // KEY_PRESSURE
  2,  // CONTROL_CHANGE
  1,  // PROGRAM_CHANGE
  1,  // CHANNEL_PRESSURE
  2   // PITCH_BEND 
};
constexpr uint8_t META_EVENTS[16] = { SEQUENCE_NUMBER, TEXT_EVENT, COPYRIGHT_NOTICE, TRACK_NAME, INSTRUMENT_NAME, LYRICS, MARKER, CUE_POINT, MIDI_CHANNEL_PREFIX, MIDI_PORT, END_OF_TRACK, TEMPO_CHANGE, SMPTE_OFFSET, TIME_SIGNATURE, KEY_SIGNATURE, SEQUENCER_SPECIFIC };

struct Note
{
  uint8_t pitch;
  uint8_t status;
  uint8_t velocity;
  uint32_t startTime;
};

struct Event
{
  uint32_t tick;
  uint32_t length;
  uint8_t status;
  std::vector<uint8_t> data;
};

class Midi
{
public:
	float tempo = 0;
  float resolution = 0;                    // Time resolution in ticks per quarter note
  std::vector<std::vector<Event>> tracks;  // List to store MIDI track data

	bool ParseFile(const std::string& filePath);
  std::vector<Event> ParseTrack(std::vector<uint8_t>::iterator trackdata);

  std::vector<Note> GetNotes();
};
