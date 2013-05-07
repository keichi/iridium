#include "select_manager.h"

void SelectManager::ClearSelection(){
    sel_list.clear();
    this->focus=true;
}
void SelectManager::AddSelection(string name){
    Selection temp;
    temp.name=name;
    temp.start_x=(GlobalValue::WindowX-GlobalValue::FontWidth*name.length())/2;
    temp.end_x=GlobalValue::WindowX-temp.start_x;
    temp.start_y=GlobalValue::WindowY/2+2*GlobalValue::FontWidth*(sel_list.size()-1);
    temp.end_y=temp.start_y+2*GlobalValue::FontWidth;
    sel_list.push_back(temp);
}

bool  SelectManager::StartSelection(){
    int x=GlobalValue::key.MouseX();
    int y=GlobalValue::key.MouseY();
    bool ret=false;
    if (focus) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
        DrawBox(0,0,GlobalValue::WindowX,GlobalValue::WindowY,GetColor(0,0,0),TRUE);
    }
    for (int i=0;i<static_cast<int>(sel_list.size());i++) {
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
        DrawStringToHandle(sel_list[i].start_x,sel_list[i].start_y,sel_list[i].name.c_str(),GetColor(255,255,255),GlobalValue::FontHandle);
        if ((sel_list[i].start_x<x)&&(sel_list[i].end_x>x)) {
            if((sel_list[i].start_y<y)&&(sel_list[i].end_y>y)) {
                if (GlobalValue::key.MouseL()) {
                    selected=i;
                    ret=true;
                }
                SetDrawBlendMode(DX_BLENDMODE_ALPHA,32);
                DrawBox(sel_list[i].start_x,sel_list[i].start_y,sel_list[i].end_x,sel_list[i].end_y,GetColor(0,255,255),TRUE);
            }
        }
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
    return ret;
}

int SelectManager::GetSelected(){
    return selected;
}

void SelectManager::AddFreeSelection(string name,int x,int y) {
    Selection temp;
    temp.name=name;
    temp.start_x=x;
    temp.end_x=temp.start_x+GlobalValue::FontWidth*name.length();
    temp.start_y=y;
    temp.end_y=temp.start_y+2*GlobalValue::FontWidth;
    sel_list.push_back(temp);
}

void SelectManager::FocusOff() {
    this->focus=false;
}