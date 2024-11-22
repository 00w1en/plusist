#include <midi.h>

#include <fstream>
#include <iostream>


bool Midi::ParseFile(const std::string& filePath)
{
  std::ifstream ifs;
  ifs.open(filePath, std::fstream::in | std::ios::binary);
  if (!ifs.is_open()) {
    std::cout << "No such file or directory.";
    return false;
  }

  char buffer[4] = { 0 };

  // Read MIDI Header
  ifs.read(buffer, sizeof(uint32_t));       // Read the file identifier
  if (*(uint32_t*)&buffer != 1684558925) {  // Verify that the file is a valid MIDI file
    ifs.close();
    std::cout << "Bad header in MIDI file.";
    return false;
  }

  ifs.read(buffer, sizeof(uint32_t));
  std::cout << "[+] Header size: " << _byteswap_ulong(*(uint32_t*)&buffer) << std::endl;
  ifs.read(buffer, sizeof(uint16_t));
  std::cout << "[+] Format: " << _byteswap_ushort(*(uint16_t*)&buffer) << std::endl;

  ifs.read(buffer, sizeof(uint16_t));
  uint16_t numOfTrack = _byteswap_ushort(*(uint16_t*)&buffer);
  std::cout << "[+] Tracks:" << numOfTrack << std::endl;

  ifs.read(buffer, sizeof(uint16_t));
  this->resolution = _byteswap_ushort(*(uint16_t*)&buffer);
  std::cout << "[+] Parts Per Quarter: " << this->resolution << std::endl;

  std::vector<std::vector<uint8_t>> tracks;

  for (uint16_t t = 0; t < numOfTrack; t++) {
    ifs.read(buffer, sizeof(uint32_t));       // Read track identifier
    if (*(uint32_t*)&buffer != 1802654797) {  // Verify track header
      ifs.close();
      std::cout << "Bad track header in MIDI file.";
      return false;
    }

    std::cout << "[+] NEW TRACK" << std::endl;

    ifs.read(buffer, sizeof(uint32_t));
    uint32_t trksz = _byteswap_ulong(*(uint32_t*)&buffer);

    std::vector<uint8_t> track;

    for (uint32_t s = 0; s < trksz; s++) {
      track.push_back(ifs.get());
    }

    tracks.push_back(track);
  }

  ifs.close();

  for (std::vector<uint8_t> trackdata : tracks) {
    this->tracks.push_back(this->ParseTrack(trackdata.begin()));
  }

  return true;
}


 std::vector<Event> Midi::ParseTrack(std::vector<uint8_t>::iterator trackdata) {
  uint8_t prevsts = 0;
  bool trackEnd = false;
  std::vector<Event> track;  // Initialize a new list for track events

  while (trackEnd == false) {
    std::vector<uint8_t> data;
    uint32_t tick = ReadValue(trackdata);      // Read delta time
    uint8_t status = *trackdata; trackdata++;  // Read event status byte

    Event event;
    event.tick = tick;
    event.status = status;

    // Process Meta Event
    if (event.status == META_EVENT) {
      uint8_t msgtype = *trackdata; trackdata++;

      if (FindInArray(META_EVENTS, SIZE_OF_ARRAY(META_EVENTS), msgtype)) {
        event.length = ReadValue(trackdata);

        for (uint8_t i = 0; i < event.length; i++) {
          data.push_back(*trackdata); trackdata++;
        }

        if (msgtype == END_OF_TRACK) {
            trackEnd = true;
        } else if (msgtype == TEMPO_CHANGE && this->tempo == 0) {
          uint32_t tempo = 0;
          tempo |= (data[0] << 16);
          tempo |= (data[1] << 8);
          tempo |= (data[2] << 0);
          uint32_t bpm = (60000000 / tempo);
          this->tempo = tempo;
          std::cout << "Tempo: " << this->tempo << "(" << bpm << "bpm)" << std::endl;
        }
      } else {
        std::cout << "Unknown Meta MIDI Event: " << msgtype << std::endl;
      }
    // Process System Exclusive Event
    } else if (event.status == SYSTEM_EXCLUSIVE_START) {
      while (true) {
        uint8_t datum = *trackdata; trackdata++;
        if (datum == SYSTEM_EXCLUSIVE_END) {
            break;
        }
        data.push_back(datum);
      }
    // Process MIDI Event
    } else {
      // Check for Running Status
      if (event.status < NOTE_OFF) {
        data.push_back(event.status);
        event.status = prevsts;
        event.length = EVENTS[(event.status-NOTE_OFF)/16] - 1;
      } else {
        event.length = EVENTS[((event.status&0xF0)-NOTE_OFF)/16];
      }

      for (uint8_t i = 0; i < event.length; i++) {
        data.push_back(*trackdata); trackdata++;
      }

      event.status &= 0xF0;
      prevsts = event.status;
    }

    event.data = data;
    track.push_back(event);
  }

  return track;
}


std::vector<Note> Midi::GetNotes()
{
  std::vector<Note> notes = {};

  for (std::vector<Event> track : this->tracks) {
    uint32_t absolute_time = 0;

    for (Event event : track) {
      absolute_time += event.tick;

      if (event.status == NOTE_OFF || event.status == NOTE_ON) {
        Note note;
        note.status = event.status;
        note.startTime = absolute_time;
        note.pitch = event.data[0];
        note.velocity = event.data[1];
        notes.push_back(note);
      }
    }
  }

  for (int i = 1; i < notes.size(); i++) {
    Note key = notes[i];
    int j = i - 1;

    while (j >= 0 && notes[j].startTime > key.startTime) {
      notes[j+1] = notes[j];
      j--;
    }
    notes[j+1] = key;
  }

  return notes;
}
