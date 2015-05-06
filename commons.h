#ifndef __COMMONS_H
#define __COMMONS_H

#include <string>
#include <vector>

using std::string;
using std::vector;

void tokenize(const string &str, vector<string> &tokens, const string &delimiters = " ");
float strToFloat(const string &str);
int strToInt(const string &str);
string intToStr(const int &in);
string floatToStr(const float &flt);
int getRndInt(int min, int max);
float getRndFloat(float min, float max)	;

#endif /* ifndef __COMMONS_H */
