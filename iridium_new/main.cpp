#include "DxLib.h"
#include "game_frame.h"

string GlobalValue::WindowCaption="Engine";
int GlobalValue::FullScreen=1;
int GlobalValue::WindowX=640;
int GlobalValue::WindowY=480;
int GlobalValue::BitDepth=16;
GameStates GlobalValue::state=Nothing;
int GlobalValue::ScriptWaitCntStart=0;
int GlobalValue::ScriptWait=0;
int GlobalValue::MessageWidth=50;
int GlobalValue::MessageHeight=4;
int GlobalValue::MessageStartX=0;
int GlobalValue::MessageStartY=0;
int GlobalValue::MessageWait=10;
KeyState GlobalValue::key;
int GlobalValue::GlyphSizeX=24;;
int GlobalValue::GlyphSizeY=24;
int GlobalValue::GlyphAnimSizeX=384;
int GlobalValue::GlyphAnimSizeY=24;
int GlobalValue::GlyphAnimNum=16;
string GlobalValue::GlyphName="glyph.png";
int GlobalValue::LayerMax=5;
bool GlobalValue::MessageEnable=true;
string GlobalValue::ScenarioName="scenario.txt";
string GlobalValue::FontName="HG創英角ﾎﾟｯﾌﾟ体";
int GlobalValue::FontSize=25;
int GlobalValue::FontThick=5;
int GlobalValue::FontWidth=0;
int GlobalValue::FontHandle=0;
bool GlobalValue::EndFlag=false;
FadeTypes GlobalValue::FadeType;
string GlobalValue::FuncName[]={
    "SetWindowCaption",
    "SetFullScreen",
    "SetGraphMode",
    "SetMessageNum",
    "SetMessageStart",
    "SetMessageWait",
    "SetDefaultFont",
    "LoadGlyphAnim",
    "SetLayerMax",
    "SetScenarioName",
    "bgmplay",
    "bgmstop",
    "wait",
    "mes",
    "img",
    "imgxy",
    "copy",
    "clear",
    "cls",
    "fade",
    "assert",
    "SetupSelection",
    "AddSelection",
    "GetSelection",
    "AddFreeSelection",
    "FocusOff",
    "end",
    "specialfade",
};

vector<Assembly> ProgramExtractor::ProgramList;
map<string,int> ProgramExtractor::LabelMap;
string ProgramExtractor::temp_s;
string ProgramExtractor::temp_ident;
int ProgramExtractor::block=0;
int ProgramExtractor::nest=0;
int ProgramExtractor::blockno[32];
map<string,any> ProgramExtractor::VarMap;
stack<string> ProgramExtractor::IdentStack;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
    GameFrame* gf=new GameFrame();
    if (!gf->Initialize()) {
        AppLogAdd("メインモジュールのイニシャライズに失敗");
        return false;
    }

    while (!ProcessMessage()&&!GlobalValue::EndFlag) {
        ClsDrawScreen();
        gf->OneFrame();
        ScreenFlip();
    }
    
    delete gf;
    
    AppLogAdd("メインモジュール正常終了");
	DxLib_End();
	return 0 ;
}