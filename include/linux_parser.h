#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates { // assigning int values to CPUStates to be used in place of vector indices.
  kUser_      = 0,
  kNice_      = 1,
  kSystem_    = 2,
  kIdle_      = 3,
  kIOwait_    = 4,
  kIRQ_       = 5,
  kSoftIRQ_   = 6,
  kSteal_     = 7,
  kGuest_     = 8,
  kGuestNice_ = 9
};

std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();
std::vector<long> CpuUtilizationLongVector(); // Helper function used to convert vector<string> from CpuUtilization to vector<long>

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
std::vector<std::string> ProcessStatData(int pid); // Helper function declared to return an vector<string> containing Process data of given Process PID
float ProcessCpuUtilization(int pid); // Helper function to calculate Cpu Utilixation of a given <PID> Process

enum ProcessData{ 
  /* enum defined to be used as vector indices. Numbers before the data name (in the comments next to the enums) 
  represent the position of the data in the stat file of the Process. The values assigned to the enums is 
  reduced by one since vector indices start with 0 .
  Reference - https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599*/
  
  kutime_     = 13,             // #14 utime - CPU time spent in user code, measured in clock ticks
  kstime_     = 14,             // #15 stime - CPU time spent in kernel code, measured in clock ticks
  kcuttime_   = 15,             // #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
  kcstime_    = 16,             // #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
  kstarttime_ = 21              // #22 starttime - Time when the process started, measured in clock ticks
};

};  // namespace LinuxParser

#endif