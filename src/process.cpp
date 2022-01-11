#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//Process Contructor defination

Process::Process (int pid) {
    pid_ = pid;
    user_ = LinuxParser::User(pid);
    command_ = LinuxParser::Command(pid);
    cpuUsage_ = LinuxParser::ProcessCpuUtilization(pid);
    ram_ = LinuxParser::Ram(pid);
    uptime_ = LinuxParser::UpTime() - LinuxParser::UpTime(pid);
}

// TODO: Return this process's ID
int Process::Pid() const { return this->pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return this->cpuUsage_; }

// TODO: Return the command that generated this process
string Process::Command() { return this->command_; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return this->ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return this->user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return this->uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const {
    return (cpuUsage_< a.cpuUsage_) ? true : false;
 }