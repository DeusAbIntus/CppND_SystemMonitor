#include <dirent.h>
#include <unistd.h>
#include <sstream>
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
  string os, kernel, version;
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
  
  string line;
  string attributeName;
  float attributeValue, MemTotal, MemFree, MemUsed;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> attributeName >> attributeValue) {
        if (attributeName == "MemTotal:") 
        {MemTotal = attributeValue;}
        else if (attributeName == "MemFree:") 
        {MemFree = attributeValue;}
      }
    }
  }
  MemUsed = (MemTotal - MemFree)/MemTotal; //Calculating MemUsed in Percent
  
  return MemUsed;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long UptimeInSec, IdleTimeInSec;
  string line;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> UptimeInSec >> IdleTimeInSec;
  }
  return UptimeInSec; 
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  
  long totalJiffies  = ActiveJiffies() + IdleJiffies();
  
  return totalJiffies; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  vector<string> ProcessVector = LinuxParser::ProcessStatData(pid);
  long ProcActiveJiffies;
  /*string line;
  string DataValue;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
         while (linestream >> DataValue ) { //push values to the back of string vector
          ProcessVector.push_back(DataValue);
         }     
    }*/
  ProcActiveJiffies = std::stol(ProcessVector[kutime_]) + std::stol(ProcessVector[kstime_])
                     + std::stol(ProcessVector[kcuttime_]) + std::stol(ProcessVector[kcstime_]);
  return ProcActiveJiffies; 
}

// TODO: Read and return the number of active jiffies for the system


long LinuxParser::ActiveJiffies(){

  vector<long> CpuDataVector = LinuxParser::CpuUtilizationLongVector();
  long activeJiff = CpuDataVector[kUser_] + CpuDataVector[kNice_] + CpuDataVector[kSystem_] + CpuDataVector[kIRQ_] + CpuDataVector[kSoftIRQ_] + CpuDataVector[kSteal_];
  return activeJiff;

  //old code commented out
  /*string line;
  string cpuName;
  float user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  long activeJiffies;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> cpuName >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
        if (cpuName == "cpu") 
         activeJiffies = user + nice + system + irq + softirq + steal;
        return activeJiffies;
      }
    }
  }*/
}  


// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
  vector<long> CpuDataVector =LinuxParser::CpuUtilizationLongVector();
  long idleJiffies = CpuDataVector[kIdle_] + CpuDataVector[kIOwait_];
  //old code commented out
  /*string line;
  string cpuName;
  float user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  long idleJiffies;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> cpuName >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
        if (cpuName == "cpu") 
         idleJiffies = idle + iowait;
        return idleJiffies;
      }
    }
  }*/
  return idleJiffies;  
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> CPUVector {};
  string line;
  string cpuName, cpuValue;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> cpuName; // pop off first cpu name from the linestring
      if (cpuName == "cpu"){  // first check of the line belongs to aggrgrate cpu
        while (linestream >> cpuValue ) { //push values to the back of string vector
         CPUVector.push_back(cpuValue);
         }
      }   
    }
  }
  return CPUVector; 
}

//Helper function defined to convert Vector<string> from CpuUtilization () to Vector<long>

vector<long> LinuxParser::CpuUtilizationLongVector() {
  vector<string> CpuDataStr = LinuxParser::CpuUtilization();
  vector<long> CpuDataLong {};
    for( string i : CpuDataStr ){
      CpuDataLong.push_back(std::stol(i));
    }
  return CpuDataLong;
}


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string attributeName;
  int attributeValue;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> attributeName >> attributeValue) {
        if (attributeName == "processes") {
          return attributeValue;
        }
      }
    }
  }
  return attributeValue;
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string attributeName;
  int attributeValue;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> attributeName >> attributeValue) {
        if (attributeName == "procs_running") {
          return attributeValue;
        }
      }
    }
  }
  return attributeValue; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  
  std::ifstream filestream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
 
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string attributeName; 
  long attributeValue, ram_;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> attributeName >> attributeValue) {
        if (attributeName == "VmSize:") {
          ram_ = attributeValue;
        }
      }
    }
  }
  return to_string(ram_/1024); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string attributeName; 
  long attributeValue;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> attributeName >> attributeValue) {
        if (attributeName == "Uid:") {
          return to_string(attributeValue);
        }
      }
    }
  }
  return to_string(attributeValue);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string UserID,line;
  string UserName, x_;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> UserName >> x_ >> UserID) {
        if (UserID == LinuxParser::Uid(pid)) {
          return UserName;
        }
      }
    }
  }

  return UserName;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  vector<string> ProcessVector = LinuxParser::ProcessStatData(pid);
  long ProcUptime;
 /* string line;
  string DataValue; 
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
         while (linestream >> DataValue ) { //push values to the back of string vector
          ProcessVector.push_back(DataValue);
         }     
    }*/
  ProcUptime = std::stol(ProcessVector[kstarttime_]);
  return ProcUptime/sysconf(_SC_CLK_TCK);  
}

// Helper function declared to return an vector<string> containing Process data of given Process PID
vector<string> LinuxParser::ProcessStatData(int pid) {
  vector<string> ProcVector{};
  string line;
  string DataValue;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
         while (linestream >> DataValue ) { //push values to the back of string vector
            ProcVector.push_back(DataValue);
         }     
    }
  }
  return ProcVector;
}

// Helper function to calculate Cpu Utilixation of a given <PID> Process
// CPU usage calculated according to Stackoverflow Post :
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float LinuxParser::ProcessCpuUtilization(int pid) {
  long ProcActiveJiffies = LinuxParser::ActiveJiffies(pid);
  long SysUptime = LinuxParser::UpTime();
  long ProcUptime = LinuxParser::UpTime(pid);
  long CpuUsage = ((ProcActiveJiffies/sysconf(_SC_CLK_TCK))/(SysUptime-ProcUptime))*100;
  return CpuUsage;
}
