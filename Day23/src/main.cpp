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
#include <thread>
#include <mutex>
#include <assert.h>

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"


using INT = long long;

struct Packet
{
	long long x{};
	long long y{};

	bool operator<(const Packet& second) const
	{
		return tie(x, y) < tie(second.x, second.y);
	};
};

struct NetworkQueue
{
	bool IsEmpty()
	{
		scoped_lock<mutex> lock(mQueueMutex);
		return mQueue.empty();
	}

	optional<Packet> GetPacket()
	{
		scoped_lock<mutex> lock(mQueueMutex);

		if (mQueue.empty())
			return nullopt;

		auto inputValue = mQueue.front();
		mQueue.pop();

		return inputValue;
	};

	void AddPacket(Packet packet)
	{
		scoped_lock<mutex> lock(mQueueMutex);
		mQueue.push(packet);
	}

private:
	std::mutex mQueueMutex;
	std::queue<Packet> mQueue;
};

struct NAT;

class IntCodeComputer {
public:
	IntCodeComputer(const vector<INT>& memory, NAT& nat)
		: memory(memory),
		nat(nat)
	{
	}

	IntCodeComputer(IntCodeComputer&&) = default;
	IntCodeComputer(const IntCodeComputer&) = delete;

	bool booted = false;
	INT inputMode = 0;
	vector<long long> outputs;

	int iddleCheck = 0;

	bool run(INT computerID)
	{
		if (iddleCheck == 1000)
			this_thread::sleep_for(100ms);

		Packet lastPacket{};

		while (true)
		{
			INT opcode = memory[++i] % 100;
			if (opcode == 99)
			{
				return true;
			}

			assert(opcode > 0);

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
				INT value = 0;
				if (!booted)
				{
					value = computerID;
					booted = true;
				}
				else
				{
					if (inputMode == 0)
					{
						auto nextPacket = mInputQueue.GetPacket();
						if (!nextPacket)
						{
							iddleCheck = min(1000, iddleCheck + 1);
							value = -1;
						}
						else
						{
							iddleCheck = 0;

							lastPacket = *nextPacket;
							value = lastPacket.x;
							inputMode = 1;
						}
					}
					else if (inputMode == 1)
					{
						value = lastPacket.y;
						inputMode = 0;
					}
					else
					{
						assert(false);
					}
				}

				INT a = getOutputPos(aMode);
				memory[a] = value;
			}
			// output
			else if (opcode == 4)
			{
				iddleCheck = 0;

				INT a = getValue(aMode);
				outputs.push_back(a);

				if (outputs.size() == 3)
				{
					Packet packet{ outputs[1], outputs[2] };

					// send to nat
					if (outputs[0] == 255)
					{
						SendToNat(packet);
					}
					// send to other workers
					else
					{
						(*networkSpace)[outputs[0]]->sendPacket(packet);
					}

					outputs.clear();
				}
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

	void sendPacket(Packet packet)
	{
		mInputQueue.AddPacket(packet);
	}

	void setNetworkSpace(vector<unique_ptr<IntCodeComputer>>* networkSpace)
	{
		this->networkSpace = networkSpace;
	}

	bool isIddle()
	{
		return mInputQueue.IsEmpty() && iddleCheck >= 1000;
	}

	void SendToNat(Packet packet);

private:
	// address pointer
	INT i{ -1 };

	// memory
	vector<INT> memory;

	// output
	INT mOutput{ 0 };

	// relative base
	INT mRelativeBase{ 0 };

	NAT& nat;
	NetworkQueue mInputQueue;
	vector<unique_ptr<IntCodeComputer>>* networkSpace{};

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


struct NAT
{
	void setNetworkSpace(vector<unique_ptr<IntCodeComputer>>* networkSpace)
	{
		this->networkSpace = networkSpace;
	}

	void SetPacket(Packet packet)
	{
		scoped_lock<mutex> lock(mLastPacketMutex);
		mLastPacket = packet;
	}

	void run()
	{
		while (true)
		{
			bool allIddle = true;
			for (auto& computer : *networkSpace)
			{
				allIddle = allIddle && computer->isIddle();
				if (!allIddle)
					break;
			}

			if (allIddle)
			{
				scoped_lock<mutex> lock(mLastPacketMutex);

				if (send[mLastPacket])
				{
					cout << "Last packet y:" << mLastPacket.y;
					return;
				}

				cout << "Packet send y:" << mLastPacket.y << endl;

				send[mLastPacket] = true;
				(*networkSpace)[0]->sendPacket(mLastPacket);
			}

			this_thread::sleep_for(100ms);
		}
	}

private:
	std::mutex mLastPacketMutex;
	Packet mLastPacket;

	map<Packet, bool> send;
	vector<unique_ptr<IntCodeComputer>>* networkSpace;
};


void IntCodeComputer::SendToNat(Packet packet)
{
	nat.SetPacket(packet);
}

int main()
{
	ifstream in("..\\..\\Day23\\src\\Day23.in");
	ofstream out("..\\..\\Day23\\src\\Day23.out");

	FStreamReader reader(in);

	auto memory = reader.ReadVectorSeparatedByChar<INT>();
	memory.resize(memory.size() + 100000);

	NAT nat;
	vector<unique_ptr<IntCodeComputer>> nics;

	for (int i = 0; i < 50; ++i)
	{
		nics.push_back(make_unique<IntCodeComputer>(memory, nat));
	}

	// run NIC's
	vector<thread> workers;
	for (int i = 0; i < 50; ++i)
	{
		nics[i]->setNetworkSpace(&nics);
		workers.push_back(thread{ [&nics, i]() { nics[i]->run(i); } });
	}

	// run NAT
	nat.setNetworkSpace(&nics);
	thread natWorker{ [&]() { nat.run(); } };

	// wait for all
	natWorker.join();
	for (auto& worker : workers)
	{
		worker.join();
	}


	return 0;
}
