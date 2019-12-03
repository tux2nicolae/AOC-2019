#pragma once

/**
 * Advent of code 2019
 * @author : Nicolae Telechi
 */
namespace AOC
{
  struct Point;

  int Fibonaci(int n, int modulo);
  vector<bool> Eratosthenes(size_t n);

  pair<vector<vector<int>>, set<int>> Lee(const vector<Point> & aCoordonates, vector<vector<int>> & aMap);

  tuple<int, int, int> SubsequenceOfLargestSum(const vector<int> & sequence);

  vector<string> Explode(const string & str, char separator);

  //--------------------------------------------------------------------------
  // implementation details

  namespace detail
  {
    vector<int> GetPartialSums(const vector<int> & sequence);

    vector<vector<int>> MultiplyMatrix(const vector<vector<int>> & first, 
      const vector<vector<int>> & second, int modulo);

    vector<vector<int>> FibonaciMatrix(int n, int modulo);
  }
}
