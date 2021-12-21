#ifndef WRITEDATATB_H
#define WRITEDATATB_H
//WRITE TB DATA CLASS
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <algorithm>
#include "Design.h"
using namespace std;

class WriteDataTB:public Design{
    private:
        int num_in,num_out;
        ofstream TB;
        bool flag1;
        vector<vector<int>> stimuli;
    public:
    /*  Constructor  */
        WriteDataTB(int _argc, std::vector<char*> _args);
    /*  Destructor  */
        ~WriteDataTB();
    /*  Member Functions  */
        void header();
        void in_out();
        void dut();
        void initialbegin();
        void path_finder();
        string dectobin(int,int);
};

#endif