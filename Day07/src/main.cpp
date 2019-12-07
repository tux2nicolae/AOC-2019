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

class Amplyfier {
public:
  Amplyfier(const vector<int> memory)
    : v(memory)
  {
  }

  bool run(const vector<int>& input)
  {
    int inputN = 0;
    while (true)
    {
      int opcode = v[++i] % 100;
      if (opcode == 99)
      {
        return true;
      }

      int aMode = (v[i] / 100) % 10;
      int bMode = (v[i] / 1000) % 10;
      int cMode = (v[i] / 10000) % 10;

      if (opcode == 1)
      {
        int a = aMode == 1 ? v[++i] : v[v[++i]];
        int b = bMode == 1 ? v[++i] : v[v[++i]];
        int c = v[++i];

        v[c] = a + b;
      }
      else if (opcode == 2)
      {
        int a = aMode == 1 ? v[++i] : v[v[++i]];
        int b = bMode == 1 ? v[++i] : v[v[++i]];
        int c = v[++i];

        v[c] = a * b;
      }

      //
      else if (opcode == 3)
      {
        int a = v[++i];

        int value = input[inputN++];
        v[a] = value;
      }
      else if (opcode == 4)
      {
        int a = aMode == 1 ? v[++i] : v[v[++i]];

        mOutput = a;
        return false;
      }

      //
      else if (opcode == 5)
      {
        int a = aMode == 1 ? v[++i] : v[v[++i]];
        int b = bMode == 1 ? v[++i] : v[v[++i]];

        if (a)
          i = b - 1;
      }
      else if (opcode == 6)
      {
        int a = aMode == 1 ? v[++i] : v[v[++i]];
        int b = bMode == 1 ? v[++i] : v[v[++i]];

        if (!a)
          i = b - 1;
      }
      else if (opcode == 7)
      {
        int a = aMode == 1 ? v[++i] : v[v[++i]];
        int b = bMode == 1 ? v[++i] : v[v[++i]];
        int c = v[++i];

        v[c] = a < b;
      }
      else if (opcode == 8)
      {
        int a = aMode == 1 ? v[++i] : v[v[++i]];
        int b = bMode == 1 ? v[++i] : v[v[++i]];
        int c = v[++i];

        v[c] = a == b;
      }
    }

    return true;
  }

  int GetOutput() {
    return mOutput;
  };

private:
  // address pointer
  int i{-1};

  // memory
  vector<int> v;

  // output
  int mOutput{0};

};

int runPhases(const vector<int> & memory, const vector<int>& phases)
{
  vector<Amplyfier> amplifiers(5, memory);

  bool firstInput = true;
  int output = 0;
  
  while(true)
  {
    for (int i = 0; i < 5; ++i)
    {
      bool finished = amplifiers[i]
        .run(firstInput ? vector<int>{phases[i], output} : vector<int>{ output });

      output = amplifiers[i].GetOutput();

      if (i==4 && finished) {
        return output;
      }
    }

    firstInput = false;
  }

  return - 1;
}

vector<int> phases(5,0);
unordered_map<int, bool> used;

int maxSignal = 0;

void backTrack(const vector<int> & memory, int depth)
{
  if (depth == 5)
  {
    maxSignal = std::max(maxSignal, runPhases(memory, phases));
    return;
  }

  for (int i = 5; i <= 9; ++i)
  {
    if(!used[i])
    {
      phases[depth] = i;

      used[i] = true;
      backTrack(memory, depth + 1);
      used[i] = false;
    }
  }
}

int main()
{
  ifstream in("..\\..\\Day07\\src\\Day07.in");
  ofstream out("..\\..\\Day07\\src\\Day07.out");

  vector<int> memory;
  while (in.good())
  {
    int x;
    in >> x;
    memory.push_back(x);

    char _;
    in >> _;
  }

  // vector<int> testPhases{ 9,8,7,6,5 };
  // auto ret = runPhases(memory, testPhases);
  // 
  // cout << endl;
  // cout << ret << endl;

  backTrack(memory, 0);
  cout << endl << maxSignal;

  return 0;
}
