#pragma once

/**
 * Advent of code 2019
 * @author : Nicolae Telechi
 */
namespace AOC
{
  struct Coordonate
  {
    int x{ 0 };
    int y{ 0 };

    bool operator < (const Coordonate& second) const
    {
      if (x < second.x)
        return true;
      else if (x == second.x)
        return y < second.y;

      return false;
    }
  };

  int Abs(int number);

  int ManhattenDistance(int x1, int y1, int x2, int y2);
  int ManhattenDistance(const Coordonate & a, const Coordonate& b);

  int Cmmdc(int n, int m);
  int Cmmmc(int n, int m);
}
