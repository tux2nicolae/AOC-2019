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
#include <stack>
#include <queue>
#include <thread>
#include <chrono>

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"


// part 1
//
// NOT A T
// NOT C J
// OR T J
// AND D J

struct AsciiIO
{
	char GetInput()
	{
		char c = {};
		cin >> noskipws >> c;

		return c;
	};

	int last = 0;
	void reportMap(int c)
	{
		last = c;
		cout << char(c);
	}

};

AsciiIO io;

using INT = long long;

class IntCodeComputer {
public:
	IntCodeComputer(const vector<INT>& memory)
		: memory(memory)
	{
	}

	bool run(const vector<INT>& input)
	{
		AOC::Point outputPos;

		while (true)
		{
			INT opcode = memory[++i] % 100;
			if (opcode == 99)
			{
				return true;
			}

			INT aMode = (memory[i] / 100) % 10;
			INT bMode = (memory[i] / 1000) % 10;
			INT cMode = (memory[i] / 10000) % 10;

			if (opcode == 1)
			{
				INT a = getValue(aMode);
				INT b = getValue(bMode);

				INT c = getOutputPos(cMode);
				memory[c] = a + b;
			}
			else if (opcode == 2)
			{
				INT a = getValue(aMode);
				INT b = getValue(bMode);

				INT c = getOutputPos(cMode);
				memory[c] = a * b;
			}

			// input
			else if (opcode == 3)
			{
				INT a = getOutputPos(aMode);
				memory[a] = io.GetInput();
			}
			// output
			else if (opcode == 4)
			{
				INT a = getValue(aMode);
				io.reportMap(a);
			}
			else if (opcode == 5)
			{
				INT a = getValue(aMode);
				INT b = getValue(bMode);

				if (a)
					i = b - 1;
			}
			else if (opcode == 6)
			{
				INT a = getValue(aMode);
				INT b = getValue(bMode);

				if (!a)
					i = b - 1;
			}
			else if (opcode == 7)
			{
				INT a = getValue(aMode);
				INT b = getValue(bMode);


				INT c = getOutputPos(cMode);
				memory[c] = a < b;
			}
			else if (opcode == 8)
			{
				INT a = getValue(aMode);
				INT b = getValue(bMode);

				INT c = getOutputPos(cMode);
				memory[c] = a == b;
			}
			else if (opcode == 9)
			{
				INT a = getValue(aMode);
				mRelativeBase += a;
			}
		}

		return true;
	}

	INT GetOutput() {
		return mOutput;
	};

private:
	// address pointer
	INT i{ -1 };

	// memory
	vector<INT> memory;

	// output
	INT mOutput{ 0 };

	// relative base
	INT mRelativeBase{ 0 };

	//---------------------------------------

	INT getValue(INT mode)
	{
		if (mode == 0)
		{
			size_t pos = memory[++i];
			return memory[pos];
		}
		else if (mode == 1)
		{
			return memory[++i];
		}
		else if (mode == 2)
		{
			return memory[memory[++i] + mRelativeBase];
		}

		return 0;
	}

	INT getOutputPos(INT mode)
	{
		if (mode == 0)
			return memory[++i];
		if (mode == 2)
			return memory[++i] + mRelativeBase;

		assert(mode != 1);
		return 0;
	}
};

int main()
{
	ifstream in("..\\..\\Day20\\src\\Day20.in");
	ofstream out("..\\..\\Day20\\src\\Day20.out");

	FStreamReader reader(in);

	auto memory = reader.ReadVectorSeparatedByChar<INT>();
	memory.resize(memory.size() + 10000000);

	// create computer
	vector<INT> input;
	IntCodeComputer mapComputer(memory);
	mapComputer.run(input);

	cout << io.last;

	// space.printMap(out);
	return 0;
}
