#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

// TODO: Read and return the system memory utilization
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

// TODO: Read and return the system uptime
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

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  /*string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice;*/
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuInfo;
  //string cpu, user, nice, system, idle, ioWait, irq,
  //    softIrq, steal, guest, guestNice;
  string val;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 11; i++) {
      linestream >> val;
      cpuInfo.push_back(val);
    }
    /*
    linestream >> cpu >> user >> nice >> system >> idle >> ioWait >> irq >>
      softIrq >> steal >> guest >> guestNice;
      cpuInfo.push_back(user);
      cpuInfo.push_back(nice);
      cpuInfo.push_back(system);
      cpuInfo.push_back(idle);
      cpuInfo.push_back(ioWait);
      cpuInfo.push_back(irq);
      cpuInfo.push_back(softIrq);
      cpuInfo.push_back(steal);
      cpuInfo.push_back(guest);
      cpuInfo.push_back(guestNice);
    //*
    for (string line; std::getline(stream, line);) {
      std::istringstream linestream(line);
      for (string cpu, user, nice, system, idle, ioWait, irq,
          softIrq, steal, guest, guestNice;
        linestream >> cpu >> user >> nice >> system >> idle >> ioWait >> irq >>
          softIrq >> steal >> guest >> guestNice;) {

        }
      }*/
  }
  return cpuInfo;
}

// TODO: Read and return the total number of processes
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

// TODO: Read and return the number of running processes
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

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command = "";
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string ram = "";
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    for (string line; std::getline(stream, line);) {
      std::istringstream linestream(line);
      for (string type, val; linestream >> type >> val;) {
        if (type == "VmSize:") {
          ram = val;
        }
      }
    }
  }
  return ram;
}

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

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
