#include <fstream>
#include <string>
#include "debugout.h"
#include "timestamp.h"

using std::ofstream;
using std::string;

void output(string message) {
    ofstream outfile;
    outfile.open("debug.log", std::ios::out | std::ios::app);
    outfile << timestring() << "> " << message << std::endl;
    outfile.close();
}
