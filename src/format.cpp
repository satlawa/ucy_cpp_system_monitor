#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long secHour) {
  // calculate hours
  int hours = secHour / 3600;
 FormatTime(hours);
  // calculate seconds left after converting to hours
  int secMin = secHour % 3600;
  // calculate minutes
  int minutes = secMin / 60;
  // calculate seconds
  int seconds = secMin % 60;
  // format to string and return
  return FormatTime(hours) + ":" + FormatTime(minutes) + ":" + FormatTime(seconds);
 }

string Format::FormatTime(int time) {
  string timeFormat;
  if (time < 10) {
    timeFormat = "0" + std::to_string(time);
  }
  else {
    timeFormat = std::to_string(time);
  }
  return timeFormat;
}
