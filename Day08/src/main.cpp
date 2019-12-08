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

unordered_map<string, vector<string>> graph;
unordered_map<string, int> dependencies;

int main()
{
  ifstream in("..\\..\\Day08\\src\\Day08.in");
  ofstream out("..\\..\\Day08\\src\\Day08.out");

  FStreamReader reader(in);
  auto v = reader.ReadMatrixOfDigits()[0];

  vector<vector<int>> layers(3600, vector<int>());

  int min = 1427343243;
  vector<int> minFreq;

  vector<vector<bool>> used(6, vector<bool>(25, 0));
  vector<vector<int>> image(6, vector<int>(25,0));

  for (int k = 0; k <= 99; ++k)
  {
    vector<int> freq(10);

    for (int i = 0; i < 6; ++i)
    {
      for (int j = 0; j < 25; ++j)
      {
        int layerSize = 6 * 25;

        int pos = (k*layerSize)+(i * 25 + j);
        freq[v[pos]]++;

        // part 2
        if (v[pos] != 2 && !used[i][j])
        {
          used[i][j] = true;
          image[i][j] = v[pos];
        }

        // out << v[pos];
      }
    }

    if (freq[0] < min)
    {
      min = freq[0];
      minFreq = freq;
    }
  }

  for (int i = 0; i < 6; ++i)
  {
    for (int j = 0; j < 25; ++j)
    {
      char c = image[i][j] ? '*' : ' ';
      out << c;
    }

    out << endl;
  }

  cout << minFreq[1] * minFreq[2];


  return 0;
}
