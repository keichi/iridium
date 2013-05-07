#include "sound_manager.h"

bool SoundManager::PlayBgm(string name) {
    //この関数を実行してから再生されるまではタイムラグが・・・
    int prev_handle=bgm_handle;
    bgm_handle=LoadSoundMem(name.c_str());
    if (bgm_handle==-1) {
        return false;
    }
    if (CheckSoundMem(prev_handle)==1) {
        DeleteSoundMem(prev_handle);
    }
    PlaySoundMem(bgm_handle,DX_PLAYTYPE_BACK);
    this->bgm_name=name;
    return true;
}

void SoundManager::StopBgm() {
    if (CheckSoundMem(bgm_handle)==1) {
        DeleteSoundMem(bgm_handle);
    }
    bgm_handle=-1;
}