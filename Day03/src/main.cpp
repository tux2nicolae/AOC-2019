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


struct Val
{
  char id{};
  int line1Sum{};
  int line2Sum{};
};

AOC::Coordonate init{ 0, 0 };
unordered_map<int, unordered_map<int, Val>> v;

map<AOC::Coordonate, Val> intersections;

void run(std::istringstream& stream, char id)
{
  char _, control = ' ';
  int distance = 0;

  AOC::Coordonate o = init;

  int steps = 0;
  auto compute = [&](auto& p)
  {
    ++steps;

    if (p.id != id && id == 1)
    {
      p.line1Sum = steps;
    }

    if (p.id != id && id == 2)
    {
      p.line2Sum = steps;
    }

    if (p.id != 0 && p.id != id)
    {
      intersections[o] = p;
    }

    p.id = id;
  };

  while (stream.good())
  {
    stream >> control;
    stream >> distance;
    stream >> _;

    if (control == 'U')
    {
      for (int i = 1; i <= distance; i++)
      {
        --o.x;

        auto& p = v[o.x][o.y];
        compute(p);
      }
    }
    else if (control == 'D')
    {
      for (int i = 1; i <= distance; i++)
      {
        ++o.x;

        auto& p = v[o.x][o.y];
        compute(p);
      }
    }
    else if (control == 'R')
    {
      for (int i = 1; i <= distance; i++)
      {
        ++o.y;

        auto& p = v[o.x][o.y];
        compute(p);
      }
    }
    else if (control == 'L')
    {
      for (int i = 1; i <= distance; i++)
      {
        --o.y;

        auto& p = v[o.x][o.y];
        compute(p);
      }
    }
  };
}

int main()
{
  ifstream in("..\\..\\Day03\\src\\Day03.in");
  ofstream out("..\\..\\Day03\\src\\Day03.out");

  string firstPath;
  string secondPath;

  getline(in, firstPath);
  getline(in, secondPath);

  std::istringstream s1(firstPath);
  std::istringstream s2(secondPath);

  run(s1, 1);
  run(s2, 2);

  // part 1
  int min = 100000;
  for (auto& [cord, p] : intersections)
  {
    if (AOC::ManhattenDistance(cord.x, cord.y, init.x, init.y) < min)
    {
      min = AOC::ManhattenDistance(cord.x, cord.y, init.x, init.y);
    }
  }

  // part 2
  int minSteps = 100000;
  for (auto& [cord, p] : intersections)
  {
    if (minSteps > (p.line1Sum + p.line2Sum))
    {
      minSteps = p.line1Sum + p.line2Sum;
    }
  }

  out << min << " " << minSteps << endl;

  return 0;
}
