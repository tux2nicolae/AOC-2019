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

bool isCollinear(int x1, int y1, int x2, int y2, int x3, int y3)
{
  return (y3 - y2) * (x2 - x1) == (y2 - y1) * (x3 - x2);
}

int main()
{
  ifstream in("..\\..\\Day10\\src\\Day10.in");
  ofstream out("..\\..\\Day10\\src\\Day10.out");

  FStreamReader reader(in);
  auto m = reader.ReadVectorOfWords();

  vector<vector<int>> counts(1000, vector<int>(1000, 0));

  auto pointsBeetweenThem = [&](int i1, int j1, int i2, int j2) -> int {
    int count = 0;

    for (int i = (i1 <= i2 ? i1 : i2); i <= (i2 >= i1 ? i2 : i1); ++i)
    {
      for (int j = (j1 <= j2 ? j1 : j2); j <= (j2 >= j1 ? j2 : j1); ++j)
      {
        if (m[i][j] != '#' || (i == i1 && j == j1) || (i == i2 && j == j2))
          continue;

        if (isCollinear(i1, j1, i, j, i2, j2))
          count++;
      }
    }

    return count;
  };

  for (int i1 = 0; i1 < m.size(); ++i1)
  {
    for (int j1 = 0; j1 < m[0].size(); ++j1)
    {
      if (m[i1][j1] != '#')
        continue;

      // check if exists
      for (int i2 = 0; i2 < m.size(); ++i2)
      {
        for (int j2 = 0; j2 < m[0].size(); ++j2)
        {
          if (m[i2][j2] != '#' || (i1 == i2 && j1 == j2))
            continue;

          if (pointsBeetweenThem(i1, j1, i2, j2))
            continue;

          counts[i1][j1]++;
        }
      }
    }
  }

  AOC::Point station;

  // part 1
  int best = 0;
  for (int i = 0; i < m.size(); ++i)
  {
    for (int j = 0; j < m[0].size(); ++j)
    {
      if (counts[i][j] > best)
      {
        best = counts[i][j];
        station = { i, j };
      }
    }
  }

  cout << best << endl;

  // part 2
  auto getDirectAsteroids = [&]() {
    vector<AOC::Point> ones;
    for (int i = 0; i < m.size(); ++i)
    {
      for (int j = 0; j < m[0].size(); ++j)
      {
        if (m[i][j] != '#' || (i == station.x && j == station.y))
        {
          out << ((i == station.x && j == station.y) ? "X" : ".");
          continue;
        }

        auto pointsBeetween = pointsBeetweenThem(i, j, station.x, station.y);
        if (pointsBeetween == 0)
        {
          out << "#";
          ones.push_back({ i,j });
        }
        else
        {
          out << ".";
        }
      }

      out << endl;
    }

    return ones;
  };


  auto angle = [&](const AOC::Point& a)
  {
    AOC::Point translatedToOrigin{ a.x - station.x, a.y - station.y };
    return atan2(translatedToOrigin.y, translatedToOrigin.x);
  };

  auto asteroids = getDirectAsteroids();
  std::sort(begin(asteroids), end(asteroids), [&](const AOC::Point & a, const AOC::Point & b)
    {
      return angle(a) > angle(b);
    });

  auto ret = asteroids[200 - 1];
  cout << ret.y * 100 + ret.x;

  return 0;
}
