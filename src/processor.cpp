#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

Processor::Processor() : prevTotal_(0.0), prevIdle_(0.0) {};

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<long> cpuInfo = convertToLong(LinuxParser::CpuUtilization());

  float total = cpuInfo[1] + cpuInfo[2] +
      cpuInfo[3] + cpuInfo[4] +
      cpuInfo[5] + cpuInfo[6] +
      cpuInfo[7] + cpuInfo[8];

  float idle = cpuInfo[4] + cpuInfo[5];

  float totalDiff = total - prevTotal_;
  float idleDiff = idle - prevIdle_;

  float percentage = (totalDiff - idleDiff) / totalDiff;

  prevTotal_ = total;
  prevIdle_ = idle;

  return percentage;
}

vector<long> Processor::convertToLong(vector<string> values) {
  vector<long> convertedValues{};

  for (int it = 0; it < (int)values.size(); it++) {
    try {
      convertedValues.push_back(std::stol(values[it]));
    } catch (const std::invalid_argument& arg) {
      convertedValues.push_back((long)0);
    }
  }
  return convertedValues;
}
