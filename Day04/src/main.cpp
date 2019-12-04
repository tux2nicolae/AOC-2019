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

bool twoAdjiacent(int a)
{
  int x = -1;
  int nr = 1;

  while (a)
  {
    if (a % 10 == x)
    {
      nr++;
    }
    else
    {
      if (nr == 2)
        return true;

      nr = 1;
    }

    x = a % 10;
    a /= 10;
  }

  if (nr == 2)
    return true;

  return false;
}

bool neverDecrease(int a)
{
  int x = 10000;
  while (a)
  {
    if (a % 10 > x)
      return false;

    x = a % 10;
    a /= 10;
  }

  return true;
}



int main()
{
  ifstream in("..\\..\\Day04\\src\\Day04.in");
  ofstream out("..\\..\\Day04\\src\\Day04.out");

  int a = 152085;
  int b = 670283;

  int s = 0;
  for (int i = a; i <= b; ++i)
  {
    s += (twoAdjiacent(i) && neverDecrease(i));
  }

  cout << s;

  return 0;
}
