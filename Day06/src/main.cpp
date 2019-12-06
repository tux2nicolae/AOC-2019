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

unordered_map<string, vector<string>> graph;
unordered_map<string, int> dependencies;

int s = 0;
int x = 0;

unordered_map<string, bool> visited;

void traverse(const string & node, int depth)
{
  for (auto& next : graph[node])
  {
    if (visited[next])
      continue;

    // part 2
    if (next == "SAN")
    {
      cout << depth - 2;
      return;
    }

    s += depth;

    visited[next] = true;
    traverse(next, depth + 1);
  }
}

int main()
{
  ifstream in("..\\..\\Day06\\src\\Day06.in");
  ofstream out("..\\..\\Day06\\src\\Day06.out");

  FStreamReader reader(in);
  vector<string> v = reader.ReadVectorOfWords();

  for(auto & str : v)
  {
    auto edge = AOC::Explode(str, ')');
    graph[edge[0]].push_back(edge[1]);
    
    // part 2
    graph[edge[1]].push_back(edge[0]);

    dependencies[edge[1]]++;
  }

  // part 2
  for (auto& edge : graph)
  {
    if (edge.first == "YOU")
    {
        traverse(edge.first, 1);
    }
  }

  return 0;
}
