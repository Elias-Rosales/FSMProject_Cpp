#ifndef SYNTAXVERIFICATION_H
#define SYNTAXVERIFICATION_H
/* SYNTAX VERIFICATION CLASS*/
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <exception>
using namespace std;

class SV{
    protected:
    /* data */
        string FSM = "FSM.txt";
        string FSM_txt = "FSM.txt";
        string FSM_path = "FSM.txt";
        string conf = "conf.ini";
        string TestB = "testbench.sv";
        string design;
        string design_path;
        bool design_path_flag = false;
        smatch m;
        int len_argv;
        vector<char*> args;
        string ext = ".sv";
        void Args();
        void extensions();
        void open_error();
        void pathVerification();
    public:
        /* Constructor */
        SV(int _argc, std::vector<char*> _args);
        /* Destructor */
        ~SV();
        /* Member Functions */
        void getFSM();
        void getFSMpath();
};
#endif