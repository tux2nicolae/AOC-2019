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
  ifstream in("..\\..\\Day02\\src\\Day02.in");
  ofstream out("..\\..\\Day02\\src\\Day02.out");

  FStreamReader reader(in);
  vector<int> x = reader.ReadVector();

  vector<int> v = x;

  for (int j = 1; j <= 99; j++)
  {
    for (int k = 1; k <= 99; k++)
    {
      v[1] = j;
      v[2] = k;

      for (int i = 0; i < v.size(); ++i)
      {
        int opcode = v[i];
        if (opcode == 99)
          break;

        int a = v[++i];
        int b = v[++i];
        int c = v[++i];

        if (opcode == 1)
          v[c] = v[a] + v[b];
        else if (opcode == 2)
          v[c] = v[a] * v[b];
      }

      if (v[0] == 19690720)
      {
        cout << 100 * v[1] + v[2];
        return 0;
      }

      v = x;
    }
  }

  return 0;
}
