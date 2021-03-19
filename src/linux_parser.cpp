#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


//=============================================================================
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

//=============================================================================
// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

//=============================================================================
// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//=============================================================================
// TODO: Read and return the system memory utilization - done
float LinuxParser::MemoryUtilization() {
  string memTotal, memFree, memTemp;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> memTemp >> memTotal;

    std::getline(stream, line);
    std::getline(stream, line);
    linestream.str(line);
    linestream >> memTemp >> memFree;
  }
  float memUse = 1 - (stof(memFree) / stof(memTotal));
  return memUse;
}

//=============================================================================
// TODO: Read and return the system uptime - done
long LinuxParser::UpTime() {
  string upTime, idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return stol(upTime);
}

//=============================================================================
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return ActiveJiffies() + IdleJiffies();
}

//=============================================================================
// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
vector<float> LinuxParser::ActiveJiffies(int pid) {
  vector<float> cpuInfoPid;
  string line, val;
  float time_part;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    for(int i = 0; i < 22; i++) {
      linestream >> val;
      if (i == 13 || i == 14 || i == 15 || i == 16 || i == 21) {
        time_part = stof(val) / sysconf(_SC_CLK_TCK);
        cpuInfoPid.push_back(time_part);
      }
    }
  }
  return cpuInfoPid;
}

//=============================================================================
// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<long> cpuInfo = CpuUtilization();
  return cpuInfo[0] + cpuInfo[1] + cpuInfo[2] + cpuInfo[5] +
         cpuInfo[6] + cpuInfo[7];
}

//=============================================================================
// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<long> cpuInfo = CpuUtilization();
  return cpuInfo[3] + cpuInfo[4];
}

//=============================================================================
// TODO: Read and return CPU utilization - done
vector<long> LinuxParser::CpuUtilization() {
  vector<long> cpuInfo;
  //string cpu, user, nice, system, idle, ioWait, irq,
  //    softIrq, steal, guest, guestNice;
  string val;
  string line;
  long valNum;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 11; i++) {
      linestream >> val;
      // convert string to long
      try {
        valNum = stol(val);
      } catch (const std::invalid_argument& arg) {
        valNum = 0;
      }
      cpuInfo.push_back(valNum);
    }
  }
  return cpuInfo;
}

//=============================================================================
// TODO: Read and return the total number of processes - done
int LinuxParser::TotalProcesses() {
  string processesTotal;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    for (string line; std::getline(stream, line);) {
      std::istringstream linestream(line);
      for (string type, val; linestream >> type >> val;) {
        if (type == "processes") {
          processesTotal = val;
        }
      }
    }
  }
  return stoi(processesTotal);
}

//=============================================================================
// TODO: Read and return the number of running processes - done
int LinuxParser::RunningProcesses() {
  string processesRunning;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    for (string line; std::getline(stream, line);) {
      std::istringstream linestream(line);
      for (string type, val; linestream >> type >> val;) {
        if (type == "procs_running") {
          processesRunning = val;
        }
      }
    }
  }
  return stoi(processesRunning);
}

//=============================================================================
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

//=============================================================================
// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  long ram;
  string type, ram_str = "";
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    for (string line; std::getline(stream, line);) {
      std::istringstream linestream(line);
      linestream >> type;
      if (type == "VmSize:") {
        linestream >> ram;
        ram /= 1000;
        ram_str = to_string(ram);
      }
    }
  }
  return ram_str;
}

//=============================================================================
// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string uid = "";
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    for (string line; std::getline(stream, line);) {
      std::istringstream linestream(line);
      for (string type, val; linestream >> type >> val;) {
        if (type == "Uid:") {
          uid = val;
        }
      }
    }
  }
  return uid;
}

//=============================================================================
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string user = "";
  string line, val, temp, key;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> val >> temp >> key) {
        if (key == uid) {
          user = val;
        }
      }
    }
  }
  return user;
}

//=============================================================================
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long upTimePid;
  string line, val;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    for (string line; std::getline(stream, line);) {
      std::istringstream linestream(line);
      for (int i = 0; i < 22; i++) {
        linestream >> val;
      }
      try {
        upTimePid = UpTime() - stol(val) / sysconf(_SC_CLK_TCK);
      } catch (const std::invalid_argument& arg) {
        upTimePid = 0;
      }
    }
  }
  return upTimePid;
}
