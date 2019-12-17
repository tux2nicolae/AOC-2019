#pragma once

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

using namespace std;

#include "../../AOCLib/src/Algorithm.h"
#include "../../AOCLib/src/FStreamReader.h"
#include "../../AOCLib/src/FStreamWriter.h"
#include "../../AOCLib/src/Math.h"
#include "../../AOCLib/src/Time.h"

struct Robot
{
  char direction{ '^' };
  AOC::Point position{ 0, 0 };

  void setPosition(AOC::Point position)
  {
    this->position = position;
  }

  AOC::Point getPosition()
  {
    return position;
  }

  void turnLeft()
  {
    switch (direction)
    {
    case '^':
      direction = '<';
      break;
    case '<':
      direction = 'v';
      break;
    case 'v':
      direction = '>';
      break;
    case '>':
      direction = '^';
      break;
    default:
      break;
    }
  }

  void turnRight()
  {
    switch (direction)
    {
    case '^':
      direction = '>';
      break;
    case '>':
      direction = 'v';
      break;
    case 'v':
      direction = '<';
      break;
    case '<':
      direction = '^';
      break;
    default:
      break;
    }
  }

  void moveForward()
  {
    // move forward
    if (direction == '^')
    {
      position.x -= 1;
    }
    else if (direction == '>')
    {
      position.y += 1;
    }
    else if (direction == 'v')
    {
      position.x += 1;
    }
    else if (direction == '<')
    {
      position.y -= 1;
    }
  }

  AOC::Point GetFront()
  {
    AOC::Point front = position;

    // move forward
    if (direction == '^')
    {
      front.x -= 1;
    }
    else if (direction == '>')
    {
      front.y += 1;
    }
    else if (direction == 'v')
    {
      front.x += 1;
    }
    else if (direction == '<')
    {
      front.y -= 1;
    }

    return front;
  }

  AOC::Point GetRight()
  {
    AOC::Point left = position;

    switch (direction)
    {
    case '^':
      left.y++;
      break;
    case '>':
      left.x++;
      break;
    case 'v':
      left.y--;
      break;
    case '<':
      left.x--;
      break;
    default:
      break;
    }

    return left;
  }

  AOC::Point GetLeft()
  {
    AOC::Point right = position;

    switch (direction)
    {
    case '^':
      right.y--;
      break;
    case '>':
      right.x--;
      break;
    case 'v':
      right.y++;
      break;
    case '<':
      right.x++;
      break;
    default:
      break;
    }

    return right;
  }
};
