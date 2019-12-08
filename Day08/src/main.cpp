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

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

int main()
{
  ifstream in("..\\..\\Day08\\src\\Day08.in");
  ofstream out("..\\..\\Day08\\src\\Day08.out");

  FStreamReader reader(in);
  auto digits = reader.ReadLineAsVectorOfDigits();
  
  const int kRows = 6;
  const int kColumns = 25;
  
  vector<vector<int>> image(kRows, vector<int>(kColumns, 0));
  vector<vector<bool>> written(kRows, vector<bool>(kColumns, false));

  int min = numeric_limits<int>::max();
  vector<int> minFreq;

  const int kLayerSize = kRows * kColumns;
  const int kLayers = digits.size() / kLayerSize;

  for (int k = 0; k < kLayers; ++k)
  {
    vector<int> freq(10);

    for (int i = 0; i < kRows; ++i)
    {
      for (int j = 0; j < kColumns; ++j)
      {
        int pos = (k* kLayerSize) + (i * kColumns + j);
        freq[digits[pos]]++;

        // part 2
        if (digits[pos] != 2 && !written[i][j])
        {
          written[i][j] = true;
          image[i][j] = digits[pos];
        }
      }
    }

    if (freq[0] < min)
    {
      min = freq[0];
      minFreq = freq;
    }
  }

  // print
  for (int i = 0; i < kRows; ++i)
  {
    for (int j = 0; j < kColumns; ++j)
    {
      char c = image[i][j] ? '*' : ' ';
      out << c;
    }

    out << endl;
  }

  cout << minFreq[1] * minFreq[2];
  return 0;
}
