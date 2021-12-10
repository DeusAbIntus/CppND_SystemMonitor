#ifndef PROCESSOR_H
#define PROCESSOR_H


class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private: // Declaring Previous iterations of system data and initilizing them as 0.
    float PrevTotal {0}, PrevIdle {0};
};

#endif