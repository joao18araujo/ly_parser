#include "second_species_counterpoint.h"

Song SecondSpeciesCounterpoint::dfs_generate_counterpoint(Song & c_song, bool ascendant, int paralels, int same_movements){
  song = &c_song;
  Song counterpoint(c_song.scale, c_song.time);
  if(song->size() > 201 || paralels > 4 || same_movements > 101)
    return Song();

  SecondSpeciesCounterpoint::solve(0, 0, paralels, same_movements, ascendant, counterpoint);
  SecondSpeciesCounterpoint::add_trailing_rests(counterpoint);

  return counterpoint;
}

void SecondSpeciesCounterpoint::analyse_and_add_interval(bool reverse_movement, bool melodic_ascendant, vector<Interval> & possible_intervals, Note previous_note, Note note, Interval interval, Song * counterpoint){
  Note next_note;
  auto previous_counterpoint_note = counterpoint->notes.back();

  if(interval.is_consonant())
    next_note = Scale::interval_to_note_on_scale(note, interval, song->scale);
  else
    next_note = Interval::interval_to_note(note, interval);

  auto previous_interval = Interval(previous_note, previous_counterpoint_note);

  if(!next_note.valid or (interval.is_dissonant() and previous_interval.is_dissonant())){
    return;
  }

  Interval melodic_interval(previous_counterpoint_note, next_note);

  bool is_valid_or_passing = (interval.is_consonant() and previous_interval.is_consonant()) or
                             (melodic_interval == "m2") or
                             (melodic_interval == "M2");

  bool can_jump = (reverse_movement or
                  melodic_interval.quantitative <= 4 or
                  melodic_interval.quantitative == 8);

  if(((can_jump and (melodic_ascendant == melodic_interval.ascendant()) and note.valid) or
      previous_counterpoint_note.note == "r") and is_valid_or_passing)
    possible_intervals.push_back(interval);
}

bool SecondSpeciesCounterpoint::solve(unsigned position, unsigned compass_position, int paralels, int same_movements, bool ascendant, Song & counterpoint){
  if(position == 0){
    memset(dp, true, sizeof dp);
    srand(clock());
  }

  if(position >= song->size_without_rest()){
    // printf("Total: %d %lu\n", paralels, song->size() - same_movements);
    return true;
  }

  if(!dp[position][compass_position][song->notes[position].midi_number][paralels][same_movements]) return false;

  vector<Interval> possible_intervals;

  Note note = song->notes[position];

  if((position || compass_position) && (position < song->size_without_rest() - 1)){
    auto previous_note = song->notes[position - 1 + compass_position];
    auto previous_counterpoint_note = counterpoint.notes.back();
    auto melodic_cantus_interval = Interval(note, previous_note);
    bool melodic_ascendant = !melodic_cantus_interval.ascendant(); // XOR with true
    auto previous_interval = Interval(previous_note, previous_counterpoint_note);
    string previous = previous_interval.description();

    if(note.note != "r"){
      if(previous != "P8")
        analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("P8", ascendant), &counterpoint);

      if(previous != "P5" && ascendant)
        analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("P5", ascendant), &counterpoint);

      analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("m3", ascendant), &counterpoint);
      analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("M3", ascendant), &counterpoint);
      analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("m6", ascendant), &counterpoint);
      analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("M6", ascendant), &counterpoint);
      analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("m10", ascendant), &counterpoint);
      analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("M10", ascendant), &counterpoint);

      random_shuffle(possible_intervals.begin(), possible_intervals.end());
      int size = possible_intervals.size();

      //intervals with same movement
      if(previous != "P8")
        analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("P8", ascendant), &counterpoint);
      if(previous != "P5" && ascendant)
        analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("P5", ascendant), &counterpoint);

      analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("m3", ascendant), &counterpoint);
      analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("M3", ascendant), &counterpoint);
      analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("m6", ascendant), &counterpoint);
      analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("M6", ascendant), &counterpoint);
      analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("m10", ascendant), &counterpoint);
      analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("M10", ascendant), &counterpoint);

      random_shuffle(possible_intervals.begin() + size, possible_intervals.end());

      if(!is_thesis(note, (note.duration / 2) * compass_position)){
        //Tempo fraco
        analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("P4", ascendant), &counterpoint);
        analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("m2", ascendant), &counterpoint);
        analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("M2", ascendant), &counterpoint);
        analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("m7", ascendant), &counterpoint);
        analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("M7", ascendant), &counterpoint);
        // TODO: m9?
      }
    }else{
      possible_intervals.push_back(Interval("P1"));
    }

    // printf("[%u][%u][%d][%d][%d]\n", position - 1 + compass_position, compass_position, song->notes[position - 1 + compass_position].midi_number, paralels, same_movements);
    if(possible_intervals.empty()){
      dp[position - 1 + compass_position][(compass_position+1)%2][song->notes[position - 1 + compass_position].midi_number][paralels][same_movements] = false;

      return false;
    }

    int par, sm;
    for(auto interval : possible_intervals){
      auto c_note = Interval::interval_to_note(note, interval);
      c_note.duration /= 2;
      Interval melodic_interval(previous_counterpoint_note, note);

      par = paralels - (interval.quantitative == previous_interval.quantitative and (interval.quantitative == 3 || interval.quantitative == 6 || interval.quantitative == 10)); //TODO: criar método retornando qualidade
      sm = same_movements - (melodic_interval.ascendant() != melodic_ascendant);
      if(par < 0 || sm < 0) continue;
      counterpoint.notes.push_back(c_note);
      if(solve(position + compass_position, (compass_position + 1)%2, par, sm, ascendant, counterpoint)) return true;
      counterpoint.notes.pop_back();
    }

  }else if(position == song->size_without_rest() - 1){
    //Última nota
    auto previous_note = song->notes[position - 1];
    auto previous_counterpoint_note = counterpoint.notes.back();
    auto melodic_cantus_interval = Interval(note, previous_note);
    bool melodic_ascendant = !melodic_cantus_interval.ascendant(); // XOR with true
    auto previous_interval = Interval(previous_note, previous_counterpoint_note);
    string previous = previous_interval.description();

    if(note.note != "r"){
      analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("P1", ascendant), &counterpoint);
      if(previous != "P8")
        analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("P8", ascendant), &counterpoint);
      if(previous != "P5" && ascendant)
        analyse_and_add_interval(true, melodic_ascendant, possible_intervals, previous_note, note, Interval("P5", ascendant), &counterpoint);

      random_shuffle(possible_intervals.begin(), possible_intervals.end());
      int size = possible_intervals.size();

      analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("P1", ascendant), &counterpoint);
      if(previous != "P8")
        analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("P8", ascendant), &counterpoint);
      if(previous != "P5" && ascendant)
        analyse_and_add_interval(false, !melodic_ascendant, possible_intervals, previous_note, note, Interval("P5", ascendant), &counterpoint);

      random_shuffle(possible_intervals.begin() + size, possible_intervals.end());
    }else{
      possible_intervals.push_back(Interval("P1"));
    }

    int par, sm;
    for(auto interval : possible_intervals){
      auto c_note = Interval::interval_to_note(note, interval);
      Interval melodic_interval(previous_counterpoint_note, note);

      par = paralels - (interval.quantitative == previous_interval.quantitative and (interval.quantitative == 3 || interval.quantitative == 6 || interval.quantitative == 10)); //TODO: criar método retornando qualidade
      sm = same_movements -  (melodic_interval.ascendant() != melodic_ascendant);
      if(par < 0 || sm < 0) continue;
      counterpoint.notes.push_back(c_note);
      if(solve(position + 1, 0, par, sm, ascendant, counterpoint)) return true;
      counterpoint.notes.pop_back();
    }
  }else{
    //Primeira nota
    possible_intervals.push_back(Interval("P1", ascendant));
    possible_intervals.push_back(Interval("P8", ascendant));
    if(ascendant) possible_intervals.push_back(Interval("P5", ascendant));
    random_shuffle(possible_intervals.begin(), possible_intervals.end());

    for(auto interval : possible_intervals){
      auto c_note = Interval::interval_to_note(note, interval);
      c_note.duration /= 2;
      counterpoint.notes.push_back(c_note);
      if(solve(position + compass_position, (compass_position + 1)%2, paralels, same_movements, ascendant, counterpoint)) return true;
      counterpoint.notes.pop_back();
    }
  }

  dp[position][compass_position][song->notes[position].midi_number][paralels][same_movements] = false;
  return false;
}

void SecondSpeciesCounterpoint::add_trailing_rests(Song & counterpoint){
  unsigned size = song->size();
  for(unsigned i = song->size_without_rest() - 1; i < size; ++i){
    counterpoint.notes.push_back(song->notes[i]);
  }
}
