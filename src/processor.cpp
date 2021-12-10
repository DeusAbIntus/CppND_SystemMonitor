#include "processor.h"
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  vector<long> CpuVector = LinuxParser::CpuUtilizationLongVector();  // get CpuData from file
  string line;
  string CpuName;
  float user_{CpuVector[LinuxParser::kUser_]}, 
        nice_{CpuVector[LinuxParser::kNice_]}, 
        system_{CpuVector[LinuxParser::kSystem_]}, 
        idle_{CpuVector[LinuxParser::kIdle_]}, 
        iowait_{CpuVector[LinuxParser::kIOwait_]}, 
        irq_{CpuVector[LinuxParser::kIRQ_]},
        softirq_{CpuVector[LinuxParser::kSoftIRQ_]}, 
        steal_{CpuVector[LinuxParser::kSteal_]}, 
        guest_{CpuVector[LinuxParser::kGuest_]},
        guestnice_{CpuVector[LinuxParser::kGuestNice_]},
        Idle, NonIdle, Total, Totald, Idled, CPU_Percentage;

  // PrevIdle = previdle + previowait
  // Idle = idle + iowait
  //  PrevIdle = prevIdle_ + prevIOwait_;
      Idle = idle_ + iowait_;

  // PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
  // NonIdle = user + nice + system + irq + softirq + steal
      //PrevNonIdle = prevUser_ + prevNice_ + prevSystem_ + prevIrq_ + prevSoftirq_ + prevSteal_;
      NonIdle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;

  //  PrevTotal = PrevIdle + PrevNonIdle
  //  Total = Idle + NonIdle
     //PrevTotal = PrevIdle + PrevNonIdle;
      Total = Idle + NonIdle; 

  //  # differentiate: actual value minus the previous one
  //  Totald = Total - PrevTotal
  //  Idled = Idle - PrevIdle
      Totald = Total - PrevTotal;
      Idled = Idle - PrevIdle;

  //  CPU_Percentage = (totald - idled)/totald
      CPU_Percentage = (Totald - Idled)/Totald;

  // Update previous data
       PrevTotal = Total;
       PrevIdle = Idle; 
    
  // return Cpu Percentage
  return CPU_Percentage;
  
}