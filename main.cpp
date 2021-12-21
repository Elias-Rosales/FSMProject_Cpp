/*
Created on November, 2021
Authors:    Reyes Sánchez Luis Angel    
            Rivera Orozco David
            Rosales Galindo Elias
            Tacuapan Moctezuma Edgar
*/

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <vector>
#include "WriteDataTB.h"

using namespace std;

int main(int argc, char *argv[]){
    auto start = chrono::high_resolution_clock::now();
    int len_argv = argc;
    vector<char*> args(argv,argv+argc+1);
    //HEADER PROGRAM
    cout << "*****************************************" << endl;
    cout << "\nFSM DESIGNER\n";
    cout << "Created on November 2021" << endl;
    cout << "Authors:" << endl;
    cout << "\tReyes Sanchez Luis Angel" << endl;
    cout << "\tRivera Orozco David" << endl;
    cout << "\tRosales Galindo Elias" << endl;
    cout << "\tTacuapan Moctezuma Edgar\n" << endl;
    cout << "*****************************************" << endl;
    //-----------------------------------------------------------
    //-----------------------------------------------------------
    //START
    WriteDataTB object(len_argv,args);
    /*testbench*/
    //END
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    system("pause");
    cout <<"\nExecution Time: " << duration.count() << "msec" << endl;
    cout << "*****\n";
    //system("cat FSM.sv");
    //cout << "*****";
    return EXIT_SUCCESS;
}