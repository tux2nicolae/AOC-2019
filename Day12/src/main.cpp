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
#include <regex>
#include <assert.h>
#include <unordered_set>

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

struct Moon {
  int x{};
  int y{};
  int z{};

  int velocityX{ 0 };
  int velocityY{ 0 };
  int velocityZ{ 0 };

  void move()
  {
    x += velocityX;
    y += velocityY;
    z += velocityZ;
  }

  int getPotentialEnergy() const
  {
    return AOC::Abs(x) + AOC::Abs(y) + AOC::Abs(z);
  }


  int getKineticEnergy() const
  {
    return AOC::Abs(velocityX) + AOC::Abs(velocityY) + AOC::Abs(velocityZ);
  }

  int getEnergy() const
  {
    return getPotentialEnergy() * getKineticEnergy();
  }

  void print()
  {
    cout << x << " " << y << " " << z << "     :     "
      << velocityX << " " << velocityY << " " << velocityZ << endl;
  }
};

int main()
{
  ifstream in("..\\..\\Day12\\src\\Day12.in");
  ofstream out("..\\..\\Day12\\src\\Day12.out");

  FStreamReader reader(in);
  auto lines = reader.ReadVectorOfWords();

  vector<Moon> moonsOriginal;

  // read data
  for (auto& line : lines)
  {
    smatch match;
    regex_match(line, match, regex("<x=(.*), y=(.*), z=(.*)>"));

    assert(match.size() == 4);

    Moon moon{};

    // 1
    moon.x = stoi(match[1]);

    // 2
    moon.y = stoi(match[2]);

    // 3
    moon.z = stoi(match[3]);


    moonsOriginal.push_back(moon);
  }

  auto runStep = [](auto& moons) {
    // update velocity
    for (int i = 0; i < moons.size(); ++i)
    {
      auto& moon1 = moons[i];

      for (int j = i + 1; j < moons.size(); ++j)
      {
        auto& moon2 = moons[j];

        // x
        if (moon1.x < moon2.x)
        {
          moon1.velocityX++;
          moon2.velocityX--;
        }
        else if (moon1.x > moon2.x)
        {
          moon1.velocityX--;
          moon2.velocityX++;
        }

        // y
        if (moon1.y < moon2.y)
        {
          moon1.velocityY++;
          moon2.velocityY--;
        }
        else if (moon1.y > moon2.y)
        {
          moon1.velocityY--;
          moon2.velocityY++;
        }

        // z
        if (moon1.z < moon2.z)
        {
          moon1.velocityZ++;
          moon2.velocityZ--;
        }
        else if (moon1.z > moon2.z)
        {
          moon1.velocityZ--;
          moon2.velocityZ++;
        }
      }
    }

    // move moons
    for (int k = 0; k < moons.size(); ++k)
    {
      auto& moon = moons[k];
      moon.move();
    }
  };
  
  // part1
  auto moons = moonsOriginal;
  for (int i = 1; i <= 1000; ++i)
  {
    runStep(moons);
  }

  int energy = 0;
  for (const auto& moon : moons)
  {
    energy += moon.getEnergy();
  }

  cout << "Part1:" << energy << endl;

  // part 2
  auto searchLoop = [&](auto moons, auto comparator) ->int
  {
    auto initialState = moons;

    int loop = 0;
    while (true)
    {
      runStep(moons);
      ++loop;

      bool found = equal(begin(moons), end(moons), begin(initialState), comparator);
      if (found)
        break;
    }

    return loop;
  };

  // x
  int loopX = searchLoop(moonsOriginal, [](const auto& first, const auto& second) {
      return first.x == second.x && first.velocityX == second.velocityX;
    }
  );

  // y
  int loopY = searchLoop(moonsOriginal, [](const auto& first, const auto& second) {
    return first.y == second.y && first.velocityY == second.velocityY;
    }
  );


  // z
  int loopZ = searchLoop(moonsOriginal, [](const auto& first, const auto& second) {
    return first.z == second.z && first.velocityZ == second.velocityZ;
    }
  );

  cout << "Part2:" << AOC::Cmmmc(AOC::Cmmmc(loopX, loopY), loopZ);

  return 0;
}
