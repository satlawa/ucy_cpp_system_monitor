#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)
  : pid_(pid),
    command_(LinuxParser::Command(pid)),
    ram_(LinuxParser::Ram(pid)),
    uptime_(LinuxParser::UpTime(pid)),
    user_(LinuxParser::User(pid)) {

  vector<float> cpuInfo_pid = LinuxParser::ActiveJiffies(pid);
  float utime = cpuInfo_pid[0];
  float stime = cpuInfo_pid[1];
  float cutime = cpuInfo_pid[2];
  float cstime = cpuInfo_pid[3];
  float start_time = cpuInfo_pid[4];

  float total_time = utime + stime + cutime + cstime;

  float seconds = LinuxParser::UpTime() - start_time;

  if (seconds != 0) {
    cpu_ = total_time / seconds;
  } else {
    cpu_ = 0;
  }
}
// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_; }

// Return the command that generated this process
string Process::Command() const { return command_; }

// Return this process's memory utilization
string Process::Ram() const { return ram_; }

// Return the user (name) that generated this process
string Process::User() const { return user_; }

//  Return the age of this process (in seconds)
long int Process::UpTime() const { return uptime_; }
// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.CpuUtilization();
}
