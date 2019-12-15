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

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

struct Space
{
  optional<int> getNextDroidDirection()
  {
    int nextDirection = 0;
    for (int i = 1; i <= 4; ++i)
    {
      auto nextPos = GetNextPosition(i);
      if (space.find(nextPos) == end(space))
      {
        nextDirection = i;
        break;
      }
    }

    if (nextDirection != 0)
    {
      directionsStack.push(nextDirection);

      waitingDirection = nextDirection;
      return nextDirection;
    }

    if (directionsStack.empty())
    {
      cout << "Stop!" << endl;
      return nullopt;
    }

    auto backwards = GetOpositeDirection(directionsStack.top());
    directionsStack.pop();

    waitingDirection = backwards;
    return backwards;
  }

  void reportStatus(int status)
  {
    auto nextPos = GetNextPosition(waitingDirection);
    space[nextPos] = status;

    // cancel going there
    if (status == 0)
    {
      assert(!directionsStack.empty());
      directionsStack.pop();
      return;
    }

    // move forward
    droid = nextPos;

    if (status == 2)
    {
      cout << "EVRICA!!! Pos: (" << droid.x << "," << droid.y << ")" << endl;
      oxygen = droid;
    }
  }

  //------------------------------------------------------

  map<AOC::Point, int>  calculateDistances(AOC::Point initialPoint)
  {
    map<AOC::Point, int> distances;

    queue<AOC::Point> unvisited;
    unvisited.push(initialPoint);

    // run
    while (!unvisited.empty())
    {
      auto from = unvisited.front();
      unvisited.pop();

      static const int directionX[4] = { -1,  0, 1, 0 };
      static const int directionY[4] = { 0,  1, 0, -1 };

      for (int i = 0; i < 4; ++i)
      {
        AOC::Point to;
        to.x = from.x + directionX[i];
        to.y = from.y + directionY[i];

        // not wall and not having a cost
        if (space[to] == 0 || distances.find(to) != end(distances))
          continue;

        distances[to] = distances[from] + 1;
        unvisited.push(to);
      }
    }

    return distances;
  };

  //------------------------------------------------------

  void printMap(ofstream& out, const map<AOC::Point, int> & distances)
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

        // starting position
        if (i == 0 && j == 0)
        {
          out << "D";
          continue;
        }

        // oxygen mask
        if (space[{i, j}] == 2)
        {
          out << "X";
          continue;
        }

        out << (space[{i, j}] == 0 ? "#" : to_string(distances.at({ i, j })));
      }

      out << endl;
    }
  }

  AOC::Point getOxygenPosition()
  {
    return oxygen;
  }

private:
  int waitingDirection{ 0 };
  std::stack<int> directionsStack;

  map<AOC::Point, int> space;
  AOC::Point droid{ 0, 0 };
  AOC::Point oxygen{ 0, 0 };

  int GetOpositeDirection(int direction)
  {
    // north
    if (direction == 1)
    {
      return 2;
    }
    // south 
    else if (direction == 2)
    {
      return 1;
    }
    // west
    else if (direction == 3)
    {
      return 4;
    }
    // east
    else if (direction == 4)
    {
      return 3;
    }

    assert(false);
    return 0;
  }

  AOC::Point GetNextPosition(int direction)
  {
    AOC::Point nextPosition(droid);

    // north
    if (direction == 1)
    {
      nextPosition.x -= 1;
    }
    // south 
    else if (direction == 2)
    {
      nextPosition.x += 1;
    }
    // west
    else if (direction == 3)
    {
      nextPosition.y -= 1;
    }
    // east
    else if (direction == 4)
    {
      nextPosition.y += 1;
    }

    return nextPosition;
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

      // input
      else if (opcode == 3)
      {
        auto value = space.getNextDroidDirection();
        if (!value)
          break;

        INT a = getOutputPos(aMode);
        memory[a] = *value;
      }
      // output
      else if (opcode == 4)
      {
        INT a = getValue(aMode);
        space.reportStatus(a);
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
  ifstream in("..\\..\\Day15\\src\\Day15.in");
  ofstream out("..\\..\\Day15\\src\\Day15.out");

  FStreamReader reader(in);

  auto v = reader.ReadVectorSeparatedByChar<INT>();
  v.resize(v.size() + 10000000);

  IntCodeComputer computer(v);

  vector<INT> input;
  computer.run(input);

  auto distances = space.calculateDistances({0, 0});
  cout << "Part 1:" << distances[space.getOxygenPosition()] << endl;

  // part 2
  auto oxygenDistances = space.calculateDistances({ 18, 18 });
  auto maxDistanceIt = max_element(begin(oxygenDistances), end(oxygenDistances), [](auto & a, auto & b) {
    return a.second < b.second;
  });

  cout << "Part 2:" << maxDistanceIt->second << endl;

  space.printMap(out, oxygenDistances);

  return 0;
}
