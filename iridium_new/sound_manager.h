#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__
#include "DxLib.h"
#include <list>
#include <string>

using namespace std;

class SoundManager{
    int bgm_handle;
    //これが空ならBGMは再生中でない→セーブデータ作るときに活用
    string bgm_name;
public:
    bool PlayBgm(string name);
    void StopBgm();
};

#endif