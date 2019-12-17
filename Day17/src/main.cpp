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

#include "robot.h"

struct Space
{
  bool continuousFeed = false;

  string main = "A,B,A,C,B,A,B,C,C,B";
  string functionA = "L,12,L,12,R,4";
  string functionB = "R,10,R,6,R,4,R,4";
  string functionC = "R,6,L,12,L,12";
  
  char GetInput()
  {
    auto c = mInput.front();
    mInput.erase(0, 1);

    return c;
  };

  int lastC = 0;
  void reportMap(int c)
  {
    assert(c != 0);
    static int reportI = 0, reportJ = 0;

    space[{reportI, reportJ++}] = c;
    cout << char(c);
    lastC = c;

    if (c == 10)
    {
      ++reportI;
      reportJ = 0;
    }
  }
 
  //------------------------------------------------------

  string calculatePath() {
    string str = "";

    while(true)
    {
      if (space.find(robot.GetRight()) != end(space) && space[robot.GetRight()] == '#')
      {
        str += "R,";
        robot.turnRight();
      }
      else if(space.find(robot.GetLeft()) != end(space) && space[robot.GetLeft()] == '#')
      {
        str += "L,";
        robot.turnLeft();
      }

      int frontN = 0;
      while (space.find(robot.GetFront()) != end(space) && space[robot.GetFront()] == '#')
      {
        frontN++;
        robot.moveForward();
      }

      if (frontN == 0)
        break;

      str += to_string(frontN) + ",";
    }

    return str;
  };

  int calculateIntersectionsAlignment()
  {
    minX = numeric_limits<int>::max();
    minY = numeric_limits<int>::max();

    maxX = 0;
    maxY = 0;

    for (auto [pixel, value] : space)
    {
      minX = std::min(minX, pixel.x);
      minY = std::min(minY, pixel.y);

      maxX = std::max(maxX, pixel.x);
      maxY = std::max(maxY, pixel.y);

      if (value == '^')
        robot.setPosition(pixel);
    }

    for (int i = minX + 1; i <= maxX - 1; i++)
    {
      for (int j = minY + 1; j <= maxY - 1; j++)
      {
        if (space[{i, j}] == '#'
          && space[{i - 1, j}] == '#' 
          && space[{i + 1, j}] == '#'
          && space[{i, j - 1}] == '#'
          && space[{i, j + 1}] == '#')
        {
          intersections.push_back({ i, j });
        }
      }
    }

    int sum = 0;
    for (const auto & point : intersections)
    {
      int alignmentParamenter = (point.x - minX) * (point.y - minY);
      sum += alignmentParamenter;
    }

    return sum;
  };

  //------------------------------------------------------

  void printMap(ostream& out)
  {
    int minX = numeric_limits<int>::max();
    int minY = numeric_limits<int>::max();

    int maxX = 0;
    int maxY = 0;

    for (auto pixel : space)
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
        out.width(4);
        out << char(space[{i, j}]);
      }

      out << endl;
    }
  }

  void generateInputData()
  {
    mInput = main + "\n" +
      functionA + "\n" +
      functionB + "\n" +
      functionC + "\n" +
      (continuousFeed ? "y\n" : "n\n");
  }

private:
  Robot robot;
  map<AOC::Point, int> space;

  int minX{};
  int minY{};
  int maxX{};
  int maxY{};

  string mInput;
  vector<AOC::Point> intersections;
};

Space space;
using INT = long long;

class IntCodeComputer {
public:
  IntCodeComputer(const vector<INT>& memory)
    : memory(memory)
  {
  }

  bool run(const vector<INT>& input)
  {
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

      // input
      else if (opcode == 3)
      {
        INT a = getOutputPos(aMode);
        memory[a] = space.GetInput();
      }
      // output
      else if (opcode == 4)
      {
        INT a = getValue(aMode);
        space.reportMap(a);
      }
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
  ifstream in("..\\..\\Day17\\src\\Day17.in");
  ofstream out("..\\..\\Day17\\src\\Day17.out");

  FStreamReader reader(in);

  auto memory = reader.ReadVectorSeparatedByChar<INT>();
  memory.resize(memory.size() + 10000000);

  space.generateInputData();

  vector<INT> input;

  // create computer
  memory[0] = 1;
  IntCodeComputer mapComputer(memory);
  mapComputer.run(input);

  cout << endl;
  cout << "part1:" << space.calculateIntersectionsAlignment() << endl;
  cout << space.calculatePath() << endl;

  // run
  memory[0] = 2;
  IntCodeComputer runComputer(memory);
  runComputer.run(input);

  cout << endl;
  cout << endl;
  cout << "part2:" << space.lastC << endl;

  // space.printMap(out);
  return 0;
}
