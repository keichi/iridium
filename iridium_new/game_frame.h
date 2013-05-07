#ifndef __GAME_FRAME_H__
#define __GAME_FRAME_H__
#include "backlog_manager.h"
#include "script_manager.h"
#include "text_manager.h"
#include "sound_manager.h"
#include <boost/any.hpp>
#include <stack>
#include "DxLib.h"
#include "global_value.h"
#include "graphic_manager.h"
#include "select_manager.h"
#include "resource.h"

class GameFrame {
    int static MenuCmdHandler(WORD cmd);
    stack<any> stk;
    SoundManager* sm;
    TextManager* tm;
    GraphicManager* gm;
    SelectManager* selm;
    ScriptManager* scenario;
	BackLogManager* log;
    void ExecuteAssembly(Assembly cmd);
    void ExecuteFunction(IntegratedFunc func);
    void RegulateFrameRate(int time);
    int last_time;
    bool IsString(any i) {
        return (i.type()==typeid(string));
    }
    bool IsInt(any i) {
        return (i.type()==typeid(int));
    }
    int StackIntPop() {
        any i=stk.top();
        int ret;
        if (!IsInt(i)) {
            printfDx("型の不一致エラーです。");
            ret=0;
        }else{
            ret=any_cast<int>(stk.top());
        }
        stk.pop();
        return ret;
    }
    string StackStringPop() {
        any i=stk.top();
        string ret;
        if (!IsString(i)) {
            printfDx("型の不一致エラーです。");
            ret="";
        }else{
            ret=any_cast<string>(stk.top());
        }
        stk.pop();
        return ret;
    }
public:
    void OneFrame();
    bool Initialize();
    GameFrame() {
        sm=new SoundManager();
        tm=new TextManager();
        gm=new GraphicManager();
        selm=new SelectManager();
        scenario=new ScriptManager();
		log=new BackLogManager();
    }
    ~GameFrame() {
        delete sm;
        delete tm;
        delete gm;
        delete selm;
        delete scenario;
		delete log;
    }
};

#endif