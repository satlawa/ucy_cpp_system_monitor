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

  float newTotal = cpuInfo[1] + cpuInfo[2] + cpuInfo[3] + cpuInfo[4] +
      cpuInfo[5] + cpuInfo[6] + cpuInfo[7] + cpuInfo[8];

  float newIdle = cpuInfo[4] + cpuInfo[5];

  float totalDiff = newTotal - oldTotal_;
  float idleDiff = newIdle - oldIdle_;

  float percentage = (totalDiff - idleDiff) / totalDiff;

  oldTotal_ = newTotal;
  oldIdle_ = newIdle;

  return percentage;
}
