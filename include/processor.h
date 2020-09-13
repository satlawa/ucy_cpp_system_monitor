#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
   float prevTotal_;
   float prevIdle_;

   std::vector<long> convertToLong(std::vector<std::string> values);
};

#endif
