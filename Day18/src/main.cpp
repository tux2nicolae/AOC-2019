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
#include <queue>
#include <bitset>

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

const int kChars = 26;
const char kMaxLetterLower = 'z';
const char kMaxLetter = 'Z';

AOC::Point initialPoint{ 0, 0 };
std::map<char, AOC::Point> inputKeys{};
std::map<char, AOC::Point> inputDoors{};

//--------------------------------------------------------------

// char -> cost
unordered_map<char, int> calculateDistance(const vector<string>& map, AOC::Point initialPoint, int initialCost)
{
  unordered_map<char, int> keyDistances;

  std::map<AOC::Point, int> distances;
  distances[initialPoint] = initialCost;

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

      // not having a free way or not having a cost
      char c = map[to.x][to.y];
      if (c == '#' || distances.find(to) != end(distances))
        continue;
      
      if ('a' <= c && c <= kMaxLetterLower)
      {
        keyDistances[toupper(c)] = distances[from] + 1;
      }

      distances[to] = distances[from] + 1;
      unvisited.push(to);
    }
  }

  return keyDistances;
};

// graph dependencies
vector<char> calculateDependencies(const vector<string>& map, AOC::Point startPoint)
{
  std::map<AOC::Point, int> distances;
  distances[startPoint] = 0;

  queue<AOC::Point> unvisited;
  unvisited.push(startPoint);

  // run
  while (!unvisited.empty())
  {
    auto from = unvisited.front();
    unvisited.pop();

    static const int directionX[4] = { -1,  0, 1, 0 };
    static const int directionY[4] = { 0,  1, 0, -1 };

    // found
    if (from == initialPoint)
    {
      vector<char> dependencies;

      while (!(from == startPoint))
      {
        for (int i = 0; i < 4; ++i)
        {
          AOC::Point to;
          to.x = from.x + directionX[i];
          to.y = from.y + directionY[i];

          if (distances.find(to) != end(distances) && distances[to] == distances[from] - 1)
          {
            int c = map[to.x][to.y];
            if ('A' <= c && c <= kMaxLetter)
              dependencies.push_back(c);

            from = to;
            break;
          }
        }
      }

      return dependencies;
    }

    //
    for (int i = 0; i < 4; ++i)
    {
      AOC::Point to;
      to.x = from.x + directionX[i];
      to.y = from.y + directionY[i];

      char c = map[to.x][to.y];
      if (c == '#' || distances.find(to) != end(distances))
      {
        continue;
      }

      distances[to] = distances[from] + 1;
      unvisited.push(to);
    }
  }

  return {};
};

void printMap(ofstream& out, const vector<string> & map)
{
  for (int i = 0; i < map.size(); i++)
  {
    for (int j = 0; j < map[0].size(); j++)
    {
      char c = map[i][j];
      if ('a' <= c && c <= 'z'/* && find(begin(keys), end(keys), Key{ c }) != end(keys)*/)
      {
        //out << map[i][j];
        out << c;
        continue;
      }

      if ('A' <= c && c <= 'Z' /*&& find(begin(keys), end(keys), Key{ char(tolower(c)) }) != end(keys)*/)
      {
        //out << map[i][j];
        out << c;
        continue;
      }

      if ('a' <= c && c <= 'z')
      {
        out << "*";
        continue;
      }

      if ('A' <= c && c <= 'Z' )
      {
        out << "#";
        continue;
      }

      if (c == '#')
      {
        out << ".";
        continue;
      }

      out << ' ';
    }

    out << endl;
  }
}

unordered_map<char, vector<char>> graph;
unordered_map<char, vector<char>> dependencyGraph;
unordered_map<char, unordered_map<char, int>> distances;
vector<char> dependenciesCount(kChars, 0);

std::bitset<26> used;
unordered_map<char, unordered_map<uint64_t, int>> cache;

int backtrack(vector<string> & map, char keyName, int depth)
{
  if (depth == kChars)
    return 0;

  int minSum = numeric_limits<int>::max();

  for (char nextKeyName = 'A'; nextKeyName <= kMaxLetter; ++nextKeyName)
  {
    if (used[nextKeyName - 'A'] || dependenciesCount[nextKeyName - 'A'])
      continue;

    const auto& neighbors = dependencyGraph[nextKeyName];

    used[nextKeyName - 'A'] = true;
    for (auto neighbor: neighbors)
      dependenciesCount[neighbor - 'A']--;

    int subgraphSum = 0;

    // find in cache
    if (auto it = cache.find(nextKeyName); it != end(cache) 
      && it->second.find(used.to_ullong()) != end(it->second))
    {
      subgraphSum = it->second.at(used.to_ullong());
    }
    else
    {
      subgraphSum = backtrack(map, nextKeyName, depth + 1);
      cache[nextKeyName][used.to_ullong()] = subgraphSum;
    }

    minSum = min(minSum, subgraphSum + distances[keyName][nextKeyName]);

    // revert
    used[nextKeyName - 'A'] = false;
    for (auto neighbor : neighbors)
      dependenciesCount[neighbor - 'A']++;
  }

  return minSum;
}

int main()
{
  ifstream in("..\\..\\Day18\\src\\Day18.in");
  ofstream out("..\\..\\Day18\\src\\Day18.out");

  FStreamReader reader(in);
  auto map = reader.ReadVectorOfWords();
  
  // read data
  for(int i = 0; i < map.size(); ++i)
  {
    for(int j = 0; j < map[0].size(); ++j)
    {
      if (map[i][j] == '@')
      {
        initialPoint = { i, j };
        map[i][j] = '.';
      }
      else if ('a' <= map[i][j] && map[i][j] <= 'z')
      {
        inputKeys[map[i][j]] = { i, j };
      }
      else if ('A' <= map[i][j] && map[i][j] <= 'Z')
      {
        inputDoors[map[i][j]] = { i, j };
      }
    }
  }

  //-----------------------------------------------------------------------
  for (auto [keyName, keyPosition] : inputKeys)
  {
    out << keyName << "     |    ";

    vector<char> dependencies = calculateDependencies(map, keyPosition);
    for (const auto& dependecy : dependencies)
    {
      dependenciesCount[toupper(keyName) - 'A']++;
      graph[toupper(keyName)].push_back(toupper(dependecy));
      dependencyGraph[toupper(dependecy)].push_back(toupper(keyName));

      out << dependecy << " ";
    }

    out << endl;
  }

  //-----------------------------------------------------------------------


  distances['0'] = calculateDistance(map, initialPoint, 0);
  for (auto [keyName, keyPosition] : inputKeys)
  {
    distances[toupper(keyName)] = (calculateDistance(map, keyPosition, 0));
  }


  //-----------------------------------------------------------------------

  cout << backtrack(map, '0', 0);

  // printMap(out, map, keys);
  // cout << minCost;

  return 0;
}
