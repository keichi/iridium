#ifndef __BACKLOG_MANAGER_H__
#define __BACKLOG_MANAGER_H__
#include <string>
#include <fstream>
#include <list>
#include "DxLib.h"

using namespace std;

class BackLogManager {
	list<string> log;
    int log_handle;
    int x_char,y_char;
    int width;
public:
	void ViewLog();
	void AddLog(string s);
	void Init();
};

#endif