#include "SyntaxVerification.h"

SV::SV(int _argc, std::vector<char*> _args){
    SV::len_argv = _argc;
    SV::args = _args;
    Args();
    extensions();
    open_error();
    pathVerification();
    getFSM();
    getFSMpath();
}
SV::~SV(){}
void SV::Args(){
    if(SV::len_argv < 2){
        cout << "\n[ERROR] No argument has been declared\n";
        cout << "\tUse the following syntax: main.py [FSM_file.txt] [Testbench_Name.sv] [conf.ini] [Design_Name.sv]\n";
        exit(EXIT_FAILURE);
    }
    else{
        SV::FSM_path = args[1];
        if (len_argv == 3)
            SV::TestB = args[2];
        else if (len_argv == 4){
            SV::TestB = args[2];
            SV::conf = args[3];
        }
        else if (len_argv == 5){
            SV::TestB = args[2];
            SV::conf = args[3];
            SV::design_path = args[4];
            SV::design_path_flag = true;
        }
    }
}
void SV::extensions(){
    if(!regex_match(FSM_path,m,regex("^.*(.txt)$"))){
        cout << "\n[ERROR] InputFileName = " << FSM_path << endl;
        cout << "\tThe input file must have a .txt extension" << endl;
        exit(EXIT_FAILURE);
    }
    if(!regex_match(TestB,m,regex("^.*(.sv)$"))){
        cout << "\n[ERROR] TestbenchFileName = " << TestB << endl;
        cout << "\tThe testbench file must have a .sv extension" << endl;
        exit(EXIT_FAILURE);
    }
    if(design_path_flag){
        if(!regex_match(design_path,m,regex("^.*(.sv)$"))){
            cout << "\n[ERROR] DesignFileName = " << design_path << endl;
            cout << "\tThe design file must have a .sv extension" << endl;
            exit(EXIT_FAILURE);
        }
    }
    if(!regex_match(conf,m,regex("^.*(.ini)$"))){
        cout << "\n[ERROR] ConfFileName = " << conf << endl;
        cout << "\tThe configuration file must have a .ini extension" << endl;
        exit(EXIT_FAILURE);
    }
}
void SV::open_error(){
    string temp = "";
    try{
        ifstream file(FSM_path);
        if(!file){
            temp = FSM_path;
            throw "[ERROR] FileNotFound = No such file or directory:";
        }
        ifstream filec(conf);
        if(!filec){
            temp = conf;
            throw "[ERROR] FileNotFound = No such file or directory:";
        }
    }catch(const char* cstr){
        cerr << cstr << temp << endl;
        exit(EXIT_FAILURE);
    }
}
void SV::pathVerification(){
    if(regex_search(FSM_path,m,regex(R"(^[C|c]:.*[\\\/](\w+.txt)$)"))){
        FSM = m[1].str();
        FSM_txt = m[1].str();
    }else{
        FSM = FSM_path;
        FSM_txt = FSM_path;
    }
    if(design_path_flag){
        if(regex_search(design_path,m,regex(R"(^[C|c]:.*[\\\/](\w+.sv)$)")))
            design = m[1].str();
        else
            design = design_path;
    }
}
void SV::getFSM(){
    FSM.replace(FSM.end()-4,FSM.end(),ext.begin(),ext.end());
    //return FSM;
}
void SV::getFSMpath(){
    FSM_path.replace(FSM_path.end()-4,FSM_path.end(),ext.begin(),ext.end());
    //return FSM_path;
}
/*string SV::getTB(){
    return TestB;
}
string SV::getDesign(){
    return design;
}
string SV::getDesignpath(){
    return design_path;
}
string SV::getconf(){
    return conf;
}
bool SV::getDesignflag(){
    return design_path_flag;
}*/