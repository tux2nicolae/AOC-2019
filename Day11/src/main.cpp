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

map<AOC::Point, int> painted;

using INT = long long;

struct Robot
{
  int x{ 0 };
  int y{ 0 };

  char direction{ '^' };

  // less comparator for sort operations
  bool operator < (const Robot& second) const
  {
    if (x < second.x)
      return true;
    else if (x == second.x)
      return y < second.y;

    return false;
  }

  void turnLeft()
  {
    switch (direction)
    {
    case '^':
      direction = '<';
      break;
    case '<':
      direction = 'v';
      break;
    case 'v':
      direction = '>';
      break;
    case '>':
      direction = '^';
      break;
    default:
      break;
    }
  }

  void turnRight()
  {
    switch (direction)
    {
    case '^':
      direction = '>';
      break;
    case '>':
      direction = 'v';
      break;
    case 'v':
      direction = '<';
      break;
    case '<':
      direction = '^';
      break;
    default:
      break;
    }
  }

  void moveForward()
  {
    // move forward
    if (direction == '^')
    {
      x -= 1;
    }
    else if (direction == '>')
    {
      y += 1;
    }
    else if (direction == 'v')
    {
      x += 1;
    }
    else if (direction == '<')
    {
      y -= 1;
    }
  }

  int getColor()
  {
    return painted[{x,y}];
  }

  void paint(int color)
  {
    painted[{x, y}] = color;
  }
};

Robot robot;

class IntCodeComputer {
public:
  IntCodeComputer(const vector<INT>& memory)
    : memory(memory)
  {
  }

  bool run(const vector<INT>& input)
  {
    INT firstInput = true;
    INT firstOutput = true;

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
        INT value = robot.getColor();

        INT a = getOutputPos(aMode);
        memory[a] = value;
      }
      else if (opcode == 4)
      {
        INT a = getValue(aMode);

        mOutput = a;

        if(firstOutput)
        {
          INT color = a;

          robot.paint(color);
          firstOutput = false;
        }
        else
        {
          INT direction = a;

          if (direction == 0)
            robot.turnLeft();
          else if (direction == 1)
            robot.turnRight();

          robot.moveForward();

          firstOutput = true;
        }
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
  ifstream in("..\\..\\Day11\\src\\Day11.in");
  ofstream out("..\\..\\Day11\\src\\Day11.out");

  FStreamReader reader(in);

  auto v = reader.ReadVectorSeparatedByChar<INT>();
  v.resize(v.size() + 10000000);

  IntCodeComputer computer(v);


  robot.paint(1);

  vector<INT> input;
  computer.run(input);

  int minX = numeric_limits<int>::max();
  int minY = numeric_limits<int>::max();

  int maxX = 0;
  int maxY = 0;

  for (auto pixel : painted)
  {
    minX = std::min(minX, pixel.first.x);
    minY = std::min(minY, pixel.first.y);

    maxX = std::max(maxX, pixel.first.x);
    maxY = std::max(maxY, pixel.first.y);
  }

  for (int i = minX; i <= maxX; i++)
  {
    for (int j = minY; j <= maxY; j++)
    {
      out << (painted[{i,j}] ? '#':' ');
    }

    out << endl;
  }

  return 0;
}
