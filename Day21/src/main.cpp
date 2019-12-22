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

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

const long long kDeckSize = 119315717514047;
const long long kTestIterations = 1;

struct Operation
{
	enum class Type {
		ROTATE,
		REVERSE,
		INCREMENT,
	};

	Type type;
	long long value{};
};


long long getCutPosition(long long cut)
{
	return (kDeckSize + cut) % kDeckSize;
}

long long getReversePosition(long long from)
{
	return (kDeckSize - from % kDeckSize) - 1;
}

long long getNextIncrementPosition(long long from, long long increment)
{
	return (from * increment) % kDeckSize;
}

vector<long long> dealWithIncrement(const vector<long long>& deck, long long n)
{
	long long popTop{ 0 };
	vector<long long> newDeck(kDeckSize, 0);

	for (long long i = 0; i < deck.size(); i++)
	{
		newDeck[getNextIncrementPosition(i, n)] = deck[i];
	}

	return newDeck;
}

vector<long long> dealWithNewStack(const vector<long long>& deck)
{
	long long popTop{ 0 };
	vector<long long> newDeck(deck.size(), 0);

	for (long long i = 0; i < deck.size(); i++)
	{
		newDeck[getReversePosition(i)] = deck[i];
	}

	return newDeck;
}

void part1(const vector<string> & v)
{
	vector<long long> deck(kDeckSize, 0);
	std::iota(deck.begin(), deck.end(), 0);

	for (auto& s : v)
	{
		stringstream ss(s);

		string word;
		ss >> word;

		if (word == "cut")
		{
			long long n{};
			ss >> n;

			long long rotatePosition = getCutPosition(n);
			rotate(begin(deck), begin(deck) + rotatePosition, end(deck));
		}
		else
		{
			string nextWord;
			ss >> nextWord;
			if (nextWord == "into")
			{
				deck = dealWithNewStack(deck);
			}
			else
			{
				ss >> nextWord;

				long long n{};
				ss >> n;

				deck = dealWithIncrement(deck, n);
			}
		}
	}

	auto it = find(begin(deck), end(deck), 2019);
	if (it == end(deck))
	{
		cout << "Not found!!!" << endl;
		return;
	}

	cout << "Part1:" << distance(begin(deck), it) << endl;
}

vector<Operation> readOperations(const vector<string>& v)
{
	vector<Operation> operations;

	for (auto& s : v)
	{
		stringstream ss(s);
		string word;
		ss >> word;

		if (word == "cut")
		{
			long long n{};
			ss >> n;

			operations.push_back({ Operation::Type::ROTATE, n });
		}
		else
		{
			string nextWord;
			ss >> nextWord;
			if (nextWord == "into")
			{
				operations.push_back({ Operation::Type::REVERSE });
			}
			else
			{
				ss >> nextWord;

				long long n{};
				ss >> n;

				operations.push_back({ Operation::Type::INCREMENT, n });
			}
		}
	}

	return operations;
}

int n = 0;
unordered_map<long long, bool> exists;

long long runPosition(ostream & out, const vector<Operation>& operations, long long position, long long iterations)
{
	for (long long i = 0; i < iterations; ++i)
	{
		// run operations
		for (auto& operation : operations)
		{
			switch (operation.type)
			{
				case Operation::Type::ROTATE:
				{
					long long rotatePosition = getCutPosition(operation.value);
					position = ((position + kDeckSize) - rotatePosition) % kDeckSize;
				}
				break;
				case Operation::Type::REVERSE:
				{
					position = getReversePosition(position);
				}
				break;
				case Operation::Type::INCREMENT:
				{
					position = getNextIncrementPosition(position, operation.value);
				};
				break;
			}

		}

        out << position << endl;

		if (exists[position])
			cout << "EVRICA!!! : " << i << " : " << ++n << endl;

		exists[position] = true;
	}


	return position;
}

int main()
{
  ifstream in("..\\..\\Day21\\src\\Day21.in");
  ofstream out("..\\..\\Day21\\src\\Day21.out");

  FStreamReader reader(in);
  auto v = reader.ReadVectorOfWords();

  vector<Operation> operations = readOperations(v);

  //part1(v);

  for(int i = 0; i < 1; ++i)
  {
	long long newPosition = runPosition(out, operations, 2020, 10);
	out << endl;
	// out << newPosition << endl;
  }

  cout << 100638597289719 -   72218748140643 << endl;
  cout << 72218748140643  -   104303908009190 << endl;
  cout << 104303908009190 -   100246725387438 << endl;
  cout << 100246725387438 -   10897014506395 << endl;
  cout << 10897014506395  -   2007130324474 << endl;
  cout << 2007130324474   -   45892765961820 << endl;
  cout << 45892765961820  -   5777744825668 << endl;
  cout << 5777744825668   -   37318630879371 << endl;
  cout << 37318630879371  -   56681511722340 << endl;

  // cout << "-----------------" << endl;
  // 
  // cout << 93762728371954 - 38538312527045 << endl;
  // cout << 38538312527045 - 102629614196183 << endl;
  // cout << 102629614196183 - 47405198351274 << endl;
  // cout << 47405198351274 - 111496500020412 << endl;
  // cout << 111496500020412 - 56272084175503 << endl;
  // cout << 56272084175503 - 1047668330594 << endl;
  // cout << 1047668330594 - 65138969999732 << endl;
  // cout << 65138969999732 - 9914554154823 << endl;
  // cout << 9914554154823 - 74005855823961 << endl;
  // 
  // cout << "-----------------" << endl;
  // 
  // long long x = 93762728371954;
  // for (int i = 0; i < 10; i++)
  // {
	 //  cout << x << endl;
	 //  x = (x + 64091301669138) % kDeckSize;
  // }



//  auto primes = AOC::Eratosthenes(1000000);
//  for (int i = 0; i < 10000; i++)
//  {
//		int ok = true;
//		for (auto increment : increments)
//		{	  ok = ok && (AOC::Cmmdc(i, increment) == 1);
//		}
//  
//		if (ok && !primes[i])
//		{	  out << "OK:" << i << endl;
//		}
//  }

  // damn good
  // for (auto increment: increments)
  // {
	 //  cout << AOC::Cmmdc(17574135437386+1, increment) << " ";
  // }
  
  // vector<int> deck;
  // FStreamWriter writter(out);
  // writter.WriteVector(deck);
  // 
  // auto it = find(begin(deck), end(deck), 2019);
  // if (it == end(deck))
  // {
	 //  cout << "Not found!!!" << endl;
	 //  return 0;
  // }
  // 
  // cout << "Part1:" << distance(begin(deck), it) << endl;

  return 0;
}
