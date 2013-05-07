#include "backlog_manager.h"
#include "global_value.h"

void BackLogManager::AddLog(string s) {
    //将来的にTextManagerの整形結果と同期をとる(今はタグを削除)
    if (log.size()==y_char) log.pop_front();
    for (int i=0;i<s.length();i+=x_char) {
        log.push_back(s.substr(i,x_char));
    }
}

void BackLogManager::ViewLog() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,200);
	DrawBox(0,0,GlobalValue::WindowX,GlobalValue::WindowY,GetColor(0,0,0),1) ;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
    int i=0;
    for (list<string>::iterator it=log.begin();it!=log.end();it++) {
        DrawStringToHandle(0,width*i,it->c_str(),GetColor(255,255,255),log_handle);
        i++;
    }
}

void BackLogManager::Init() {
    log_handle=CreateFontToHandle(GlobalValue::FontName.c_str(),GlobalValue::FontSize*2/3,GlobalValue::FontThick,DX_FONTTYPE_ANTIALIASING);
    width=GetDrawStringWidthToHandle("あ",2,log_handle);
    x_char=GlobalValue::WindowX/width*2;
    y_char=GlobalValue::WindowY/width;
}