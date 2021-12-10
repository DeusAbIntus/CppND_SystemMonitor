#include <string>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds/*[[maybe_unused]]*/) { 
    int hr, min, sec;
    std::string ans;
    std::stringstream ss;
    
    hr = seconds / 3600;    // calculate hours from seconds
    min = (seconds - (hr * 3600)) / 60; // calculate min from remaining seconds after subtracting hours from seconds
    sec = seconds - (min * 60) - (hr * 3600); // calculate remaining seconds by substracting hours and min from original input
    
    //std::to_string(hr) + ":" + std::to_string(min) + ":" + std::to_string(sec); 
    ss  << std::setw(2) << std::setfill('0') << hr << ":"
        << std::setw(2) << std::setfill('0') << min << ":" 
        << std::setw(2) << std::setfill('0') << sec;
    ans = ss.str();
    return ans;
}