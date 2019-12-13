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

vector<vector<char>> m(1000, vector<char>(1000, ' '));

int s = 0;
struct Joystick
{
  int getPosition()
  {
    AOC::Point paddle = getPaddlePosition();
    AOC::Point ball = getBallPosition();

    if (paddle.x < ball.x)
      return 1;
    else if (paddle.x == ball.x)
      return 0;
    else
      return -1;
  };

private:

  AOC::Point getBallPosition()
  {
    for (int i = 0; i < m.size(); ++i)
    {
      for (int j = 0; j < m[i].size(); ++j)
      {
        if (m[i][j] == '*')
          return { i, j };
      }
    }

    return { 0, 0 };
  }

  AOC::Point getPaddlePosition()
  {
    for (int i = 0; i < m.size(); ++i)
    {
      for (int j = 0; j < m[i].size(); ++j)
      {
        if (m[i][j] == '|')
          return { i, j };
      }
    }

    return { 0, 0 };
  }

};

Joystick joystick;

class IntCodeComputer {
public:
  IntCodeComputer(const vector<INT>& memory)
    : memory(memory)
  {
  }

  bool run(const vector<INT>& input)
  {
    INT outputN = 0;
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
      }
      else if (opcode == 2)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);

        INT c = getOutputPos(cMode);
        memory[c] = a * b;
      }

      //
      else if (opcode == 3)
      {
        INT value = joystick.getPosition();

        INT a = getOutputPos(aMode);
        memory[a] = value;
      }
      else if (opcode == 4)
      {
        INT a = getValue(aMode);

        if (outputN % 3 == 0)
        {
          INT x = a;
          outputPos.x = x;
        }
        if (outputN % 3 == 1)
        {
          INT y = a;
          outputPos.y = y;
        }
        if (outputN % 3 == 2)
        {
          INT titleId = a;

          if (outputPos.x == -1 && outputPos.y == 0)
          {
            cout << "Score:" << titleId << endl;
          }
          else
          {
            // part 1
            if (titleId == 2)
              s++;

            char tile = ' ';
            switch(titleId)
            {
            case 0: tile = ' ';
              break;
            case 1: tile = 'W';
              break;
            case 2: tile = '#';
              break;
            case 3: tile = '|';
              break;
            case 4: tile = '*';
              break;
            }

            m[outputPos.x][outputPos.y] = tile;
          }
        }

        outputN++;
        mOutput = a;
      }

      //
      else if (opcode == 5)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);

        if (a)
          i = b - 1;
      }
      else if (opcode == 6)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);

        if (!a)
          i = b - 1;
      }
      else if (opcode == 7)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);


        INT c = getOutputPos(cMode);
        memory[c] = a < b;
      }
      else if (opcode == 8)
      {
        INT a = getValue(aMode);
        INT b = getValue(bMode);

        INT c = getOutputPos(cMode);
        memory[c] = a == b;
      }
      else if (opcode == 9)
      {
        INT a = getValue(aMode);
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

int main()
{
  ifstream in("..\\..\\Day13\\src\\Day13.in");
  ofstream out("..\\..\\Day13\\src\\Day13.out");

  FStreamReader reader(in);

  auto v = reader.ReadVectorSeparatedByChar<INT>();
  v.resize(v.size() + 10000000);

  // part 2
  v[0] = 2;
  IntCodeComputer computer(v);

  vector<INT> input;
  computer.run(input);

  for (int i = 0; i < 45; ++i)
  {
    for (int j = 0; j < 24; ++j)
    {
      out << m[i][j];
    }

    out << endl;
  }

  cout << s;

  return 0;
}
