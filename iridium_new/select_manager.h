#ifndef __SELECT_MANAGER_H__
#define __SELECT_MANAGER_H__
#include "DxLib.h"
#include <vector>
#include "global_value.h"

using namespace std;

class Selection {
public:
    int start_x,start_y,end_x,end_y;
    bool focus;
    string name;
};

class SelectManager {
    int selected;
    vector<Selection> sel_list;
public:
    bool focus;
    void ClearSelection();
    void AddSelection(string name);
    void AddFreeSelection(string name,int x,int y);
    bool  StartSelection();
    int GetSelected();
    void FocusOff();
};

#endif