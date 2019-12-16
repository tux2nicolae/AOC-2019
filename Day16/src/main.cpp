/**
 * Advent of code 2019
 * @author : Nicolae Telechi
 */
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <unordered_map>
#include <optional>
#include <numeric>
#include <assert.h>
#include <execution>

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

vector<int> nextPattern;
vector<int> basePattern{ 0, 1, 0, -1 };

// part 2
const size_t kPhases = 100;
const size_t kMultiplyInput = 10000;
const size_t kSevenDigits = 5970927;

// int getRepeatingPatternValue(size_t linePos, size_t valuePos)
// {
//   linePos++;
//   size_t patternSize = (linePos * 4);
// 
//   size_t inPatternPos = (valuePos) % patternSize;
//   size_t finalPos = ((inPatternPos + 1) % patternSize) / linePos;
// 
//   return basePattern[finalPos];
// }

// long long getSum(const vector<int> & moonPhase, size_t size, size_t linePos)
// {
  // size_t patternSize = (linePos + 1) * 4;

  // size_t until = patternSize;
  // for (; until < inputValues.size(); until += patternSize);
  // 
  // long long sum = 0;
  // for (size_t j = 0; j < until; ++j)
  // {
  //   sum += moonPhase[j % moonPhase.size()] * getRepeatingPatternValue(linePos, j);
  // }

  // add remained values (part2 : *10000)
  // size_t totalSize = inputValues.size();
  // size_t repeatCount = totalSize / until;
  // 
  // sum *= repeatCount;
  // until *= repeatCount;

  // try paralel
  // const int * firstElement = &moonPhase[0];
  // long long sum = std::reduce(std::execution::par_unseq, begin(moonPhase) + 1, end(moonPhase), 0ll, [firstElement, linePos](long long current, const int& phase) {
  //   size_t index = &phase - firstElement;
  //   current += phase * getRepeatingPatternValue(linePos, index);
  // 
  //   return current;
  // });

  // long long sum = 0;
  // for (size_t j = max(linePos, kSevenDigits); j < size; ++j)
  // {
  //   sum += moonPhase[j]; // *getRepeatingPatternValue(linePos, j);
  // }
  // 
  // // cout << sum << " ";
  // return sum;
// }

void runPhase(vector<int> & moonPhase, const size_t size)
{
  // initial line sum
  int lineSum = 0;
  for (size_t j = kSevenDigits; j < size; ++j)
  {
    // all will be 1
    lineSum += moonPhase[j]; // *getRepeatingPatternValue(linePos, j);
  }


  for (int i = kSevenDigits; i < size; ++i)
  {
    int temp = moonPhase[i];

    moonPhase[i] = lineSum % 10;
    lineSum -= temp;
  }
}

int main()
{
  ifstream in("..\\..\\Day16\\src\\Day16.in");
  ofstream out("..\\..\\Day16\\src\\Day16.out");

  FStreamReader reader(in);
  auto inputValues = reader.ReadLineAsVectorOfDigits();

  // input
  nextPattern = inputValues;

  // copies
  for(int i = 1; i < kMultiplyInput; i++)
    copy(begin(inputValues), end(inputValues), back_inserter(nextPattern));

  // const int nextPatternSize = inputValues.size() * kMultiplyInput;
  
  for(int i = 0; i < kPhases; ++i)
  {
    cout << "Phase : " << i << endl;
    runPhase(nextPattern, nextPattern.size());
  }

  for (int j = kSevenDigits; j < nextPattern.size(); ++j)
  {
    out.width(1);
    out << nextPattern[j % nextPattern.size()];
    // out << getRepeatingPatternValue(i, j);

    if ((j + 1) % nextPattern.size() == 0)
      out << " ";
  }

  out << endl;
  
  return 0;
}
