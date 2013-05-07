#ifndef __GRAPHIC_MANAGER_H__
#define __GRAPHIC_MANAGER_H__
#include <string>
#include <vector>
#include <map>
#include "DxLib.h"
#include "global_value.h"

using namespace std;

class LayerObject {
public:
    bool enable;
    string name;
    int handle;
    int x,y;
    LayerObject();
};

class GraphicManager {
    vector<LayerObject> layer;
    vector<LayerObject> layer_back;
    int fade_frame;
    int fade_frame_left;
    int LoadGraphicHandle(string name);
    void DeleteGraphicHandle(int handle);
    void CrossFade(int fade_left,int fade_frame);
    void MoveFade(int fade_left,int fade_frame);
    void CurtainFade(int fade_left,int fade_frame);
public:
    void DrawLayer();
    void LoadBackLayer(int i,int x,int y,string s);
    void Init();
    void SetFade(int frame);
    void CopyForeToBack();
    void ClearBack(int i);
    void ClearAllBack();
};

#endif