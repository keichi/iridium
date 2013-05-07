#ifndef __GLOBAL_VALUE_H__
#define __GLOBAL_VALUE_H__
#include <string>

//ゲーム全体で共有されるstatic変数。
//エンジンの設定・現在のゲームステータスフラグなど。

using namespace std;

enum GameStates {
    Nothing,
    ScriptWaiting,
    TextPrinting,
    ClickWaiting,
    Transitioning,
    SelectWaiting,
	BackLogging,
};

enum FadeTypes {
    FadeCross,
    FadeMove,
    FadeCurtain,
};

enum IntegratedFunc {
    f_SetWindowCaption,
    f_SetFullScreen,
    f_SetGraphMode,
    f_SetMessageNum,
    f_SetMessageStart,
    f_SetMessageWait,
    f_SetDefaultFont,
    f_LoadGlyphAnim,
    f_SetLayerMax,
    f_SetScenarioName,
    f_bgmplay,
    f_bgmstop,
    f_wait,
    f_mes,
    f_img,
    f_imgxy,
    f_copy,
    f_clear,
    f_cls,
    f_fade,
    f_assert,
    f_SetupSelection,
    f_AddSelection,
    f_GetSelection,
    f_AddFreeSelection,
    f_FocusOff,
    f_end,
    f_SpecialFade,
    func_id_count,
};

class KeyState {
    int mousex,mousey;
public:
    void Update() {
        GetMousePoint(&mousex,&mousey);
    };
    bool MouseL() {return (GetMouseInput()&MOUSE_INPUT_LEFT)!=0;}
    bool MouseR() {return (GetMouseInput()&MOUSE_INPUT_RIGHT)!=0;}
    bool KeyUp() {return CheckHitKey(KEY_INPUT_UP)==1;}
    bool KeyDown() {return CheckHitKey(KEY_INPUT_DOWN)==1;}
    int MouseX() {return mousex;}
    int MouseY() {return mousey;}
    bool KeyAccept() {
        return (CheckHitKey(KEY_INPUT_SPACE)!=0)||(CheckHitKey(KEY_INPUT_RETURN)!=0);
    }
};

class GlobalValue {
public:
    static string WindowCaption;
    static int FullScreen;
    static int WindowX,WindowY;
    static int BitDepth;
    static GameStates state;
    static int ScriptWaitCntStart;
    static int ScriptWait;
    static int MessageWidth,MessageHeight;
    static int MessageStartX,MessageStartY;
    static int MessageWait;
    static KeyState key;
    static int GlyphSizeX,GlyphSizeY,GlyphAnimSizeX,GlyphAnimSizeY;
    static int GlyphAnimNum;
    static string GlyphName;
    static int LayerMax;
    static bool MessageEnable;
    static string ScenarioName;
    static string FontName;
    static int FontSize,FontThick,FontWidth,FontHandle;
    static string FuncName[];
    static bool EndFlag;
    static FadeTypes FadeType;
};

#endif