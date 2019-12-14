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
#include <regex>
#include <queue>
#include <stack>

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

struct NodeData
{
  int fromQuantity;
  vector<pair<string, int>> toQuantity;
};

vector<string> chemicals;
unordered_map<string, NodeData> graph;
unordered_map<string, int> depth;

int calculateDeeps(const string & nodeKey)
{
  if (nodeKey == "ORE")
    return 0;

  int maxDepth = 0;
  for (const auto & [next, _] : graph[nodeKey].toQuantity)
  {
    maxDepth = max(calculateDeeps(next) + 1, maxDepth);
  }

  depth[nodeKey] = maxDepth;
  return maxDepth;
}

int main()
{
  ifstream in("..\\..\\Day14\\src\\Day14.in");
  ofstream out("..\\..\\Day14\\src\\Day14.out");

  FStreamReader reader(in);
  auto lines = reader.ReadVectorOfWords();

  // read data
  for (auto& line : lines)
  {
    auto tokens = AOC::Explode(line, '=');
    
    // result
    int resultQuantity;
    string resultChemical;

    string resultTokens(tokens[1].erase(0, 1));
    stringstream resultChemicalStream(resultTokens);
    resultChemicalStream >> resultQuantity >> resultChemical;

    NodeData nodeData;
    nodeData.fromQuantity = resultQuantity;

    // source
    vector<string> sourceChemicals = AOC::Explode(tokens[0],',');
    for (const auto& sourceChemical : sourceChemicals)
    {
      int quantity;
      string chemical;

      stringstream sourceChemicalStream(sourceChemical);
      sourceChemicalStream >> quantity >> chemical;

      // do it
      nodeData.toQuantity.emplace_back(chemical, quantity);
    }

    graph[resultChemical] = nodeData;
    chemicals.push_back(resultChemical);
  }

  // precalculate deeps
  calculateDeeps("FUEL");

  auto comparator = [](const string& left, const string& right) {
    return depth[left] > depth[right];
  };

  std::sort(begin(chemicals), end(chemicals), comparator);

  //-------------------------------------------------------------------------

  auto getOreForFuel = [&](long long fuel) -> long long
  {
    // reaction
    unordered_map<string, long long> reaction;
    reaction["FUEL"] = fuel;

    auto traverse = [&reaction](const string& startKey)
    {
      stack<string> st;
      st.push(startKey);

      while (!st.empty())
      {
        auto currentKey = st.top();
        st.pop();

        const auto& currentNode = graph[currentKey];

        // calculate reaction
        long long some = (reaction[currentKey] / currentNode.fromQuantity);
        reaction[currentKey] = reaction[currentKey] % currentNode.fromQuantity;

        if (some == 0)
          continue;

        for (const auto& next : currentNode.toQuantity)
        {
          // do reaction
          reaction[next.first] += some * next.second;

          if (next.first != "ORE")
            st.push(next.first);
        }
      }
    };

    for (auto& chemicalKey : chemicals)
    {
      // add value until a full reaction
      const auto& currentNode = graph[chemicalKey];
      if (reaction[chemicalKey] > 0 && (reaction[chemicalKey] < currentNode.fromQuantity))
        reaction[chemicalKey] = currentNode.fromQuantity;

      // dfs traverse
      traverse(chemicalKey);
    }

    return reaction["ORE"];
  };

  std::cout << "Part1:" << getOreForFuel(1) << endl;

  long long a = 1;
  long long b = 10000000000;

  // binary search for the value
  while (a <= b)
  {
    long long mid = a + (b - a) / 2;
    long long midValue = getOreForFuel(mid);

    if (midValue == 1000000000000)
    {
      cout << "Part2(!):" << mid;
      break;
    }

    if (midValue < 1000000000000)
      a = mid + 1;
    else
      b = mid - 1;
  }

  cout << "Part2:" << a - 1;

  return 0;
}
