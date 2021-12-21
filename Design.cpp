#include "Design.h"
#include <set>

Design::Design(int _argc, std::vector<char*> _args) : ReadData(_argc,_args){
    if(design_path_flag){
        name = design;
        path = design_path;
    }else{
        name = FSM;
        path = FSM_path;
    }
    Design_F.open(path,ios::out);
    module_name = name.replace(name.end()-3,name.end(),"");
    module();
    inputs_print();
    outputs_print();
    States();
    StateReg();
    NextStateLogic();
    OutputLogic();
}
Design::~Design(){}

void Design::module(){
    if (vers == 0){
        Design_F << "module " << module_name << "(" << clk << "," << rst << ",";
        for(int i = 0;i!=inputs_s.size();++i)
            Design_F << inputs.at(i) << ",";
        for(int i = 0;i!=outputs_s.size();++i){
            if(outputs.at(i) != outputs.at(outputs.size()-1))
                Design_F << outputs.at(i) << ",";
            else
                Design_F << outputs.at(i) << ");\n";
        }
    }else{
        Design_F << "module " << module_name << "(\n";
    }
}

void Design::inputs_print(){
    for(int i = 0;i!=inputs_s.size();++i)
        inp.insert(pair< int, vector<string> >(inputs_s.at(i),{}));
    for(it=inp.begin();it!=inp.end();++it){
        for(int i=0;i!=inputs_s.size();++i){
            if((it->first) == inputs_s.at(i))    
                (it->second).push_back(inputs.at(i));
        }
        if((it->first) == 1)
            Design_F << "\tinput " << clk << "," << rst << ",";
        else
            Design_F << "\tinput [" << (it->first) << ":0] ";
        for(int i=0;i!=(it->second).size();++i){
            if(((it->second).at(i) == (it->second).at((it->second).size()-1)) && (vers == 0)){
                Design_F << (it->second).at(i) << ";\n";
                break;
            }
            if(((it->second).at(i) == (it->second).at((it->second).size()-1)) && (vers == 1)){
                Design_F <<  (it->second).at(i) << ",\n";
                break;
            }
            Design_F << (it->second).at(i) << ",";
        }
    }
    /*for(it=inp.begin();it!=inp.end();++it){
        cout << it ->first << ":";
        for(int i=0;i!=inp.at(it->first).size();++i)
            cout << (it->second).at(i) << ",";
        cout << endl;
    }*/
}

void Design::outputs_print(){
    for(int i=0;i!=outputs_s.size();++i)
        out.insert(pair< int,vector<string> >(outputs_s.at(i),{}));
    for(it = out.begin();it!=out.end();++it)
        temp_out.push_back(it->first);
    //second for
    for(it = out.begin();it!=out.end();++it){
        for(int i=0;i!=outputs_s.size();++i){
            if((it->first) == outputs_s.at(i))
                (it->second).push_back(outputs.at(i));
        }
        if((it->first) == 1)
            Design_F << "\toutput reg ";
        else
            Design_F << "\toutput reg [" << (it->first) << ":0] ";
        for(int i=0;i!=(it->second).size();++i){
            if(((it->second).at(i) == (it->second).at((it->second).size()-1)) && (vers == 0)){
                Design_F << (it->second).at(i) << ";\n";
                break;
            }
            if(((it->second).at(i) == (it->second).at((it->second).size()-1)) && (vers == 1)){
                if(out.size() != 1 && (it->first)!=temp_out.at(temp_out.size()-1)){
                    Design_F << (it->second).at(i) << ",\n";
                    break;
                }else{
                    Design_F << (it->second).at(i) << ");\n";
                    break;
                }
            }
            Design_F << (it->second).at(i) << ",";
        }
    }
    Design_F << "\n";
}

void Design::States(){
    Design_F << "\treg [" << num_bits_states-1 << ":0]" << c_state << ";\n";
    Design_F << "\treg [" << num_bits_states-1 << ":0]" << n_state << ";\n\n";
    for(int i=0;i!=states.size();++i)
        Design_F << "\tparameter " << states.at(i) << " = " << i << ";\n";
    Design_F << "\n";
    Design_F << "\tinitial begin\n";
    Design_F << "\t\t" << c_state << " = " << initial_state << ";\n";
    Design_F << "\tend\n";
    Design_F << "\n";
}

void Design::StateReg(){
    Design_F << "\talways @(" << edge << " " << clk << ", " << edge << " " << rst << ")\n";
    Design_F << "\t\tbegin\n";
    Design_F << "\t\t\tif (";
    if (!active_rst)
        Design_F << "~";
    Design_F << rst << ")\n";
    Design_F << "\t\t\t\t" << c_state << " <= " << initial_state << ";\n";
    Design_F << "\t\t\telse\n";
    Design_F << "\t\t\t\t" << c_state << " <= " << n_state << ";\n";
    Design_F << "\t\tend\n";
    Design_F << "\n";
}

void Design::NextStateLogic(){
    Design_F << "//Next state logic\n";
    Design_F << "\talways @(" << c_state << "," << inputs_string << ")\n";
    Design_F << "\t\tbegin\n";
    Design_F << "\t\t\tcase(" << c_state << ")";
    int last_index = next_state[initial_state].size()-1;
    //cout <<last_index<<endl;
    map<string,vector<string>>::iterator ite;
    ite = next_state.begin();
    for(int i=0;i<states.size();i++){
        if(i<states.size()-1){Design_F << "\n\t\t\t\t"<<states[i]<<": ";}
        else Design_F<<"\n\t\t\t\tdefault: ";
        if(num_bits_inputs>1){
            Design_F << "\n\t\t\t\t\tcase({"<<inputs_string<<"})";
            set<string> seen,printed;
            vector<int> indexes;
            map<string,vector<int>> duplicates;
            string be="\'b";
            for(int j=0;j<ite->second.size();j++){
                if(seen.count(ite->second[j])==0){
                    seen.insert(ite->second[j]);}
                else{
                    for(int m=0;m<ite->second.size();m++){
                        if(ite->second[j]==ite->second[m]){
                            indexes.push_back(m);}}
                    duplicates.insert(pair<string,vector<int>>(ite->second[j],indexes));
                    indexes.clear();}
            }
                /* map<string,vector<int>>::iterator iterator;
                if(!duplicates.empty()){
                    cout << ite->first << endl;
                    for(iterator=duplicates.begin();iterator!=duplicates.end();++iterator){
                    cout << iterator ->first << ":";
                    for(int i=0;i!=duplicates.at(iterator->first).size();++i)
                        cout << (iterator->second).at(i) << ",";
                    cout << endl;
                    }
                } else{ cout << "mapa vacio" << endl;} */
            bool flag_default;
            for(int j=0;j<ite->second.size();j++){
                flag_default = false;
                if(duplicates.count(ite->second[j])==1 && printed.find(ite->second[j])==printed.end()){
                    printed.insert(ite->second[j]);
                    for(int n=0;n<duplicates[ite->second[j]].size();n++){
                        if(duplicates[ite->second[j]][n]==last_index){
                            flag_default = true;}}
                    for(int k=0;k<duplicates[ite->second[j]].size();k++){
                        if(flag_default){
                            Design_F<<"\n\t\t\t\t\t\tdefault: "+n_state+" = "+ite->second[j]+";"; break;}
                        else{
                            if(k<duplicates[ite->second[j]].size()-1){
                                Design_F<<"\n\t\t\t\t\t\t"+to_string(num_bits_inputs)
                                +be+input_values[duplicates[ite->second[j]][k]]+",\n";}
                            else{Design_F<<"\t\t\t\t\t\t"+to_string(num_bits_inputs)
                            +be+input_values[duplicates[ite->second[j]][k]]+": "+n_state+" = "+ite->second[j]+";";}
                        }
                    }
                }
                else if(duplicates.count(ite->second[j])==0){
                    if(j==last_index){
                        flag_default=true;}
                    if(flag_default){Design_F<<"\n\t\t\t\t\t\tdefault: "+n_state+" = "+ite->second[j]+";";}
                    else{Design_F<<"\n\t\t\t\t\t\t"+to_string(num_bits_inputs)+be+input_values[j]+": "+n_state
                    +" = "+ite->second[j]+";";}
                }
            }
            Design_F<<"\n\t\t\t\t\tendcase";
        }
        else{
            if(input_values[0]=="0"){
                if(ite->second[1]==ite->second[0]){Design_F<<"\n\t\t\t\t\t"+n_state+" = "+ite->second[1]+";";}
                else{
                    Design_F<<"\n\t\t\t\t\tif("+inputs_string+")";
                    Design_F<<"\n\t\t\t\t\t\t"+n_state+" = "+ite->second[1]+";";
                    Design_F<<"\n\t\t\t\t\telse";
                    Design_F<<"\n\t\t\t\t\t\t"+n_state+" = "+ite->second[0]+";";}}
            else{
                if(ite->second[1]==ite->second[0]){Design_F<<"\n\t\t\t\t\t"+n_state+" = "+ite->second[1]+";";}
                else{
                    Design_F<<"\n\t\t\t\t\tif("+inputs_string+")";
                    Design_F<<"\n\t\t\t\t\t\t"+n_state+" = "+ite->second[0]+";";
                    Design_F<<"\n\t\t\t\t\telse";
                    Design_F<<"\n\t\t\t\t\t\t"+n_state+" = "+ite->second[1]+";";}}
        }
        if(ite!=next_state.end()){++ite;}
    }
    Design_F << "\n\t\t\tendcase\n";
    Design_F << "\t\tend\n";
    Design_F << "\n";
}

void Design::OutputLogic(){
    Design_F << "//Output Logic\n";
    string be="\'b";
    if(num_bits_outputs>1){outputs_string="{"+outputs_string+"}";}
    if(fsm_type){//True -> Mealy; False -> Moore
        Design_F<<"\talways @ ("+c_state+","+inputs_string+")\n\t\t";
        Design_F<<"begin\n\t\t\tcase("+c_state+")";
        int last_index=output_state[initial_state].size()-1;
        map<string,vector<string>>::iterator ite;
        ite = output_state.begin();
        for(int i=0;i<states.size();i++){
            if(i<states.size()-1){Design_F<<"\n\t\t\t\t"<<states[i]<<": ";}
            else{Design_F<<"\n\t\t\t\tdefault: ";}
            if(num_bits_inputs>1){
                Design_F << "\n\t\t\t\t\tcase({"<<inputs_string<<"})\n";
                set<string> seen,printed;
                vector<int> indexes;
                map<string,vector<int>> duplicates;
                for(int j=0;j<ite->second.size();j++){
                    if(seen.count(ite->second[j])==0){
                        seen.insert(ite->second[j]);}
                    else{
                        for(int m=0;m<ite->second.size();m++){
                            if(ite->second[j]==ite->second[m]){
                                indexes.push_back(m);}}
                        duplicates.insert(pair<string,vector<int>>(ite->second[j],indexes));
                        indexes.clear();}
                }
                /* map<string,vector<int>>::iterator iterator;
                if(!duplicates.empty()){
                    cout << ite->first << endl;
                    for(iterator=duplicates.begin();iterator!=duplicates.end();++iterator){
                    cout << iterator ->first << ":";
                    for(int i=0;i!=duplicates.at(iterator->first).size();++i)
                        cout << (iterator->second).at(i) << ",";
                    cout << endl;
                    }
                } else{ cout << "mapa vacio" << endl;} */
                bool flag_default;
                for(int j=0;j<ite->second.size();j++){
                    flag_default = false;
                    if(duplicates.count(ite->second[j])==1 && printed.find(ite->second[j])==printed.end()){
                        printed.insert(ite->second[j]);
                        for(int n=0;n<duplicates[ite->second[j]].size();n++){
                            if(duplicates[ite->second[j]][n]==last_index){
                                flag_default = true;}}
                        for(int k=0;k<duplicates[ite->second[j]].size();k++){
                            if(flag_default){
                                Design_F<<"\t\t\t\t\t\tdefault: "<<outputs_string<<" = "
                                <<num_bits_outputs<<be<<ite->second[j]<<";"; break;}
                            else{
                                if(k<duplicates[ite->second[j]].size()-1){
                                    Design_F<<"\t\t\t\t\t\t"<<num_bits_inputs<<be
                                    <<input_values[duplicates[ite->second[j]][k]]<<",\n";}
                                else{Design_F<<"\t\t\t\t\t\t"<<num_bits_inputs<<be
                                <<input_values[duplicates[ite->second[j]][k]]<<": "<<outputs_string
                                <<" = "<<num_bits_outputs<<be<<ite->second[j]<<";";}}
                        }
                    }
                    else if(duplicates.count(ite->second[j])==0){
                        if(j==last_index){flag_default=true;}
                        if(flag_default){Design_F<<"\n\t\t\t\t\t\tdefault: "<<outputs_string
                        <<" = "<<num_bits_outputs<<be<<ite->second[j]<<";";}
                        else{Design_F<<"\n\t\t\t\t\t\t"<<num_bits_inputs<<be<<input_values[j]
                        <<": "<<outputs_string<<" = "<<num_bits_outputs<<be<<ite->second[j]<<";";}
                    }
                }
                Design_F<<"\n\t\t\t\t\tendcase";
            }
            else{
                if(input_values[0]=="0"){
                    if(ite->second[1]==ite->second[0]){Design_F<<"\n\t\t\t\t\t"<<outputs_string
                    <<" = "<<num_bits_outputs<<be<<ite->second[1]<<";";}
                    else{
                    Design_F<<"\n\t\t\t\t\tif("<<inputs_string<<")";
                    Design_F<<"\n\t\t\t\t\t\t"<<outputs_string<<" = "<<num_bits_outputs
                    <<be<<ite->second[1]<<";";
                    Design_F<<"\n\t\t\t\t\telse";
                    Design_F<<"\n\t\t\t\t\t\t"<<outputs_string<<" = "<<num_bits_outputs
                    <<be<<ite->second[0]<<";";}}
                else{
                    if(ite->second[1]==ite->second[0]){Design_F<<"\n\t\t\t\t\t"<<outputs_string
                    <<" = "<<num_bits_outputs<<be<<ite->second[1]<<";";}
                    else{
                    Design_F<<"\n\t\t\t\t\tif("<<inputs_string<<")";
                    Design_F<<"\n\t\t\t\t\t\t"<<outputs_string<<" = "<<num_bits_outputs
                    <<be<<ite->second[0]<<";";
                    Design_F<<"\n\t\t\t\t\telse";
                    Design_F<<"\n\t\t\t\t\t\t"<<outputs_string<<" = "<<num_bits_outputs
                    <<be<<ite->second[1]<<";";}}
            }
            if(ite!=output_state.end()){++ite;}
        }
        Design_F << "\n\t\t\tendcase\n";
        Design_F << "\t\tend\n";
    }
    else{
        Design_F<<"\n\n\talways @ ("<<c_state<<")\n\t\t";
        Design_F<<"begin\n\t\t\tcase("<<c_state<<")";
        map<string,vector<string>>::iterator ite;
        for(ite=output_state.begin();ite!=output_state.end();++ite){
            if(ite->first!=output_state.rbegin()->first){
                Design_F<<"\n\t\t\t\t"<<ite->first<<": "<<outputs_string<<" = "
                <<num_bits_outputs<<be<<ite->second[0]<<";";}
            else{Design_F<<"\n\t\t\t\tdefault: "<<outputs_string<<" = "<<num_bits_outputs
            <<be<<ite->second[0]<<";";}
        }
        Design_F<<"\n\t\t\tendcase\n\t\tend\n";
    }
    Design_F<<"endmodule";
    Design_F.close(); 
}