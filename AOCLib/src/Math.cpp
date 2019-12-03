/**
 * Advent of code 2019
 * @author : Nicolae Telechi
 */
#include "pch.h"
#include "Math.h"

int AOC::Abs(int number)
{
  return number > 0 ? number : -number;
}

int AOC::ManhattenDistance(int x1, int y1, int x2, int y2)
{
  return Abs(x1 - x2) + Abs(y1 - y2);
}

int AOC::ManhattenDistance(const Coordonate& a, const Coordonate& b)
{
  return AOC::ManhattenDistance(a.x, a.y, b.x, b.y);
}

int AOC::Cmmdc(int n, int m)
{
  if (n == 0)
    return m;

  return Cmmdc(m % n, n);
}

int AOC::Cmmmc(int n, int m)
{
  return (n * m) / Cmmdc(n, m);
}
