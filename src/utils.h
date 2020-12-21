#pragma once
#include <vector>
#include <functional>
using std::vector;
using std::function;

vector<long> randomVector(unsigned int length, unsigned int seed, unsigned int floor, unsigned int ceil);
void printVector(vector<long> v);
function<long(long)> createNormalizer(const long modulo, const long decrement=0);
