#include "game_frame.h"

void GameFrame::ExecuteFunction(IntegratedFunc func) {
    int temp_int;
    switch(func) {
        case f_SetWindowCaption:
            GlobalValue::WindowCaption=StackStringPop();
            break;
        case f_SetFullScreen:
            GlobalValue::FullScreen=StackIntPop();
            break;
        case f_SetGraphMode:
            GlobalValue::BitDepth=StackIntPop();
            GlobalValue::WindowY=StackIntPop();
            GlobalValue::WindowX=StackIntPop();
            break;
        case f_SetMessageNum:
            GlobalValue::MessageHeight=StackIntPop();
            GlobalValue::MessageWidth=StackIntPop()*2;
            break;
        case f_SetMessageStart:
            GlobalValue::MessageStartY=StackIntPop();
            GlobalValue::MessageStartX=StackIntPop();
            break;
        case f_SetMessageWait:
            GlobalValue::MessageWait=StackIntPop();
            break;
        case f_SetDefaultFont:
            GlobalValue::FontThick=StackIntPop();
            GlobalValue::FontSize=StackIntPop();
            GlobalValue::FontName=StackStringPop();
            break;
        case f_LoadGlyphAnim:
            GlobalValue::GlyphSizeY=StackIntPop();
            GlobalValue::GlyphSizeX=StackIntPop();
            GlobalValue::GlyphAnimSizeY=StackIntPop();
            GlobalValue::GlyphAnimSizeX=StackIntPop();
            GlobalValue::GlyphName=StackStringPop();
            GlobalValue::GlyphAnimNum=(GlobalValue::GlyphAnimSizeX*GlobalValue::GlyphAnimSizeY)/(GlobalValue::GlyphSizeX*GlobalValue::GlyphSizeY);
            break;
        case f_SetLayerMax:
            GlobalValue::LayerMax=StackIntPop();
            break;
        case f_SetScenarioName:
            GlobalValue::ScenarioName=StackStringPop();
            break;
        case f_bgmplay:
            if(!sm->PlayBgm(StackStringPop())) {
                printfDx("サウンドロードエラー");
            };
            break;
        case f_bgmstop:
            this->sm->StopBgm();
            break;
        case f_wait:
            GlobalValue::ScriptWaitCntStart=GetNowCount();
            GlobalValue::ScriptWait=StackIntPop();
            GlobalValue::state=ScriptWaiting;
            break;
        case f_mes:
			{
			string s=StackStringPop();
            tm->SetMessage(s);
			log->AddLog(s);
            GlobalValue::state=TextPrinting;
			}
            break;
        case f_img:
            temp_int=StackIntPop();
            gm->LoadBackLayer(temp_int,0,0,StackStringPop().c_str());
            break;
        case f_imgxy:
            int x,y;
            y=StackIntPop();
            x=StackIntPop();
            temp_int=StackIntPop();
            gm->LoadBackLayer(temp_int,x,y,StackStringPop().c_str());
            break;
        case f_copy:
            gm->CopyForeToBack();
            break;
        case f_clear:
            gm->ClearBack(StackIntPop());
            break;
        case f_cls:
            gm->ClearAllBack();
            break;
        case f_assert:
            if (IsInt(stk.top())) {
                printfDx("Script Assert:%d\n",StackIntPop());
            }else if (IsString(stk.top())) {
                printfDx("Script Assert:%s\n",StackStringPop().c_str());
            }
            break;
        case f_SetupSelection:
            selm->ClearSelection();
            break;
        case f_AddSelection:
            selm->AddSelection(StackStringPop());
            break;
        case f_AddFreeSelection:
            y=StackIntPop();
            x=StackIntPop();
            selm->AddFreeSelection(StackStringPop(),x,y);
            break;
        case f_GetSelection:
            GlobalValue::state=SelectWaiting;
            break;
        case f_FocusOff:
            selm->FocusOff();
            break;
        case f_end:
            GlobalValue::EndFlag=true;
            break;
        case f_fade:
            gm->SetFade(StackIntPop());
            GlobalValue::FadeType=FadeCross;
            break;
        case f_SpecialFade:
            GlobalValue::FadeType=static_cast<FadeTypes>(StackIntPop());
            gm->SetFade(StackIntPop());
            break;
        default:
            printfDx("スクリプトにおいて不明な関数が実行されました。\n");
            break;
    }
}

void GameFrame::ExecuteAssembly(Assembly cmd) {
    any a,b;
    switch (cmd.cmd) {
        case label:
            break;
        case invoke:
            ExecuteFunction(any_cast<IntegratedFunc>(cmd.parameter[0]));
            break;
        case pushi:
            stk.push(cmd.parameter[0]);
            break;
        case pushs:
            stk.push(cmd.parameter[0]);
            break;
        case jump:
            scenario->JumpToLabel(any_cast<string>(cmd.parameter[0]));
            break;
        case addi:
            a=stk.top();
            stk.pop();
            b=stk.top();
            stk.pop();
            if (IsInt(a)&&IsInt(b)) {
                stk.push(any_cast<int>(a)+any_cast<int>(b));
            }else if (IsString(a)&&IsString(b)) {
                stk.push(any_cast<string>(b)+any_cast<string>(a));
            }else if (IsInt(a)&&IsString(b)) {
                stk.push(any_cast<string>(b)+lexical_cast<string>(any_cast<int>(a)));
            }else{
                printfDx("演算子が型に対応していません。");
            }
            break;
        case subi:
            a=stk.top();
            stk.pop();
            b=stk.top();
            stk.pop();
            if (IsInt(a)&&IsInt(b)) {
                stk.push(any_cast<int>(b)-any_cast<int>(a));
            }else{
                printfDx("演算子が型に対応していません。");
            }
            break;
        case muli:
            a=stk.top();
            stk.pop();
            b=stk.top();
            stk.pop();
            if (IsInt(a)&&IsInt(b)) {
                stk.push(any_cast<int>(a)*any_cast<int>(b));
            }else{
                printfDx("演算子が型に対応していません。");
            }
            break;
        case divi:
            a=stk.top();
            stk.pop();
            b=stk.top();
            stk.pop();
            if (IsInt(a)&&IsInt(b)) {
                if (any_cast<int>(a)==0) {
                    printfDx("0で除算しました。");
                    break;
                }
                stk.push(any_cast<int>(b)/any_cast<int>(a));
            }else{
                printfDx("演算子が型に対応していません。");
            }
            break;
        case eqi:
            a=stk.top();
            stk.pop();
            b=stk.top();
            stk.pop();
		    if (IsInt(a)&&IsInt(b)) {
				if (any_cast<int>(a)==any_cast<int>(b)) {
				    stk.push(1);
				}else{
				    stk.push(0);
				}
			}else{
			    printfDx("演算子が型に対応していません。");
			}
            break;
        case neqi:
            a=stk.top();
            stk.pop();
            b=stk.top();
            stk.pop();
		    if (IsInt(a)&&IsInt(b)) {
				if (any_cast<int>(a)!=any_cast<int>(b)) {
				    stk.push(1);
				}else{
				    stk.push(0);
				}
			}else{
			    printfDx("演算子が型に対応していません。");
			}
            break;
        case gtei:
            a=stk.top();
            stk.pop();
            b=stk.top();
            stk.pop();
		    if (IsInt(a)&&IsInt(b)) {
				if (any_cast<int>(b)>=any_cast<int>(a)) {
				    stk.push(1);
				}else{
				    stk.push(0);
				}
			}else{
			    printfDx("演算子が型に対応していません。");
			}
            break;
        case stei:
            a=stk.top();
            stk.pop();
            b=stk.top();
            stk.pop();
		    if (IsInt(a)&&IsInt(b)) {
				if (any_cast<int>(b)<=any_cast<int>(a)) {
				    stk.push(1);
				}else{
				    stk.push(0);
				}
			}else{
			    printfDx("演算子が型に対応していません。");
			}
            break;
        case gti:
            a=stk.top();
            stk.pop();
            b=stk.top();
            stk.pop();
		    if (IsInt(a)&&IsInt(b)) {
				if (any_cast<int>(b)>any_cast<int>(a)) {
				    stk.push(1);
				}else{
				    stk.push(0);
				}
			}else{
			    printfDx("演算子が型に対応していません。");
			}
            break;
        case sti:
            a=stk.top();
            stk.pop();
            b=stk.top();
            stk.pop();
		    if (IsInt(a)&&IsInt(b)) {
				if (any_cast<int>(b)<any_cast<int>(a)) {
				    stk.push(1);
				}else{
				    stk.push(0);
				}
			}else{
			    printfDx("演算子が型に対応していません。");
			}
           break;
        case jumpf:
            if (any_cast<int>(stk.top())>0) {
                scenario->JumpToLabel(any_cast<string>(cmd.parameter[0]));
            }
            stk.pop();
            break;
        case loadi:
            stk.push(scenario->LoadVariable(any_cast<string>(cmd.parameter[0])));
            break;
        case storei:
            scenario->StoreVariable(any_cast<string>(cmd.parameter[0]),stk.top());
            stk.pop();
            break;
    }
}

bool GameFrame::Initialize() {
    Assembly temp;
    AppLogAdd("init,txtのパースを開始\n");
    if(!scenario->ExtractScript("init.txt")) {
        AppLogAdd("init,txtのパースに失敗\n");
        return false;
    }
    temp=scenario->ReturnNextCode();
    while(temp.cmd!=eof) {
        this->ExecuteAssembly(temp);
        temp=scenario->ReturnNextCode();
    }
    if (GlobalValue::FullScreen==0) {
        SetUseGDIFlag(TRUE);
        if (ChangeWindowMode(true)!=DX_CHANGESCREEN_OK) {
            return false;
        }
    }
    SetMainWindowText(GlobalValue::WindowCaption.c_str()) ;
    SetGraphMode(GlobalValue::WindowX,GlobalValue::WindowY,GlobalValue::BitDepth);
    AppLogAdd("init,txtを読み込み完了\n");
    AppLogAdd("DXライブラリ初期化開始\n");
    if (DxLib_Init()==-1) {
        AppLogAdd("DXライブラリの初期化に失敗\n");
        return false;
    }
    AppLogAdd("DXライブラリの初期化成功\n");
    SetDrawScreen(DX_SCREEN_BACK);
    AppLogAdd("TextManagerの初期化開始\n");
    tm->Init();
    AppLogAdd("TextManagerの初期化成功\n");
    AppLogAdd("GraphicManagerの初期化開始\n");
    gm->Init();
    AppLogAdd("GraphicManagerの初期化成功\n");
    AppLogAdd("BackLogManagerの初期化開始\n");
    log->Init();
    AppLogAdd("BackLogManagerの初期化成功\n");    AppLogAdd("シナリオファイルのパース開始\n");
    if(!scenario->ExtractScript(GlobalValue::ScenarioName)) {
        AppLogAdd("シナリオファイルのパースに失敗\n");
        return false;
    }
    AppLogAdd("シナリオファイルのパース成功\n");
    AppLogAdd("全ての初期化処理成功\n");
    AppLogAdd("シナリオファイルのアセンブリコードをダンプします\n");
    scenario->DumpAssembly("dump.txt");
    SetWindowMenu(IDR_MENU1,&GameFrame::MenuCmdHandler);
    AppLogAdd("メニューバーを追加\n");
    return true;
}

void GameFrame::OneFrame() {
    last_time=GetNowCount();
    GlobalValue::key.Update();
    gm->DrawLayer();
    tm->PrintMessage();
	//↑が押されていればバックログにstateを切り替える
	if(GlobalValue::key.KeyUp()) GlobalValue::state=BackLogging;
    switch (GlobalValue::state) {
        //ここのcase文の順番けっこう大事
        case ScriptWaiting:
            if (last_time-GlobalValue::ScriptWaitCntStart>GlobalValue::ScriptWait) {
                GlobalValue::state=Nothing;
            }
            return;
            break;
        case Nothing:
            {
            Assembly temp;
            temp=scenario->ReturnNextCode();
            if (temp.cmd==eof) {
                break;
            }
            ExecuteAssembly(temp);
            }
            break;
        case TextPrinting:
            tm->UpdateScreenBuffer();
            break;
        case Transitioning:
            break;
        case ClickWaiting:
            if (GlobalValue::key.MouseL()||GlobalValue::key.KeyAccept()) {
                if (tm->GetTextFinished()) {
                    GlobalValue::state=Nothing;
                }else{
                    GlobalValue::state=TextPrinting;
                }
            }
            break;
        case SelectWaiting:
            if (selm->StartSelection()) {
                stk.push(selm->GetSelected());
                GlobalValue::state=Nothing;
            }
            break;
		case BackLogging:
			if (GlobalValue::key.MouseR()) {
				GlobalValue::state=Nothing;
			}
			log->ViewLog();
			break;
    }
    RegulateFrameRate(1000/60);
}

void GameFrame::RegulateFrameRate(int frame_time) {
    int wait=frame_time-GetNowCount()+last_time;
    if (wait>0){
        Sleep(wait);
    }/*else{
        DrawString(0,0,"コマ落ち中",GetColor(0,255,0));
    }*/
}

int GameFrame::MenuCmdHandler(WORD cmd) {
    switch (cmd) {
        case IDM_SCREEN_WINDOW:
            ChangeWindowMode(TRUE);
            break;
        case IDM_SCREEN_FULLSCREEN:
            ChangeWindowMode(FALSE);
            break;
        case IDM_SAVE_DATA_001:
            break;
        case IDM_SAVE_DATA_002:
            break;
        case IDM_SAVE_DATA_003:
            break;
        case IDM_SAVE_DATA_004:
            break;
        case IDM_SAVE_DATA_005:
            break;
        case IDM_SAVE_DATA_006:
            break;
        case IDM_SAVE_DATA_007:
            break;
        case IDM_SAVE_DATA_008:
            break;
        case IDM_SAVE_DATA_009:
            break;
        case IDM_SAVE_DATA_0010:
            break;
        case IDM_LOAD_DATA_001:
            break;
        case IDM_LOAD_DATA_002:
            break;
        case IDM_LOAD_DATA_003:
            break;
        case IDM_LOAD_DATA_004:
            break;
        case IDM_LOAD_DATA_005:
            break;
        case IDM_LOAD_DATA_006:
            break;
        case IDM_LOAD_DATA_007:
            break;
        case IDM_LOAD_DATA_008:
            break;
        case IDM_LOAD_DATA_009:
            break;
        case IDM_LOAD_DATA_0010:
            break;
        case IDM_VERSION:
            MessageBox(GetMainWindowHandle(),"His Lie.\nCopyright(c) 大阪教育大学附属高校池田校舎\n\tデジタルクリエーション同好会\nSupported by Iridium Engine","バージョン情報",MB_OK);
            break;
        case IDM_EXIT:
            if (MessageBox(GetMainWindowHandle(),"終了してもよろしいですか？","確認",MB_YESNO)==IDYES) {
                GlobalValue::EndFlag=true;
            }
            break;
    }
    return 0;
}

