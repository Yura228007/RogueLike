#pragma once
#include "SuperObject.h"
#include <list>
using namespace std;

const int HIGH = 10;
const int WIDTH = 10;
//char emptyChar = '.';

int fps = 4;
int latency = 1000 / fps;

Point display[HIGH][WIDTH]{};
list<SuperObject*> objects;

char keyboardPress;
bool main_flag = true;