#include "ReadData.h"
#include <iostream>
#include <regex>
#include <cmath>

ReadData::ReadData(int _argc, std::vector<char*> _args) : SV(_argc,_args){
    conf_file.open(conf.c_str(),ios::in);
    config();
    book.open(FSM_txt, ios::in);
    states_names();
    in_out();
    input_val();
    fsm();}
ReadData::~ReadData(){}
void ReadData::config(){
    regex pttrn_timescale (R"(^\s*timescale\s*=\s*(\w+\/\w+).*$)");
    regex pttrn_delay (R"(^\s*delay\s*=\s*(\#\d+).*$)");
    regex pttrn_dumpvars (R"(^\s*dumpvars\s*=\s*(\d+).*$)");
    regex pttrn_cases (R"(^\s*num_cases\s*=\s*(\d+).*$)");
    regex pttrn_periodclk (R"(^\s*period_clk\s*=\s*(\#\d+).*$)");
    regex pttrn_version (R"(^\s*version\s*=\s*(\d+).*$)");
    regex pttrn_clk (R"(^\s*clk\s*=\s*(\w+).*$)");
    regex pttrn_rst (R"(^\s*rst\s*=\s*(\w+).*$)");
    regex pttrn_c_state (R"(^\s*c_state\s*=\s*(\w+).*$)");
    regex pttrn_n_state (R"(^\s*n_state\s*=\s*(\w+).*$)");
    regex pttrn_edge (R"(^\s*edge\s*=\s*(\w+).*$)");
    regex pttrn_active_rst (R"(^\s*active_rst\s*=\s*(\d+).*$)");
    while(getline(conf_file,line)){
        if(regex_search(line,sm,pttrn_timescale))
            timescale = sm[1].str();
        if(regex_search(line,sm,pttrn_delay))
            delay = sm[1].str();
        if(regex_search(line,sm,pttrn_dumpvars))
            dumpvars = sm[1].str();
        if(regex_search(line,sm,pttrn_cases))
            num_cases = stoi(sm[1].str());
        if(regex_search(line,sm,pttrn_periodclk))
            period_clk = sm[1].str();
        if(regex_search(line,sm,pttrn_version))
            vers = stoi(sm[1].str());
        if(regex_search(line,sm,pttrn_clk))
            clk = sm[1].str();
        if(regex_search(line,sm,pttrn_rst))
            rst = sm[1].str();
        if(regex_search(line,sm,pttrn_c_state))
            c_state = sm[1].str();
        if(regex_search(line,sm,pttrn_n_state))
            n_state = sm[1].str();
        if(regex_search(line,sm,pttrn_edge))
            edge = sm[1].str();
        if(regex_search(line,sm,pttrn_active_rst)){
            if(stoi(sm[1].str()) == 1)
                active_rst = true;
            else
                active_rst = false;
        }
    }
    conf_file.close();
}
void ReadData::states_names(){
    regex pttrn_states("states\\s*=\\s*(.*);");
    regex pttrn_initial_state("initial_state\\s*=\\s*(\\w+);");
    regex simple ("(\\w+)");
    string line;
    smatch sm;
    while (getline(book,line)){
        if(regex_match(line,sm,pttrn_states))
        states_string = sm.str(1);
        if(regex_match(line,sm,pttrn_initial_state))
        initial_state = sm.str(1);}
    auto words_begin = sregex_iterator(states_string.begin(),states_string.end(),simple);
    auto words_end = sregex_iterator();
    for(sregex_iterator i = words_begin; i != words_end; i++){
        smatch match = *i;
        string match_str = match.str();
        states.push_back(match_str);}
    num_bits_states = ceil(log2(states.size()));
    /*for (auto it = begin(states); it!=end(states); ++it)
        cout << *it;
    cout << endl << initial_state << endl << num_bits_states;*/
}
void ReadData::in_out(){
    book.clear(); book.seekg(0);
    regex pttrn_inputs("inputs\\s*=\\s*(.*);");
    regex pttrn_outputs("outputs\\s*=\\s*(.*);");
    regex simple ("(\\w+)");
    regex pairs("(\\s*\\w+\\s*,\\s*\\w+\\s*)");
    string line;
    smatch sm;
    string pairs_in,pairs_out,token,var;
    vector<string> in,out;
    while (getline(book,line)){
        if(regex_match(line,sm,pttrn_inputs))
        pairs_in = sm.str(1);
        if(regex_match(line,sm,pttrn_outputs))
        pairs_out = sm.str(1);
    }
    //cout << pairs_in << " " << pairs_out << endl;
    auto words_begin = sregex_iterator(pairs_in.begin(),pairs_in.end(),pairs);
    auto words_end = sregex_iterator();
    for(sregex_iterator i = words_begin; i != words_end; i++){
        smatch match = *i;
        string match_str = match.str();
        in.push_back(match_str);
    }
    /*for(int j=0; j<in.size();j++){
        cout << in[j] << endl;
    }*/
    words_begin = sregex_iterator(pairs_out.begin(),pairs_out.end(),pairs);
    words_end = sregex_iterator();
    for(sregex_iterator i = words_begin; i != words_end; i++){
        smatch match = *i;
        string match_str = match.str();
        out.push_back(match_str);
    }
    /*for(int j=0; j<out.size();j++){
        cout << out[j] << endl;
    }*/
    for(int i = 0; i<in.size();i++){
        token = in[i].substr(0,in[i].find(","));
        if(token=="clk" || token=="reset"){
            var = token+"_In";}
        else var = token;
        inputs.push_back(var);
        in[i].erase(0,in[i].find(",")+1);
        inputs_s.push_back(stoi(in[i]));
        if(i==in.size()-1){
            inputs_string+=var;}
        else 
            inputs_string+=var+",";
        num_bits_inputs += stoi(in[i]);
    }
    for(int i = 0; i<out.size();i++){
        token = out[i].substr(0,out[i].find(","));
        if(token=="clk" || token=="reset"){
            var = token+"_Out";}
        else var = token;
        outputs.push_back(var);
        out[i].erase(0,out[i].find(",")+1);
        outputs_s.push_back(stoi(out[i]));
        if(i==out.size()-1){
            outputs_string+=var;}
        else 
            outputs_string+=var+",";
        num_bits_outputs += stoi(out[i]);
    }
    //cout << inputs_string << " " << outputs_string << " " << num_bits_inputs << num_bits_outputs << endl;    
}
void ReadData::input_val(){
    book.clear(); book.seekg(0);
    regex pttrn_in("in\\s*=\\s*(.*);");
    regex simple ("(\\w+)");
    string line,values;
    smatch sm;
    while (getline(book,line)){
        if(regex_match(line,sm,pttrn_in))
        values=sm.str(1);
    }
    auto words_begin = sregex_iterator(values.begin(),values.end(),simple);
    auto words_end = sregex_iterator();
    for(sregex_iterator i = words_begin; i != words_end; i++){
        smatch match = *i;
        string match_str = match.str();
        input_values.push_back(match_str);
    }
    /*for(int i = 0; i<input_values.size();i++){
        cout << input_values[i] << endl;
    }*/
}
void ReadData::fsm(){
    book.clear(); book.seekg(0);
    regex pttrn_fsm("(\\w+)\\|\\s*(.*)\\|\\s*(.*);");
    regex simple ("(\\w+)");
    string line,states,values,key;
    vector<string> split_states,split_values;
    smatch sm;
    while (getline(book,line)){
        if(regex_match(line,sm,pttrn_fsm)){
            key=sm.str(1);
            states=sm.str(2);
            values=sm.str(3);
            auto words_begin = sregex_iterator(states.begin(),states.end(),simple);
            auto words_end = sregex_iterator();
            for(sregex_iterator i = words_begin; i != words_end; i++){
                smatch match = *i;
                string match_str = match.str();
                split_states.push_back(match_str);
            }
            words_begin = sregex_iterator(values.begin(),values.end(),simple);
            words_end = sregex_iterator();
            for(sregex_iterator i = words_begin; i != words_end; i++){
                smatch match = *i;
                string match_str = match.str();
                split_values.push_back(match_str);
            }
            next_state.insert(pair<string,vector<string>>(key,split_states));
            output_state.insert(pair<string,vector<string>>(key,split_values));
            split_states.clear();
            split_values.clear();
        }
    }
    map<string,vector<string>>::iterator it;
    it = output_state.begin();
    if(it->second.size()>1){
        fsm_type = true; //Mealy
    }
    //cout << "next_state size = "<<next_state.size() << " output_state size = " << output_state.size()<<endl;
    //cout << fsm_type;
}