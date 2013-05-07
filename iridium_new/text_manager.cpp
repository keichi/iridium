#include "text_manager.h"
#include "global_value.h"

void TextManager::Init() {
    //次に何行目のバッファに書き込むか
    this->current_line=0;
    this->defualt_font_handle=CreateFontToHandle(GlobalValue::FontName.c_str(),GlobalValue::FontSize,GlobalValue::FontThick,DX_FONTTYPE_ANTIALIASING);
    //message_buf次に表示する文字のmessage_bufferにおける位置
    this->message_buf_pos=0;
    //wait_cnt次の文字を表示するまでのフレーム数
    this->wait_cnt=GlobalValue::MessageWait;
    //screen_bufferフレームごとにスクリーンに表示するためのバッファ
    this->screen_buffer.resize(GlobalValue::MessageHeight);
    font_width=GetDrawStringWidthToHandle("a",1,this->defualt_font_handle);
    font_height=font_width*2;
    GlobalValue::FontWidth=font_width;
    GlobalValue::FontHandle=defualt_font_handle;
    int glyph_x_num=GlobalValue::GlyphAnimSizeX/GlobalValue::GlyphSizeX;
    int glyph_y_num=GlobalValue::GlyphAnimSizeY/GlobalValue::GlyphSizeY;
    LoadDivGraph(GlobalValue::GlyphName.c_str(),glyph_x_num*glyph_y_num,glyph_x_num,glyph_y_num,GlobalValue::GlyphSizeX,GlobalValue::GlyphSizeY,glyph_handle);
    glyph_frame=1;
    dir=1;
}

void TextManager::PrintMessage() {
    if (GlobalValue::MessageEnable) {
        for (int i=0;i<GlobalValue::MessageHeight;i++) {
            DrawStringToHandle(GlobalValue::MessageStartX,GlobalValue::MessageStartY+font_height*i,screen_buffer[i].c_str(),GetColor(0,0,0),this->defualt_font_handle);
            DrawStringToHandle(GlobalValue::MessageStartX,GlobalValue::MessageStartY+font_height*i,screen_buffer[i].c_str(),GetColor(255,255,255),this->defualt_font_handle);
       }
        //グリフの表示
        if (GlobalValue::state==ClickWaiting) {
            int x,y;
            this->GetNextCharCoordinate(&x,&y);
            DrawGraph(x,y,glyph_handle[glyph_frame],TRUE);
            if (glyph_frame==0||glyph_frame==GlobalValue::GlyphAnimNum-1) {
                dir=-dir;
            }
            glyph_frame+=dir;
        }
    }
}

void TextManager::SetMessage(std::string s) {
    this->message_buffer=s;
    this->message_buf_pos=0;
    this->wait_cnt=GlobalValue::MessageWait;
}

void TextManager::UpdateScreenBuffer() {
    //GobalValue::MessageWaitフレームごとにscreen_bufferに文字を追加
    if (this->wait_cnt>0) {
        wait_cnt--;
    }else{
        wait_cnt=GlobalValue::MessageWait;
        //message_buf_posが末端に到達すればstateをNothingにリセットする
        if (GetTextFinished()) {
            GlobalValue::state=Nothing;
            return;
        }
        //１行あたりの文字数を超えた場合新しい行に移動
        if (static_cast<int>(screen_buffer[current_line].size())+2>=GlobalValue::MessageWidth) {
            current_line++;
        }
        //行数があふれた場合リセット
        if (current_line>=GlobalValue::MessageHeight) {
            ResetScreenBuffer();
        }
        //current_lineに文字を追加
        string c=message_buffer.substr(message_buf_pos,2);
        //エスケープ文字判定
        if (c=="%n") {
            current_line++;
            message_buf_pos+=2;
        }
        else if (c=="%w") {
            GlobalValue::state=ClickWaiting;
            message_buf_pos+=2;
        }
        else if (c=="%c") {
            ResetScreenBuffer();
            message_buf_pos+=2;

        }else{
            if (!IsKanji(static_cast<unsigned char>((c.c_str())[0]))) {
                screen_buffer[current_line]+=c[0];
                message_buf_pos++;

            }else{
                screen_buffer[current_line]+=c;
                message_buf_pos+=2;
            }
        }
        //行数があふれた場合リセット
        if (current_line>=GlobalValue::MessageHeight) {
            ResetScreenBuffer();
        }
    }
    return;
}

void TextManager::ResetScreenBuffer() {
    this->current_line=0;
    for (int i=0;i<static_cast<int>(screen_buffer.size());i++) {
        this->screen_buffer[i].clear();
    }
}

bool TextManager::GetTextFinished() {
    if (message_buf_pos>=static_cast<int>(message_buffer.size())) {
        return true;
    }
    return false;
}

void TextManager::GetNextCharCoordinate(int *x, int *y) {
    *x=GlobalValue::MessageStartX+(this->screen_buffer[current_line].size())*font_width;
    *y=GlobalValue::MessageStartY+current_line*font_height;
}

bool TextManager::IsKanji(unsigned char c) {
    return ((c >= 0x81 && c <= 0x9f) || (c >= 0xe0 && c >= 0xff));
}