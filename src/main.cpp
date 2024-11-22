#include <midi.h>

#include <map>
#include <iostream>


int main(int argc, char *argv[])
{
  std::string filePath;
  std::cout << "MIDI file path: ";
  getline(std::cin, filePath);

  float delay = 0;
  std::cout << "Start delay time: ";
  std::cin >> delay;

  Midi midi;
  bool res = midi.ParseFile(filePath);
  if (res == false) {
    return 1;
  }

  std::vector<Note> notes = midi.GetNotes();

  Sleep(3000);

  uint32_t current_time = 0;
  std::map<uint8_t, uint32_t> active_notes;

  for (Note note : notes) {
    if (GetAsyncKeyState(VK_BACK) & 0x8000) {
      break;
    }
    if (note.pitch < 36 || note.pitch > 96) {
        continue;
    }

    delay = note.startTime - current_time;
    current_time = note.startTime;

    Sleep(midi.tempo/1000.0f/midi.resolution*delay);

    if (note.status == NOTE_ON) {
      if (note.velocity == 0) {
        if (active_notes.find(note.pitch) != active_notes.end()) {
          TurnOFF(note.pitch);
          active_notes.erase(note.pitch);
        }
      } else {
        if (active_notes.find(note.pitch) != active_notes.end()) {
          TurnOFF(note.pitch);
        }
        TurnON(note.pitch);
        active_notes[note.pitch] = current_time;
      }
    } else {
      if (active_notes.find(note.pitch) != active_notes.end()) {
        TurnOFF(note.pitch);
        active_notes.erase(note.pitch);
      }
    }
  }

  for (std::pair<const uint8_t, uint32_t> &p : active_notes) {
    TurnOFF(p.first);
  }

  return 0;
}
