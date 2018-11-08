#include "song_reader.h"

Note SongReader::string_to_note(Note prev, string s){
  if(!regex_match(s, regex("[a-gr][ies]*[\',]*\\d*\\.*")))
    return Note();

  string accidental = "", modifier = "";
	string note = "";
	int duration = 0, dots = 0;
  int octave = BELOW_MIDDLE_C_OCTAVE;

	note += s[0];
	s.erase(s.begin());
	for (auto & c : s){
		if (isdigit(c)){
			duration *= 10;
			duration += (c - '0');
		} else if (isalpha(c))
			accidental += c;
		else if(c == '.')
      dots++;
    else
			modifier += c;
	}

  for (auto& c : modifier){
    if (c == '\'')
      octave++;
    else if (c == ',')
      octave--;
  }

  if(duration){
    //TODO setar valor mínimo (fusa atualmente)
    duration = (duration ? QUANT / duration : duration);
    int increment = duration / 2;
    while(dots--){
      duration += increment;
      increment /= 2;
    }
  }
  else
    duration = prev.duration;

  accidental = regex_replace(accidental, regex("is"), "#");
  accidental = regex_replace(accidental, regex("es"), "\u266D");

	Note n(note, accidental, octave, duration);
  n.valid = (duration != 0);
  return n;
}

string SongReader::note_to_string(Note note){
  int octave_diff = note.octave - BELOW_MIDDLE_C_OCTAVE;
  string s = note.full_note();

  s = regex_replace(s, regex("#"), "is");
  s = regex_replace(s, regex("\u266D"), "es");

  if(octave_diff > 0){
    while(octave_diff--)
      s += "\'";
  }else{
    while(octave_diff++)
      s += ",";
  }

  string duration = to_string((note.duration ? QUANT / msb(note.duration) : 0));
  int total_dots = number_of_on_bits(note.duration) - 1;
  while(total_dots-- > 0){
    duration += ".";
  }

  s += duration;

  return s;
}

Scale SongReader::string_to_scale(string line){
  if(!regex_match(line, regex("\\\\key [a-g] \\\\(major|minor)")))
    return Scale();

  stringstream scale_line(line);
  string identificator, key, mode;
  scale_line >> identificator >> key >> mode;
  mode.erase(mode.begin());
  return Scale(key, mode);
}

string SongReader::scale_to_string(Scale scale){
  return "\\key " + scale.base_note.note + " \\" + scale.mode;
}

CompassTime SongReader::string_to_compass_time(string line){
  if(!regex_match(line, regex("\\\\time \\d+\\/\\d+")))
    return CompassTime();

  int times, base_note;
  sscanf(line.c_str(), "\\time %d/%d\n", &times, &base_note);
  return CompassTime(times, base_note);
}

string SongReader::compass_time_to_string(CompassTime compass_time){
  return "\\time " + to_string(compass_time.times) + "/" + to_string(compass_time.base_note);
}

int SongReader::msb(int N) {
  return N ? 1 << (31 - __builtin_clz(N)) : 0;
}

int SongReader::number_of_on_bits(int N) {
  return __builtin_popcount(N);
}
