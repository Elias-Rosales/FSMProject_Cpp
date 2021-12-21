#ifndef READDATA_H
#define READDATA_H
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "SyntaxVerification.h"
using namespace std;

class ReadData: public SV{
    protected:
        ifstream conf_file;
        ifstream book;
        string line;
        string timescale, delay, dumpvars, period_clk, clk, rst;
        string c_state , n_state, edge;
        int num_cases,vers;
        bool active_rst;
        smatch sm;
        string states_string,initial_state,inputs_string,outputs_string;
        vector<string> states,inputs,outputs,input_values;
        vector<int> inputs_s,outputs_s;
        map<string,vector<string>> next_state={},output_state={};
        int num_bits_states=0, num_bits_inputs=0, num_bits_outputs=0;
        bool fsm_type = false;
    public:
        ReadData(int _argc, std::vector<char*> _args);
        ~ReadData();
        void config();
        void states_names();
        void in_out();
        void input_val();
        void fsm();
};
#endif