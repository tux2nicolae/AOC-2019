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
#include <stack>
#include <queue>
#include <thread>
#include <chrono>

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

struct Space
{
  int sum = 0;
  void report(int c)
  {
    sum += c;
  }
};

Space space;
using INT = long long;

class IntCodeComputer {
public:
  IntCodeComputer(const vector<INT>& memory)
    : memory(memory)
  {
  }

  bool debug = false;

  int run(const vector<INT>& input)
  {
    static int inputN = 0;
    AOC::Point outputPos;

    while (true)
    {
      INT opcode = memory[++i] % 100;
      if (opcode == 99)
      {
        return true;
      }

      INT aMode = (memory[i] / 100) % 10;
      INT bMode = (memory[i] / 1000) % 10;
      INT cMode = (memory[i] / 10000) % 10;

      if (opcode == 1)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);

        INT c = getOutputPos(cMode);
        memory[c] = a + b;

        if (debug)
        {
          cout << a << " + " << b << endl;
        }
      }
      else if (opcode == 2)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);

        INT c = getOutputPos(cMode);
        memory[c] = a * b;

        if (debug)
        {
          cout << a << " * " << b << endl;
        }
      }

      // input
      else if (opcode == 3)
      {
        INT a = getOutputPos(aMode);
        memory[a] = input[inputN++ % 2];

        if (debug)
        {
          cout << "input : " << a << endl;
        }
      }
      // output
      else if (opcode == 4)
      {
        INT a = getValue(aMode);
        space.report(a);

        if (debug)
        {
          cout << "output : " << a << endl;
        }
        return a;
      }
      else if (opcode == 5)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);

        if (a)
          i = b - 1;

        if (debug)
        {
          cout << "jump(" << bool(a) <<") : " << i << endl;
        }
      }
      else if (opcode == 6)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);

        if (!a)
          i = b - 1;

        if (debug)
        {
          cout << "jump(" << bool(!a) << ") : " << i << endl;
        }
      }
      else if (opcode == 7)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);


        INT c = getOutputPos(cMode);
        memory[c] = a < b;

        //cout << a << "<" << b << endl;
      }
      else if (opcode == 8)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);

        INT c = getOutputPos(cMode);
        memory[c] = a == b;

        if (debug)
        {
          cout << a << "==" << b << endl;
        }
      }
      else if (opcode == 9)
      {
        INT a = getValue(aMode);
        mRelativeBase += a;

        if (debug)
        {
          cout << "base : " << mRelativeBase << endl;
        }
      }
    }

    return 0;
  }

  INT GetOutput() {
    return mOutput;
  };

private:
  // address pointer
  INT i{ -1 };

  // memory
  vector<INT> memory;

  // output
  INT mOutput{ 0 };

  // relative base
  INT mRelativeBase{ 0 };

  //---------------------------------------

  INT getValue(INT mode)
  {
    if (mode == 0)
    {
      size_t pos = memory[++i];
      return memory[pos];
    }
    else if (mode == 1)
    {
      return memory[++i];
    }
    else if (mode == 2)
    {
      return memory[memory[++i] + mRelativeBase];
    }

    return 0;
  }

  INT getOutputPos(INT mode)
  {
    if (mode == 0)
      return memory[++i];
    if (mode == 2)
      return memory[++i] + mRelativeBase;

    assert(mode != 1);
    return 0;
  }
};


vector<vector<int>> map(10000, vector<int>(1000, 0));

int main()
{
  ifstream in("..\\..\\Day19\\src\\Day19.in");
  ofstream out("..\\..\\Day19\\src\\Day19.out");

  FStreamReader reader(in);

  auto memory = reader.ReadVectorSeparatedByChar<INT>();
  memory.resize(memory.size() + 100);


  vector<INT> input;
  input.push_back(3);
  input.push_back(5);

  IntCodeComputer computer(memory);
  auto result = computer.run(input);
  
  auto isInRange = [](int i, int j) {
    int addRight = (i / 23);

    int right = (i * 2) + addRight;
    int left = right - ((right - i) / 2.5);
    
    return left <= j && j <= right;
  };

  auto check = [&](int i, int j) -> bool {
    if (i < 99 || j < 99)
      return false;

    int found = true;
    for (int k = 0; k < 100; ++k)
    {
      vector<INT> inputVertical;
      inputVertical.push_back(i - k);
      inputVertical.push_back(j + 99);

      IntCodeComputer computerVertical(memory);
      found = found && computerVertical.run(inputVertical);
      if (!found)
        return false;

      // horizontal
      vector<INT> inputHorizontal;
      inputHorizontal.push_back(i);
      inputHorizontal.push_back(j + k);

      IntCodeComputer computer(memory);
      found = found && computer.run(inputHorizontal);
      if (!found)
        return false;
    }

    return true;
  };

  for (int i = 0; i < 10000; ++i)
  {
    bool isFirst = true;
    bool found = false;

    for (int j = 0; j < 10000; ++j)
    {
      if (/*isInRange(i, j) &&*/ isFirst)
      {
        vector<INT> input;
        input.push_back(i);
        input.push_back(j);

        IntCodeComputer computer(memory);
        auto result = computer.run(input);

        if (result)
        {
          isFirst = false;
          found = check(i, j);
          if (found)
          {
            cout << "found at : " << i << " " << j << endl;
            cout << "solution : " << i - 99 << " " << j << endl;
          }
          break;
        }
      }
    }

    if (found)
    {
      break;
    }
  }

  return 0;
}
