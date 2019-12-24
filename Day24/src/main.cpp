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

struct RecursiveData
{
	int up = 0;
	int down = 0;
	int left = 0;
	int right = 0;
};

struct Layer
{
	vector<string> map{ ".....", ".....", "..?..", ".....", "....." };
};

auto isInBoundary = [&](const AOC::Point& to)-> bool
{
	return !(to.x < 0 || to.y < 0 || to.x >= 5 || to.y >= 5);
};

auto getTopBorderSum = [](const Layer& layer)
{
	int sum = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (layer.map[0][i] == '#')
			sum++;
	}

	return sum;
};

auto getBottomBorderSum = [](const Layer& layer)
{
	int sum = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (layer.map[4][i] == '#')
			sum++;
	}

	return sum;
};

auto getLeftBorderSum = [](const Layer& layer)
{
	int sum = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (layer.map[i][0] == '#')
			sum++;
	}

	return sum;
};

auto getRightBorderSum = [](const Layer& layer)
{
	int sum = 0;
	for (int i = 0; i < 5; ++i)
	{
		if (layer.map[i][4] == '#')
			sum++;
	}

	return sum;
};

auto getCenterTop = [](const Layer& layer) -> int
{
	AOC::Point center{2, 2};
	return layer.map[center.x - 1][center.y] == '#';
};

auto getCenterBottom = [](const Layer& layer) -> int
{
	AOC::Point center{ 2, 2 };
	return layer.map[center.x + 1][center.y] == '#';
};

auto getCenterLeft = [](const Layer& layer) -> int
{
	AOC::Point center{ 2, 2 };
	return layer.map[center.x][center.y - 1] == '#';
};

auto getCenterRight = [](const Layer& layer) -> int
{
	AOC::Point center{ 2, 2 };
	return layer.map[center.x][center.y + 1] == '#';
};

int main()
{
	ifstream in("..\\..\\Day24\\src\\Day24.in");
	ofstream out("..\\..\\Day24\\src\\Day24.out");

	FStreamReader reader(in);
	auto initialMap = reader.ReadVectorOfWords();

	FStreamWriter writer(out);

	static const int directionX[4] = { -1,  0, 1, 0 };
	static const int directionY[4] = { 0,  1, 0, -1 };


	auto getScore = [&]() -> int
	{
		int n = 0;
		int score = 0;

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				score |= ((initialMap[i][j] == '#') ? 1 : 0) << n++;
			}
		}

		return score;
	};


	const int kGenerations = 200;
	const int kDepth = kGenerations / 2;
	const int kSize = (kDepth * 2) + 3;

	vector<Layer> layers(kSize);
	layers[kSize / 2] = Layer{ initialMap };

	auto runNextGeneration = [&]() -> void
	{
		vector<Layer> newLayers(kSize);
		for (int layerNr = 1; layerNr < (layers.size() - 1); ++layerNr)
		{
			Layer& currentLayer = layers[layerNr];
			Layer newLayer = currentLayer;

			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					AOC::Point from{ i,j };

					int nr = 0;
					for (int k = 0; k < 4; ++k)
					{
						AOC::Point to;
						to.x = from.x + directionX[k];
						to.y = from.y + directionY[k];

						if (!isInBoundary(to))
						{
							const auto& previousLayer = layers[layerNr - 1];

							if (directionX[k] == -1)
								nr += getCenterTop(previousLayer);
							else if (directionX[k] == 1)
								nr += getCenterBottom(previousLayer);
							else if (directionY[k] == -1)
								nr += getCenterLeft(previousLayer);
							else if (directionY[k] == 1)
								nr += getCenterRight(previousLayer);

							continue;
						}

						if (currentLayer.map[to.x][to.y] == '?')
						{
							const auto& nextLayer = layers[layerNr + 1];

							if (directionX[k] == -1)
								nr += getBottomBorderSum(nextLayer);
							else if (directionX[k] == 1)
								nr += getTopBorderSum(nextLayer);
							else if (directionY[k] == -1)
								nr += getRightBorderSum(nextLayer);
							else if (directionY[k] == 1)
								nr += getLeftBorderSum(nextLayer);

							continue;
						}

						if (currentLayer.map[to.x][to.y] == '#')
							nr++;
					}

					if (currentLayer.map[i][j] == '#' && nr != 1)
						newLayer.map[i][j] = '.';

					if (currentLayer.map[i][j] == '.' && (nr == 1 || nr == 2))
						newLayer.map[i][j] = '#';
				}
			}

			newLayers[layerNr] = newLayer;
		}

		layers = newLayers;
	};

	/*
	auto run = [&](int i, int j, int depth) -> int
	{
		AOC::Point from{ i,j };

		int nr = 0;
		for (int k = 0; k < 4; ++k)
		{
			AOC::Point to;
			to.x = from.x + directionX[k];
			to.y = from.y + directionY[k];

			if (!isInBoundary(to))
			{
				continue;
			}

			if (m[to.x][to.y] == '#')
				nr++;
		}

		if (m[i][j] == '#' && nr != 1)
			nextMap[i][j] = '.';

		if (m[i][j] == '.' && (nr == 1 || nr == 2))
			nextMap[i][j] = '#';
	};*/

	for (int generation = 0; generation < kGenerations; generation++)
	{
		// run generation
		runNextGeneration();

		// calculate sum
		int sum = 0;
		for (auto& layer : layers)
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					sum += layer.map[i][j] == '#';
				}
			}
		}

		cout << "Generation "<< generation << " sum:" << sum << endl;
	}

	for (const auto& layer : layers)
	{
		for (const auto line : layer.map)
		{
			out << line << endl;
		}

		out << endl;
	}

	return 0;
}
