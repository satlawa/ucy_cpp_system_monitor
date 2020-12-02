#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

Processor::Processor() : oldTotal_(0.0), oldIdle_(0.0) {};

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {

  vector<long> cpuInfo = LinuxParser::CpuUtilization();
  // vector = string cpu, user, nice, system, idle,
  //          ioWait, irq, softIrq, steal, guest, guestNice;
  // calculate total work load
  float newTotal = cpuInfo[1] + cpuInfo[2] + cpuInfo[3] + cpuInfo[4] +
      cpuInfo[5] + cpuInfo[6] + cpuInfo[7] + cpuInfo[8];
  // calculate total idle
  float newIdle = cpuInfo[4] + cpuInfo[5];
  // subtract new values by old values to get a more recent CPU utilisation
  float totalDiff = newTotal - oldTotal_;
  float idleDiff = newIdle - oldIdle_;
  // total CPU utilistion
  float percentage = (totalDiff - idleDiff) / totalDiff;
  // update old values
  oldTotal_ = newTotal;
  oldIdle_ = newIdle;

  return percentage;
}
