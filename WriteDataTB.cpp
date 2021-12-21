#include "WriteDataTB.h"
#include <algorithm>
#include <math.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <stack>
using namespace std;

WriteDataTB::WriteDataTB(int _argc, std::vector<char*> _args) : Design(_argc,_args){
    TB.open(TestB);
    header();
    in_out();
    dut();
    path_finder();
    initialbegin();
}
WriteDataTB::~WriteDataTB(){}
void WriteDataTB::header(){
    TB << "`timescale " << timescale <<endl;
    TB << "\nmodule " << module_name << "_TB;\n";
    TB.flush();
}

void WriteDataTB::in_out(){
    int maxElement;
    num_in = inputs.size();
    num_out = outputs.size();
    bool flag1 = false;
    bool flag2 = false;
    //inputs
    TB << "  reg " << clk <<"," << rst <<";\n";
    maxElement = *max_element(inputs_s.begin(),inputs_s.end());
    vector<vector<string>> in;
    in.resize(maxElement);

    if (num_in !=0){

        for(int i=0;i<inputs.size();i++){
            in[inputs_s[i]-1].push_back(inputs[i]);
        } 
        for(int i=0;i<maxElement;i++){
            if(!in[i].empty()){
                if(i+1==1){
                    TB << "  reg ";
                    for(int j=0; j<in[i].size();j++){
                        if(j==in[i].size()-1){
                            TB << in[i][j] << "_TB;\n";
                        }
                        else TB << in[i][j] << "_TB, ";
                    }
                }
                else{
                    TB << "  reg [" << i << ":0] ";
                    for(int j=0; j<in[i].size(); j++){
                        if(j==in[i].size()-1){
                            TB << in[i][j] << "_TB;\n";
                        }
                        else TB << in[i][j] << "_TB, ";
                    }
                }
            }
        }
    }
    else{
        flag1 = true;
    }
    //outputs
    maxElement = *max_element(outputs_s.begin(),outputs_s.end());
    vector<vector<string>> out;
    out.resize(maxElement);

    if (num_out !=0){
        for(int i=0;i<outputs.size();i++){
            out[outputs_s[i]-1].push_back(outputs[i]);
        }
        for(int k=0;k<maxElement;k++){
            if(!out[k].empty()){
                if(k+1==1){
                    TB << "  wire ";
                    for(int j=0; j<out[k].size();j++){
                        if(j==out[k].size()-1){
                            TB << out[k][j] << "_TB;\n";
                        }
                        else 
                        TB << out[k][j] << "_TB, ";
                    }
                }
                else{
                    TB << "  wire [" << k << ":0] ";
                    for(int j=0; j<out[k].size(); j++){
                        if(j==out[k].size()-1){
                            TB << out[k][j] << "_TB;\n";
                        }
                        else
                        TB << out[k][j] << "_TB, ";
                    }
                }
            }
        }
    }
    else{
        flag2 = true;
    }
    if (flag1 && flag2){
        TB << ");" << endl;
    }
    TB.flush();
}

void WriteDataTB::dut(){
    num_in = inputs.size();
    num_out = outputs.size();
    flag1 = false;
    bool flag2 = false;

    TB << "\n  " << module_name << " DUT(." << clk <<"(" << clk <<"), ."<< rst <<"(" << rst <<"), ";

    if (num_in !=0){
        for (int i = 0; i < num_in; i++){
            if (i == num_in - 1){
                if(num_out!=0){
                    TB << "." << inputs[i] << "(" << inputs[i] << "_TB),\n            ";
                }
                else{
                    TB << "." << inputs[i] << "(" << inputs[i] << "_TB));" << endl;
                }
            }
            else{
                TB << "." << inputs[i] << "(" << inputs[i] << "_TB), " ;
            }
        }
    }
    else{
        flag1 = true;
    }

    if (num_out !=0){
        for (int i = 0; i < num_out; i++){
            if (i == num_out - 1){
                TB << "." << outputs[i] << "(" << outputs[i] << "_TB));" << endl;
            }
            else{
                TB << "." << outputs[i] << "(" << outputs[i] << "_TB), ";
            }
        }
    }
    else{
        flag2 = true;
    }

    if (flag1 && flag2){
        TB << ");" << endl;
    }
    TB.flush();
}

void WriteDataTB::path_finder(){
    stack<string> stack;
    stack.push(initial_state);
    vector<string> base;
    base.push_back(initial_state);
    vector<string> statesArray;
    statesArray.push_back(initial_state);
    vector<string> targets;

    while (!stack.empty()){
        string actual = stack.top();
        stack.pop();
        vector<string> statesPossibilites = next_state[actual];
        int i = 0;
        for (auto iter = statesPossibilites.begin(); iter != statesPossibilites.end(); ++iter){
            if (find(statesArray.begin(),statesArray.end(),*iter) == statesArray.end()){
                statesArray.push_back(*iter);
                stack.push(*iter);
                base.push_back(actual);
            }
            else{
                i += 1;
            }
        }
        if (i == statesPossibilites.size()){
            targets.push_back(actual);
        }
    }
    
    /* cout << "\ntargets";
    for (auto iter2 = targets.begin(); iter2 != targets.end(); ++iter2)
        cout << *iter2;
    cout << endl; */

    // Rutas
    map<string,vector<string>> paths;
    for (auto iter = targets.begin(); iter != targets.end(); ++iter){
        vector<string> pathfollowed;
        pathfollowed.push_back(*iter);
        string tempo = *iter;
        while (tempo != initial_state){
            for (int i = 0; i < statesArray.size(); ++i){
                if (statesArray[i] == tempo){
                    tempo = base[i];
                }
            }
            pathfollowed.insert(pathfollowed.begin(),tempo);
        }
        paths.insert(pair<string,vector<string>>(*iter,pathfollowed));
    }

    /* for (map<string, vector<string>>::iterator it=paths.begin(); it!=paths.end(); ++it){
        cout << it->first << " => ";
        for( auto vec_iter = it->second.begin() ; vec_iter != it->second.end() ; ++vec_iter ){
            cout << *vec_iter << " ";
        }
        cout << "\n" ;
    } */

    for (map<string,vector<string>>::iterator iter = paths.begin(); iter != paths.end(); ++iter){
        int ind = 0;
        vector<int> stimulus;
        vector<string> list = paths[iter->first];
        string actual = list[0];
        vector<string> statesPossibilites = next_state[actual];
        for (int i = 0; i < list.size() - 1; ++i){
            string nextStateEvaluate = list[i+1];
            auto it = find(statesPossibilites.begin(), statesPossibilites.end(), nextStateEvaluate);
            int index = it - statesPossibilites.begin();
            stimulus.push_back(index);
            statesPossibilites = next_state[nextStateEvaluate];
        }
        stimuli.push_back(stimulus);
        /*
        cout << "\nstimulus : ";
        for (auto iter2 = stimulus.begin(); iter2 != stimulus.end(); ++iter2)
            cout << *iter2;
            */
    }

    /* cout << endl;
    for(int i = 0; i < stimuli.size(); ++i) {
        for(int j = 0; j < stimuli[i].size(); ++j){
            cout << "stimuli ij: " << stimuli[i][j] << endl;
        }
    } */
}

void WriteDataTB::initialbegin(){
    TB << "\n  initial begin\n      "<<clk<<" = 1'b0; \n      "<<rst<<" = 1'b1;\n  end\n";
    TB << "\n  always begin "<<clk<<"=~"<<clk<<";"<< period_clk <<"; end\n";
    TB << "\n  initial begin\n    $dumpfile(\"" << module_name << ".vcd\");\n    ";
    TB << "$dumpvars(" << dumpvars << "," << module_name << "_TB);\n\n";
    
    string in_vector;
    
    int num_totcases = pow(2,num_bits_inputs);
    if(!flag1){
        for(int i=0;i<num_in;i++){
            if(i==num_in-1) in_vector = in_vector+inputs[i]+"_TB";
            else in_vector = in_vector+inputs[i]+"_TB,";
        }
        // Initial case
        TB << "    {"+in_vector+"} = "
        << num_bits_inputs << "'b"+dectobin(num_bits_inputs,0)+"; #1" << endl;
        TB << "    "<<rst<<" = 1'b0; "<<delay<<endl;
        for(int i = 0; i < stimuli.size(); ++i) {
            for(int j = 0; j < stimuli[i].size(); ++j){
                //cout << "stimuli ij: " << stimuli[i][j] << endl;
                TB << "    {"+in_vector+"} = " << num_bits_inputs << "'b"+dectobin(num_bits_inputs,stimuli[i][j])+"; " << delay << endl;
            }
            if(i<stimuli.size()-1){
                TB << "    "<<rst<<" = 1'b1; " << delay << endl;
                TB << "    "<<rst<<" = 1'b0; " << delay << endl;
            }
        }
    }
    TB << "\n    $finish;\n  end\nendmodule";
    TB.close();
}

string WriteDataTB::dectobin(int _bits,int _num){
    long i;
    string bin_num;
    for (i = 1 << _bits-1; i > 0; i = i / 2) //for 2^(bits-1), while i > 0
    {
      if((_num & i) != 0)   //check if i bit is 1 or 0
      {
        bin_num=bin_num+"1";
      }
      else
      {
        bin_num=bin_num+"0";
      }
    }
    return bin_num;
}
