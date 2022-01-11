#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>
#include <unistd.h>

int main() {
  System system;
  NCursesDisplay::Display(system);
  std::vector<std::string> v = LinuxParser::ProcessStatData (2496);
for (std::string i : v)
{
  std::cout << i << " ";
}
  std::cout << "\n";

  float CpuUsage = LinuxParser::ProcessCpuUtilization(2496);

  std::cout << CpuUsage << "\n";

} 