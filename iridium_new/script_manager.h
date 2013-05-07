#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__

#include <boost/spirit.hpp>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <stack>
#include "DxLib.h"
#include "global_value.h"

using namespace std;
using namespace boost;
using namespace boost::spirit;

enum Operators {
    pushi,//0
    pushs,
    invoke,
    jump,
    addi,
    subi,//5
    muli,
    divi,
    eqi,
    neqi,
    gtei,//10
    stei,
    gti,
    sti,
    jumpf,
    label,
    loadi,
    storei,
    eof,
};

class Assembly {
public:
    Operators cmd;
    vector<any> parameter;
    Assembly() {
        cmd=eof;
    }
};

class ProgramExtractor {
public:
    static vector<Assembly> ProgramList;
    static map<string,int> LabelMap;
    static map<string,any> VarMap;
    static string temp_s,temp_ident;
    static int block,nest;
    static int blockno[32];
    static stack<string> IdentStack;

    static void LiteralInteger(int n) {
        Assembly temp;
        temp.cmd=pushi;
        temp.parameter.push_back(n);
        ProgramList.push_back(temp);
    }

    static void LiteralStringStart(char c) {
        temp_s.clear();
    }

    static void LiteralStringAppend(char c) {
        if (c!='\"') {
            temp_s.append(1,c);
        }
    }

    static void LiteralStringEnd(char c) {
        Assembly temp;
        temp.cmd=pushs;
        temp.parameter.push_back(temp_s);
        ProgramList.push_back(temp);
    }

    static void InvokeFunc(const char*,const char*) {
        Assembly temp;
        temp.cmd=invoke;
        for (int i=0;i<func_id_count;i++) {
            if (GlobalValue::FuncName[i]==IdentStack.top()) {
                temp.parameter.push_back(static_cast<IntegratedFunc>(i));
            }
        }
        ProgramList.push_back(temp);
        IdentStack.pop();
    }

    static void IdentStart(char c) {
        temp_ident.clear();
        temp_ident.append(1,c);
    }

    static void IdentAppendString(char c) {
       temp_ident.append(1,c);
    }

    static void Label(char c) {
        Assembly temp;
        temp.cmd=label;
        temp.parameter.push_back(temp_ident);
        ProgramList.push_back(temp);
        LabelMap.insert(pair<string,int>(temp_ident,ProgramList.size()-1));
    }

    static void GotoStatement(const char*,const char*) {
        Assembly temp;
        temp.cmd=jump;
        temp.parameter.push_back(temp_ident);
        ProgramList.push_back(temp);
    }
    static void Add(const char*,const char*) {
        Assembly temp;
        temp.cmd=addi;
        ProgramList.push_back(temp);
    }

    static void Sub(const char*,const char*) {
        Assembly temp;
        temp.cmd=subi;
        ProgramList.push_back(temp);
    }

    static void Mul(const char*,const char*) {
        Assembly temp;
        temp.cmd=muli;
        ProgramList.push_back(temp);
    }

    static void Div(const char*,const char*) {
        Assembly temp;
        temp.cmd=divi;
        ProgramList.push_back(temp);
    }

    static void Equal(const char*,const char*) {
        Assembly temp;
        temp.cmd=eqi;
        ProgramList.push_back(temp);
    }
    static void NotEqual(const char*,const char*) {
        Assembly temp;
        temp.cmd=neqi;
        ProgramList.push_back(temp);
    }
    static void GreaterOrEqual(const char*,const char*) {
        Assembly temp;
        temp.cmd=gtei;
        ProgramList.push_back(temp);
    }
    static void SmallerOrEqual(const char*,const char*) {
        Assembly temp;
        temp.cmd=stei;
        ProgramList.push_back(temp);
    }
    static void Greater(const char*,const char*) {
        Assembly temp;
        temp.cmd=gti;
        ProgramList.push_back(temp);
    }
    static void Smaller(const char*,const char*) {
        Assembly temp;
        temp.cmd=sti;
        ProgramList.push_back(temp);
    }

    static void PushBlock() {
        block++;
        nest++;
        blockno[nest]=block;
    }

    static void PopBlock() {
        nest--;
    }

    static void IfBlockStart(const char*,const char*) {
        PushBlock();
        Assembly temp;
        temp.cmd=jumpf;
        temp.parameter.push_back("IF_TRUE_BLOCK_"+lexical_cast<string>(blockno[nest]));
        ProgramList.push_back(temp);
        temp.cmd=jump;
        temp.parameter.clear();
        temp.parameter.push_back("IF_FALSE_BLOCK_"+lexical_cast<string>(blockno[nest]));
        ProgramList.push_back(temp);
    }

    static void IfTrueBlockStart(char c) {
        string label_name="IF_TRUE_BLOCK_"+lexical_cast<string>(blockno[nest]);
        Assembly temp;
        temp.cmd=label;
        temp.parameter.push_back(label_name);
        ProgramList.push_back(temp);
        LabelMap.insert(pair<string,int>(label_name,ProgramList.size()-1));
    }

    static void IfFalseBlockStart(const char*,const char*) {
        string label_name="IF_FALSE_BLOCK_"+lexical_cast<string>(blockno[nest]);
        Assembly temp;
        temp.cmd=label;
        temp.parameter.push_back(label_name);
        ProgramList.push_back(temp);
        LabelMap.insert(pair<string,int>(label_name,ProgramList.size()-1));
    }

    static void IfEnd(const char*,const char*) {
        string label_name="IF_END_"+lexical_cast<string>(blockno[nest]);
        Assembly temp;
        temp.cmd=label;
        temp.parameter.push_back(label_name);
        ProgramList.push_back(temp);
        LabelMap.insert(pair<string,int>(label_name,ProgramList.size()-1));
        PopBlock();
    }

    static void IfTrueBlockEnd(const char*,const char*) {
        Assembly temp;
        temp.cmd=jump;
        temp.parameter.push_back("IF_END_"+lexical_cast<string>(blockno[nest]));
        ProgramList.push_back(temp);
    }

    static void DefineVariable(const char*,const char*) {
        VarMap.insert(pair<string,int>(temp_ident,0));
    }

    static void StoreToVariable(char c) {
        Assembly temp;
        temp.cmd=storei;
        temp.parameter.push_back(IdentStack.top());
        ProgramList.push_back(temp);
        IdentStack.pop();
    }

    static void LoadFromVariable(const char*,const char*) {
        Assembly temp;
        temp.cmd=loadi;
        temp.parameter.push_back(temp_ident);
        ProgramList.push_back(temp);
    }

    static void PushIdentName(char c) {//const char*,const char*) {
        IdentStack.push(temp_ident);
    }

    static void WhileBlockStart(const char*,const char*) {
        PushBlock();
        string label_name="WHILE_START_"+lexical_cast<string>(blockno[nest]);
        Assembly temp;
        temp.cmd=label;
        temp.parameter.push_back(label_name);
        ProgramList.push_back(temp);
        LabelMap.insert(pair<string,int>(label_name,ProgramList.size()-1));
    }

    static void WhileTrueBlockStart(char c) {
        string label_name="WHILE_TRUE_BLOCK_"+lexical_cast<string>(blockno[nest]);
        Assembly temp;
        temp.cmd=label;
        temp.parameter.push_back(label_name);
        ProgramList.push_back(temp);
        LabelMap.insert(pair<string,int>(label_name,ProgramList.size()-1));
    }

    static void WhileEnd(const char*,const char*) {
        Assembly temp;
        temp.cmd=jump; 
        temp.parameter.push_back("WHILE_START_"+lexical_cast<string>(blockno[nest]));
        ProgramList.push_back(temp);

        string label_name="WHILE_END_"+lexical_cast<string>(blockno[nest]);
        temp.parameter.clear();
        temp.cmd=label;
        temp.parameter.push_back(label_name);
        ProgramList.push_back(temp);
        LabelMap.insert(pair<string,int>(label_name,ProgramList.size()-1));

        PopBlock();
    }

    static void WhileComp(const char*,const char*) {
        Assembly temp;
        temp.cmd=jumpf;
        temp.parameter.push_back("WHILE_TRUE_BLOCK_"+lexical_cast<string>(blockno[nest]));
        ProgramList.push_back(temp);
        temp.cmd=jump;
        temp.parameter.clear();
        temp.parameter.push_back("WHILE_END_"+lexical_cast<string>(blockno[nest]));
        ProgramList.push_back(temp);
    }

};

// 文法を定義した definition テンプレートをメンバに持つ
struct IridiumParser : public grammar<IridiumParser>
{
	template<typename S> struct definition {
		rule<S> program,statement,func,ident,expr,term,fact,comp_exp;

		definition(const IridiumParser& self) {
            program=+(statement);
            statement=func>>ch_p(';')
                    |(str_p("int")|str_p("string"))>>ident[&ProgramExtractor::DefineVariable]>>*(ch_p(',')>>ident[&ProgramExtractor::DefineVariable])>>ch_p(';')
                    |ident>>ch_p('=')[&ProgramExtractor::PushIdentName]>>expr>>ch_p(';')[&ProgramExtractor::StoreToVariable]
                    |ident>>ch_p(':')[&ProgramExtractor::Label]
                    |str_p("goto")>>ident[&ProgramExtractor::GotoStatement]>>ch_p(';')
                    |str_p("if")>>comp_exp[&ProgramExtractor::IfBlockStart]>>ch_p('{')[&ProgramExtractor::IfTrueBlockStart]>>program>>ch_p('}')>>eps_p[&ProgramExtractor::IfTrueBlockEnd]>>eps_p[&ProgramExtractor::IfFalseBlockStart]>>!(str_p("else")>>ch_p('{')>>program>>ch_p('}'))>>eps_p[&ProgramExtractor::IfEnd]
                    |str_p("while")[&ProgramExtractor::WhileBlockStart]>>comp_exp[&ProgramExtractor::WhileComp]>>ch_p('{')[&ProgramExtractor::WhileTrueBlockStart]>>program>>ch_p('}')>>eps_p[&ProgramExtractor::WhileEnd];
            func=ident>>ch_p('(')[&ProgramExtractor::PushIdentName]>>
                !(comp_exp>>*(ch_p(',')>>comp_exp))>>ch_p(')')>>eps_p[&ProgramExtractor::InvokeFunc];
            ident=(alpha_p[&ProgramExtractor::IdentStart]|ch_p('_')[&ProgramExtractor::IdentStart])>>*((alnum_p[&ProgramExtractor::IdentAppendString]|ch_p('_')[&ProgramExtractor::IdentAppendString]));
            //identにて謎のエラー発生することあり
            expr=term>>*((ch_p('+')>>term)[&ProgramExtractor::Add]
		                |(ch_p('-')>>term)[&ProgramExtractor::Sub]);
			term=fact>>*((ch_p('*')>>fact)[&ProgramExtractor::Mul]
		                |(ch_p('/')>>fact)[&ProgramExtractor::Div]);
			fact=func
                 |int_p[&ProgramExtractor::LiteralInteger]
                 |ident[&ProgramExtractor::LoadFromVariable]
			     |ch_p('(')>>expr>>ch_p(')')
                 |ch_p('\"')[&ProgramExtractor::LiteralStringStart]>>*(anychar_p[&ProgramExtractor::LiteralStringAppend]-'\"')>>ch_p('\"')[&ProgramExtractor::LiteralStringEnd];
            comp_exp=expr>>!((str_p("==")>>expr)[&ProgramExtractor::Equal]
                    |(str_p("!=")>>expr)[&ProgramExtractor::NotEqual]
                    |(str_p(">=")>>expr)[&ProgramExtractor::GreaterOrEqual]
                    |(str_p("<=")>>expr)[&ProgramExtractor::SmallerOrEqual]
                    |(str_p(">")>>expr)[&ProgramExtractor::Greater]
                    |(str_p("<")>>expr)[&ProgramExtractor::Smaller]);
        }
		const rule<S>& start() const {
            return program;
        }
	};
};


class ScriptManager {
	IridiumParser parser;
    int pc;
    vector<Assembly> code;
    map<string,int> label_map;
    map<string,any> var_map;
    string program_buf;
    string FuncNameFromId(IntegratedFunc id);
    IntegratedFunc IdFromFunacName(string name);
    int cmd_profiler;
public:
    ScriptManager() {
        pc=0;
    }
    bool ExtractScript(string file_name);
    Assembly ReturnNextCode();
    bool JumpToLabel(string label);
    void DumpAssembly(string filename);
    void StoreVariable(string name,any i);
    any LoadVariable(string name);
};

#endif