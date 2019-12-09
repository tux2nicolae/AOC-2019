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

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

using INT = long long;

class Amplyfier {
public:
  Amplyfier(const vector<INT> & memory)
    : v(memory)
  {
  }

  INT readMode(INT mode)
  {
    if (mode == 0)
    {
      size_t pos = v[++i];
      return v[pos];
    }
    else if (mode == 1)
    {
      return v[++i];
    }
    else if (mode == 2)
    {
      return v[v[++i] + mRelativeBase];
    }

    return 0;
  }

  INT readModeOutput(INT mode)
  {
    if (mode == 0)
      return v[++i];
    if(mode == 2)
      return v[++i] + mRelativeBase;

    return 0;
  }

  bool run(const vector<INT>& input)
  {
    INT inputN = 0;
    while (true)
    {
      INT opcode = v[++i] % 100;
      if (opcode == 99)
      {
        return true;
      }

      INT aMode = (v[i] / 100) % 10;
      INT bMode = (v[i] / 1000) % 10;
      INT cMode = (v[i] / 10000) % 10;

      if (opcode == 1)
      {
        INT a = readMode(aMode);
        INT b = readMode(bMode);

        INT c = readModeOutput(cMode);
        v[c] = a + b;
      }
      else if (opcode == 2)
      {
        INT a = readMode(aMode);
        INT b = readMode(bMode);

        INT c = readModeOutput(cMode);
        v[c] = a * b;
      }

      //
      else if (opcode == 3)
      {
        INT a = readModeOutput(aMode);

        INT value = input[inputN++];
        v[a] = value;
      }
      else if (opcode == 4)
      {
        INT a = readMode(aMode);

        mOutput = a;
        cout << mOutput << " ";
        //return false;
      }

      //
      else if (opcode == 5)
      {
        INT a = readMode(aMode);
        INT b = readMode(bMode);

        if (a)
          i = b - 1;
      }
      else if (opcode == 6)
      {
        INT a = readMode(aMode);
        INT b = readMode(bMode);

        if (!a)
          i = b - 1;
      }
      else if (opcode == 7)
      {
        INT a = readMode(aMode);
        INT b = readMode(bMode);

        
        INT c = readModeOutput(cMode);
        v[c] = a < b;
      }
      else if (opcode == 8)
      {
        INT a = readMode(aMode);
        INT b = readMode(bMode);

        INT c = readModeOutput(cMode);
        v[c] = a == b;
      }
      else if (opcode == 9)
      {
        INT a = readMode(aMode);
        mRelativeBase += a;
      }
    }

    return true;
  }

  INT GetOutput() {
    return mOutput;
  };

private:
  // address pointer
  INT i{ -1 };

  // memory
  vector<INT> v;

  // output
  INT mOutput{ 0 };

  INT mRelativeBase{ 0 };
};

int main()
{
  ifstream in("..\\..\\Day09\\src\\Day09.in");
  ofstream out("..\\..\\Day09\\src\\Day09.out");

  FStreamReader reader(in);
  auto v = reader.ReadVectorSeparatedByChar();
  
  v.resize(v.size() + 10000000);
  Amplyfier amplifiers(v);

  vector<INT> input;
  input.push_back(2);

  amplifiers.run(input);

  return 0;
}
