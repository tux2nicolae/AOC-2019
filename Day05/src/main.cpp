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
  ifstream in("..\\..\\Day05\\src\\Day05.in");
  ofstream out("..\\..\\Day05\\src\\Day05.out");

  FStreamReader reader(in);
  vector<int> v = reader.ReadVector();

  int i = -1;
  while (true)
  {
    int opcode = v[++i] % 100;
    if (opcode == 99)
      break;

    int aMode = (v[i] / 100) % 10;
    int bMode = (v[i] / 1000) % 10;
    int cMode = (v[i] / 10000) % 10;

    if (opcode == 1)
    {
      int a = aMode == 1 ? v[++i] : v[v[++i]];
      int b = bMode == 1 ? v[++i] : v[v[++i]];
      int c = v[++i];

      v[c] = a + b;
    }
    else if (opcode == 2)
    {
      int a = aMode == 1 ? v[++i] : v[v[++i]];
      int b = bMode == 1 ? v[++i] : v[v[++i]];
      int c = v[++i];

      v[c] = a * b;
    }
    //
    else if (opcode == 3)
    {
      int a = v[++i];

      // part 1
      // int value = 1

      int value = 5;
      v[a] = value;
    }
    else if (opcode == 4)
    {
      int a = aMode == 1 ? v[++i] : v[v[++i]];
      cout << a << " ";
    }

    //
    else if (opcode == 5)
    {
      int a = aMode == 1 ? v[++i] : v[v[++i]];
      int b = bMode == 1 ? v[++i] : v[v[++i]];
      
      if (a)
        i = b - 1;
    }
    else if (opcode == 6)
    {
      int a = aMode == 1 ? v[++i] : v[v[++i]];
      int b = bMode == 1 ? v[++i] : v[v[++i]];

      if (!a)
        i = b - 1;
    }
    else if (opcode == 7)
    {
      int a = aMode == 1 ? v[++i] : v[v[++i]];
      int b = bMode == 1 ? v[++i] : v[v[++i]];
      int c = v[++i];

      v[c] = a < b;
    }
    else if (opcode == 8)
    {
      int a = aMode == 1 ? v[++i] : v[v[++i]];
      int b = bMode == 1 ? v[++i] : v[v[++i]];
      int c = v[++i];

      v[c] = a == b;
    }
  }


  return 0;
}
