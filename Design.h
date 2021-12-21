#ifndef DESIGN_H
#define DESIGN_H
#include "ReadData.h"

class Design: public ReadData{
    protected:
        ofstream Design_F;
        string name = "";
        string path = "";
        string module_name;
        map<int,vector<string>> inp;
        map<int,vector<string>> out;
        vector<int> temp_out = {};
        map<int,vector<string>>::iterator it;
    public:
        Design(int _argc, std::vector<char*> _args);
        ~Design();
        void module();
        void inputs_print();
        void outputs_print();
        void States();
        void StateReg();
        void NextStateLogic();
        void OutputLogic();
};
#endif