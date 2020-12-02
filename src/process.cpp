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

Process::Process(int pid) : pid_(pid), command_(LinuxParser::Command(pid)),
      user_(LinuxParser::User(pid)){
        cpu_ = Process::CalcCpuUtilization();
      };

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_; }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return cpu_ < a.cpu_;
}

float Process::CalcCpuUtilization() {

  vector<long> cpuInfo_pid = LinuxParser::ActiveJiffies(pid_);
  // total time spent of the process and its child processes
  float totalLoad_pid = cpuInfo_pid[0] + cpuInfo_pid[1] + cpuInfo_pid[2] + cpuInfo_pid[3];

  vector<long> cpuInfo = LinuxParser::CpuUtilization();
  // total time spent working of all the processes
  float totalLoad = cpuInfo[1] + cpuInfo[2] + cpuInfo[3] + cpuInfo[4] +
      cpuInfo[5] + cpuInfo[6] + cpuInfo[7] + cpuInfo[8];
  // total time spent idle of all the processes
  float totalIdle = cpuInfo[4] + cpuInfo[5];
  // total CPU utilistion
  float cpuUtilTot = (totalLoad-totalIdle)/totalLoad;
  // calculate CPU utilistion of the process by multiplying the total
  // CPU utilistion by the portion the process has on the total working processes
  float percentage = cpuUtilTot * (totalLoad_pid/totalLoad);

  return percentage;
}
