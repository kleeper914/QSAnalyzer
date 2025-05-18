#ifndef UTILITY_H
#define UTILITY_H

#include <QString>
#include <complex>
#include <sstream>
#include <vector>

constexpr double PI = 3.14159265358979323846;

QString complexToQString(const std::complex<double>& complexNum);
bool containsNumber(const std::string& s);
bool isComplexLine(const std::string& s);
void extractRealAndImg(const std::vector<std::complex<double>>& signal, std::vector<double>& real, std::vector<double>& imag);
std::vector<double> linToDB(const std::vector<double>& data, bool isPower);
void shiftVector(std::vector<double>& vec);

#endif // UTILITY_H
