#include "graphic_manager.h"

LayerObject::LayerObject() {
    enable=false;
    x=0;
    y=0;
}

void GraphicManager::DrawLayer() {
    if (GlobalValue::state==Transitioning) {
        switch (GlobalValue::FadeType) {
            case FadeCross:
                CrossFade(fade_frame_left,fade_frame);
                break;
            case FadeMove:
                MoveFade(fade_frame_left,fade_frame);
                break;
            case FadeCurtain:
                CurtainFade(fade_frame_left,fade_frame);
                break;
        }
        fade_frame_left--;

        if (this->fade_frame_left==0) {
            for (int i=0;i<GlobalValue::LayerMax;i++) {
                if ((layer[i].enable)&&(layer[i].handle!=layer_back[i].handle)) {
                    DeleteGraph(layer[i].handle);
                }
                layer[i]=layer_back[i];
                layer_back[i].enable=false;
            }
            GlobalValue::state=Nothing;
        }
    }else{
        for (int i=0;i<GlobalValue::LayerMax;i++) {
            if (layer[i].enable) {
                DrawGraph(layer[i].x,layer[i].y,layer[i].handle,TRUE);
            }
        }
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void GraphicManager::LoadBackLayer(int i,int x,int y,string s) {
    int handle_temp;
    if (i>=GlobalValue::LayerMax) {
        printfDx("描画レイヤの指定が規定範囲外です。\n");
        return;
    }
    //int c=GetNowCount();
    handle_temp=LoadGraph(s.c_str());
    //printfDx("LoadGraph所要時間:%d\n",GetNowCount()-c);
    if (handle_temp==-1) {
        printfDx("ロードしようとした画像%sが見つかりません。\n",s.c_str());
    }
    layer_back[i].name=s;
    layer_back[i].x=x;
    layer_back[i].y=y;
    layer_back[i].handle=handle_temp;
    layer_back[i].enable=true;
}

void GraphicManager::Init() {
    layer.resize(GlobalValue::LayerMax);
    layer_back.resize(GlobalValue::LayerMax);
}

void GraphicManager::CopyForeToBack() {
    for (int i=0;i<GlobalValue::LayerMax;i++) {
        if ((layer_back[i].enable)&&(layer[i].handle!=layer_back[i].handle)) {
            DeleteGraph(layer_back[i].handle);
        }
        layer_back[i]=layer[i];
    }
}

void GraphicManager::SetFade(int frame) {
    if (frame<=0) {
        frame=1;
    }
    fade_frame=frame;
    fade_frame_left=frame;
    GlobalValue::state=Transitioning;
}

void GraphicManager::ClearBack(int i) {
    layer_back[i].enable=false;
}

void GraphicManager::ClearAllBack() {
    for (int i=0;i<GlobalValue::LayerMax;i++) {
        layer_back[i].enable=false;
    }
}

void GraphicManager::CrossFade(int fade_left, int fade_frame) {
    SetDrawBlendMode(DX_BLENDMODE_ALPHA,255*(fade_frame-fade_frame_left)/fade_frame);
    for (int i=0;i<GlobalValue::LayerMax;i++) {
        if (layer_back[i].enable) {
            DrawGraph(layer_back[i].x,layer_back[i].y,layer_back[i].handle,TRUE);
        }
    }
    SetDrawBlendMode(DX_BLENDMODE_ALPHA,255*fade_frame_left/fade_frame);
    for (int i=0;i<GlobalValue::LayerMax;i++) {
        if (layer[i].enable) {
            DrawGraph(layer[i].x,layer[i].y,layer[i].handle,TRUE);
        }
    }
}

void GraphicManager::MoveFade(int fade_left, int fade_frame) {
    for (int i=0;i<GlobalValue::LayerMax;i++) {
        if (layer[i].enable) {
            DrawGraph(GlobalValue::WindowX*(fade_left-fade_frame)/fade_frame+layer[i].x,layer[i].y,layer[i].handle,TRUE);
        }
    }
    for (int i=0;i<GlobalValue::LayerMax;i++) {
        if (layer_back[i].enable) {
            DrawGraph(GlobalValue::WindowX*fade_left/fade_frame+layer_back[i].x,layer_back[i].y,layer_back[i].handle,TRUE);
        }
    }
}

void GraphicManager::CurtainFade(int fade_left, int fade_frame) {
/*    DrawGraph(0,0,back_buffer_handle,TRUE);
	for( int i= 0;i<GlobalValue::WindowX/16;i++) {
		SetDrawArea(i*32,0,i*32+32*fade_left/fade_frame,GlobalValue::WindowY);
        DrawGraph(0,0,fore_buffer_handle,TRUE);
	}
    SetDrawArea(0,0,GlobalValue::WindowX,GlobalValue::WindowY);*/
}