#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include "commons.h"

using namespace std;

void tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

float strToFloat(const string &str)
{
    istringstream iss(str);
    float result;
    iss >> result;
    return result;
}

int strToInt(const string &str)
{
    istringstream iss(str);
    int result;
    iss >> result;
    return result;
}

string intToStr(const int &in)
{
    ostringstream oss;
    oss << in;
    return oss.str();
}

string floatToStr(const float &flt)
{
    ostringstream oss;
    oss << flt;
    return oss.str();
}

int getRndInt(int min, int max)
{
	return (rand() % (max-min)) + min;
}

float getRndFloat(float min, float max)
{
	return (max-min)*((float)rand() / RAND_MAX) + min;
}
