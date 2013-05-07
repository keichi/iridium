#include "script_manager.h"

bool ScriptManager::ExtractScript(string file_name) {
    ifstream ifs(file_name.c_str());
    program_buf.clear();
    if(!ifs) {
        return false;
    }
    while(!ifs.eof()) {
        string temp;
        getline(ifs,temp);
        program_buf+=temp;
    }
    parse_info<> r=parse(program_buf.c_str(),parser,space_p);
    if (r.full) {
        //パーサからプログラム・変数定義・ラベル定義を収得
        code=ProgramExtractor::ProgramList;
        label_map=ProgramExtractor::LabelMap;
        //変数の中身を破壊しないようにコピー
        map<string,any>::iterator it;
        for (it=ProgramExtractor::VarMap.begin();it!=ProgramExtractor::VarMap.end();it++) {
            map<string,any>::iterator found;
            found=var_map.find((*it).first);
            if (found==var_map.end()) {
                var_map.insert(*it);
            }
        }
        return true;
    }else{
        return false;
    }
}

Assembly ScriptManager::ReturnNextCode() {
    if (pc<static_cast<int>(code.size())) {
        /*AppLogAdd("アセンブリ実行:%d,前回の所要時間:%d\n",code[pc].cmd,GetNowCount()-cmd_profiler);
        if (code[pc].cmd==invoke) {
            AppLogAdd("↑関数名:%s\n",GlobalValue::FuncName[any_cast<IntegratedFunc>(code[pc].parameter[0])].c_str());
        }
        cmd_profiler=GetNowCount();*/
        return code[pc++];
    }
    Assembly temp;
    return temp;
}

bool ScriptManager::JumpToLabel(string label) {
    map<string,int>::iterator it;
    it=label_map.find(label);
    if (it==label_map.end()) {
        return false;
    }
    pc=(*it).second;
    return true;
}

void ScriptManager::DumpAssembly(string filename) {
    ofstream ofs(filename.c_str());
    for(int i=0;i<static_cast<int>(code.size());i++) {
        switch(code[i].cmd) {
            case pushi:
                ofs<<"pushi";
                break;
            case pushs:
                ofs<<"pushs";
                break;
            case invoke:
                ofs<<"invoke";
                break;
            case jump:
                ofs<<"jump";
                break;
            case addi:
                ofs<<"addi";
                break;
            case subi:
                ofs<<"subi";
                break;
            case muli:
                ofs<<"muli";
                break;
            case divi:
                ofs<<"divi";
                break;
            case eqi:
                ofs<<"eqi";
                break;
            case neqi:
                ofs<<"neqi";
                break;
            case gtei:
                ofs<<"gtei";
                break;
            case stei:
                ofs<<"stei";
                break;
            case gti:
                ofs<<"gti";
                break;
            case sti:
                ofs<<"sti";
                break;
            case jumpf:
                ofs<<"jumpf";
                break;
            case loadi:
                ofs<<"loadi";
                break;
            case storei:
                ofs<<"storei";
                break;
            case label:
                break;
       }
        if (!code[i].parameter.empty()) {
            if (code[i].cmd==label) {
                ofs<<any_cast<string>(code[i].parameter[0])<<":";
            }else if (code[i].cmd==invoke) {
                ofs<<" ";
                ofs<<GlobalValue::FuncName[any_cast<IntegratedFunc>(code[i].parameter[0])];
            }else{
                ofs<<" ";
                for (int j=0;j<static_cast<int>(code[i].parameter.size());j++) {
                    if (code[i].parameter[j].type()==typeid(int)) {
                        ofs<<any_cast<int>(code[i].parameter[j]);
                        ofs<<",";
                    }else if (code[i].parameter[j].type()==typeid(string)) {
                        ofs<<any_cast<string>(code[i].parameter[j]);
                    }
                }
            }
        }
        ofs<<endl;
    }
}

void ScriptManager::StoreVariable(string name,any i) {
    map<string,any>::iterator it;
    it=var_map.find(name);
    if (it==var_map.end()) {
        return;
    }
    (*it).second=i;
}

any ScriptManager::LoadVariable(string name) {
    map<string,any>::iterator it;
    it=var_map.find(name);
    if (it==var_map.end()) {
        return any(0);
    }
    return (*it).second;
}