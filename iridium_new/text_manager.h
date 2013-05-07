#ifndef __TEXT_MANAGER_H__
#define __TEXT_MANAGER_H__
#include "DxLib.h"
#include <string>
#include <vector>

using namespace std;

class TextManager{
    int defualt_font_handle;
    int font_width,font_height;
    string message_buffer;
    vector<string> screen_buffer;
    int wait_cnt,current_line,message_buf_pos;
    void ResetScreenBuffer();
    int glyph_handle[64];
    int glyph_frame,dir;
    bool IsKanji(unsigned char c);
public:
    void SetMessage(string s);
    void PrintMessage();
    void UpdateScreenBuffer();
    void Init();
    bool GetTextFinished();
    void GetNextCharCoordinate(int* x,int* y);
};

#endif