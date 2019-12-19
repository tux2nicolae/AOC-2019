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


struct Robot{
  char id{};
  AOC::Point position;
};

vector<Robot> robots;

std::map<char, AOC::Point> inputKeys{};
std::map<char, AOC::Point> inputDoors{};

// optimization for backtracking algorithm
char kMaxLetter = 'A';

unordered_map<char, vector<char>> graph;
unordered_map<char, vector<char>> dependencyGraph;
unordered_map<char, unordered_map<char, int>> distances;

std::bitset<26> used;
vector<char> dependenciesCount(26, 0);

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
      
      if ('a' <= c && c <= tolower(kMaxLetter))
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
    auto it = find_if(begin(robots), end(robots), [&](auto& robot) {
      return robot.position == from;
    });

    if (it != end(robots))
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

int getKeyId(char keyName)
{
  return toupper(keyName) - 'A';
}

unordered_map<string, unordered_map<uint64_t, int>> cache;
int backtracking(vector<string> & map, const vector<char> & robots, size_t lettersCount)
{
  if (lettersCount == 0)
    return 0;

  int minSum = numeric_limits<int>::max();
  for (char nextKeyName = 'A'; nextKeyName <= kMaxLetter; ++nextKeyName)
  {
    if (used[getKeyId(nextKeyName)] || dependenciesCount[getKeyId(nextKeyName)])
      continue;

    auto getRobotIndex = [&]() -> int {
      for (int i = 0; i < robots.size(); ++i)
      {
        auto robot = robots[i];
        if (distances[robot][nextKeyName])
          return i;
      }

      assert(false);
      return -1;
    };

    const int robotIndex = getRobotIndex();
    const int robotId = robots[robotIndex];

    const auto& neighbors = dependencyGraph[nextKeyName];

    // remove dependencies
    used[getKeyId(nextKeyName)] = true;
    for (auto neighbor: neighbors)
      dependenciesCount[getKeyId(neighbor)]--;

    vector<char> nextRobots = robots;
    nextRobots[robotIndex] = nextKeyName;

    auto robotsHash = reduce(begin(nextRobots), end(nextRobots), string(), [](auto current, auto& val) {
      return current + val;
    });

    // find in cache
    int subgraphSum = 0;
    if (auto it = cache.find(robotsHash); it != end(cache) 
      && it->second.find(used.to_ullong()) != end(it->second))
    {
      subgraphSum = it->second.at(used.to_ullong());
    }
    else
    {
      subgraphSum = backtracking(map, nextRobots, lettersCount - 1);
      cache[robotsHash][used.to_ullong()] = subgraphSum;
    }

    minSum = min(minSum, subgraphSum + distances[robotId][nextKeyName]);

    // add dependencies back
    used[getKeyId(nextKeyName)] = false;
    for (auto neighbor : neighbors)
      dependenciesCount[getKeyId(neighbor)]++;
  }

  return minSum;
}

int main()
{
  ifstream in("..\\..\\Day18\\src\\Day18.in");
  ofstream out("..\\..\\Day18\\src\\Day18.out");

  FStreamReader reader(in);
  auto map = reader.ReadVectorOfWords();
  
  //-----------------------------------------------------------------------
  // read data

  char robotCharId = '0';
  for(int i = 0; i < map.size(); ++i)
  {
    for(int j = 0; j < map[0].size(); ++j)
    {
      char c = map[i][j];
      if (c == '@')
      {
        robots.push_back(Robot{ robotCharId++, AOC::Point{ i, j } });
        c = '.';
      }
      else if ('a' <= c && c <= 'z')
      {
        inputKeys[map[i][j]] = { i, j };
        kMaxLetter = std::max((int)kMaxLetter, toupper(c));
      }
      else if ('A' <= c && c <= 'Z')
      {
        inputDoors[c] = { i, j };
      }
    }
  }

  //-----------------------------------------------------------------------
  // calculate dependencies

  for (auto [keyName, keyPosition] : inputKeys)
  {
    out << keyName << "     |     ";

    vector<char> dependencies = calculateDependencies(map, keyPosition);
    for (const auto& dependecy : dependencies)
    {
      dependenciesCount[getKeyId(keyName)]++;
      graph[toupper(keyName)].push_back(toupper(dependecy));
      dependencyGraph[toupper(dependecy)].push_back(toupper(keyName));

      out << dependecy << " ";
    }

    out << endl;
  }

  //-----------------------------------------------------------------------
  // calculate distances
  
  // distances from eacth robot

  for(auto [robotId, robotPosition] : robots)
  {
    distances[robotId] = calculateDistance(map, robotPosition, 0);
  }

  // distances between each key
  for (auto [keyName, keyPosition] : inputKeys)
  {
    distances[toupper(keyName)] = (calculateDistance(map, keyPosition, 0));
  }

  //-----------------------------------------------------------------------
  // run backtracking

  vector<char> robotsIds;
  transform(begin(robots), end(robots), back_inserter(robotsIds), [](auto& robot) { return robot.id; });

  cout << backtracking(map, robotsIds, inputKeys.size());

  // printMap(out, map, keys);
  // cout << minCost;

  return 0;
}
