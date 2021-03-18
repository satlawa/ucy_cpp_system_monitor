#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string command_;
  std::string ram_;
  long uptime_;
  std::string user_;
  float cpu_;

};

#endif
