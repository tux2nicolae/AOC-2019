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
#include <queue>

#include <assert.h>

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

struct Node
{
  int cost{};
  AOC::Point position;
};

vector<string> m;

map<AOC::Point, vector<Node>> graph;
unordered_map<string, vector<AOC::Point>> portals;

// inner = true
map<AOC::Point, bool> portalTypes;

bool isUpperLetter(char c)
{
  return 'A' <= c && c <= 'Z';
}

bool isPortal(AOC::Point portal)
{
  return portalTypes.find(portal) != end(portalTypes);
}

bool isInnerPortal(AOC::Point portal)
{
  return portal.x > 1 
    && portal.y > 1 
    && portal.x < (m.size() - 2) 
    && portal.y < (m[0].size() - 2);
}

string readPortal(const vector<string>& m, AOC::Point from)
{
  static const int directionX[4] = { -1,  0, 1, 0 };
  static const int directionY[4] = { 0,  1, 0, -1 };

  string portal("**");

  // up
  if (isUpperLetter(m[from.x - 1][from.y]))
  {
    portal[0] = m[from.x - 2][from.y];
    portal[1] = m[from.x - 1][from.y];
  }
  // right
  else if (isUpperLetter(m[from.x][from.y + 1]))
  {
    portal[0] = m[from.x][from.y + 1];
    portal[1] = m[from.x][from.y + 2];
  }
  // down
  else if (isUpperLetter(m[from.x + 1][from.y]))
  {
    portal[0] = m[from.x + 1][from.y];
    portal[1] = m[from.x + 2][from.y];
  }
  // left
  else if (isUpperLetter(m[from.x][from.y - 1]))
  {
    portal[0] = m[from.x][from.y - 2];
    portal[1] = m[from.x][from.y - 1];
  }

  assert(portal != "**");
  return portal;
}

int main()
{
  ifstream in("..\\..\\Day20\\src\\Day20.in");
  ofstream out("..\\..\\Day20\\src\\Day20.out");
  
  FStreamReader reader(in);
  m = reader.ReadVectorOfWords();

  AOC::Point entrance;
  AOC::Point exit;

  // read all neighbours
  for (int i = 0; i < m.size(); i++)
  {
    for (int j = 0; j < m[0].size(); j++)
    {
      if (m[i][j] != '.')
        continue;

      AOC::Point from = {i, j};

      static const int directionX[4] = { -1,  0, 1, 0 };
      static const int directionY[4] = { 0,  1, 0, -1 };

      for (int k = 0; k < 4; ++k)
      {
        AOC::Point to;
        to.x = from.x + directionX[k];
        to.y = from.y + directionY[k];

        if (m[to.x][to.y] == '.')
        {
          auto& node = graph[from];
          node.push_back(Node{1, to});
        }
        else if ('A' <= m[to.x][to.y] && m[to.x][to.y] <= 'Z')
        {
          graph[from].push_back(Node{1, to});
          graph[to].push_back(Node{0, from});

          string portal = readPortal(m, from);

          if (portal == "AA")
            entrance = to;
          else if (portal == "ZZ")
            exit = to;
          else
          {
            portals[portal].push_back(to);
            portalTypes[to] = isInnerPortal(to);
          }
        }
      }
    }
  }

  // read all portals
  for (auto& [portal, portalPositions] : portals)
  {
    assert(portalPositions.size() == 2);

    graph[portalPositions[0]].push_back(Node{0, portalPositions[1]});
    graph[portalPositions[1]].push_back(Node{0, portalPositions[0]});
  }

  struct Cost
  {
    int cost = numeric_limits<int>::max();
  };

  struct ID{
    AOC::Point value;
    int level = 0;
  };

  map<AOC::Point, unordered_map<int, Cost>> costs;
  map<AOC::Point, unordered_map<int, bool>> visited;

  // run dijkstra
  auto heapComparator = [&](ID& left, ID& right) { 
    return costs[left.value][left.level].cost > costs[right.value][right.level].cost; 
  };

  std::priority_queue<ID, vector<ID>, decltype(heapComparator)> heap(heapComparator);

  heap.push(ID{ entrance, 0 });
  costs[entrance][0] = { 0 };

  while (!heap.empty())
  {
    auto current = heap.top();
    heap.pop();

    if (current.value == exit)
    {
      cout << costs[current.value][current.level].cost - 1;
      break;
    }

    visited[current.value][current.level] = true;

    for (auto& neighbour : graph[current.value])
    {
      if (neighbour.position == exit && current.level != 0)
      {
        continue;
      }

      // adjust level
      int neightbourLevel = current.level;
      if (isPortal(current.value) && isPortal(neighbour.position))
      {
        if (isInnerPortal(neighbour.position))
          neightbourLevel--;
        else
          neightbourLevel++;
      }

      // skip outer portals on level 0
      if (isPortal(neighbour.position) && !isInnerPortal(neighbour.position) && neightbourLevel <= 0)
        continue;

      if (visited[neighbour.position][neightbourLevel])
        continue;

      // diferent levels
      if (isPortal(current.value) && isPortal(neighbour.position))
      {
        costs[neighbour.position][neightbourLevel] = Cost{ costs[current.value][current.level].cost + neighbour.cost };
      }
      // same level
      else
      {
        if (costs[neighbour.position][neightbourLevel].cost > costs[current.value][neightbourLevel].cost + neighbour.cost)
        {
          costs[neighbour.position][neightbourLevel] = Cost{ costs[current.value][neightbourLevel].cost + neighbour.cost };
        }
      }
          
      heap.push(ID{ neighbour.position, neightbourLevel });
    }
  }

  return 0;
}
