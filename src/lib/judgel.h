#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>
#include "color.h"

using namespace std;
using namespace std::chrono;
using namespace std::filesystem; 

void ClearCache(){
    system("rm -rf ~/.cache/judgel");
    system("mkdir -p ~/.cache/judgel");
}

void PrintHelpPage() {
    
    cout << ColorGray;
    cout << "\nUsage: ";
    cout << ColorReset;
    cout << ColorGreen;
    cout << "judgel ";
    cout << ColorYellow;
    cout << "<TIME_LIMIT(s, int)> \n";
    cout << ColorGray;
    cout << "More on github.com/udontur/judgel#usage\n\n";
    cout << ColorReset;
    
    cout << ColorGreen;
    cout << "Judgel";
    cout << ColorReset;
    cout << " - Simple local C++ judge\n";
    cout << "Made with ";
    cout << ColorGreen;
    cout << "passion";
    cout << ColorReset;
    cout << ", by ";
    cout << ColorGreen;
    cout << "Hadrian (@udontur)\n";
    cout << ColorReset;
    cout << ColorGray;
    cout << "Source: github.com/udontur/judgel\n";
    cout << "MIT license - Copyright (c) 2024 Hadrian Lau (github.com/udontur)\n\n";
    cout  << ColorReset;
}

bool isac(path CurrentTestCase){
    ifstream tin (CurrentTestCase);
    ifstream uin ("~/.cache/judgel/out.txt");
    string t, u;
    system("cat ~/.cache/judgel/out.txt");
    cout<<"\n";
    while(uin>>u){
        tin>>t;
        cout<<t<<" | "<<u<<"\n";
        //if(t!=u) return 0;
    }
    while(tin>>t){
        uin>>u;
        cout<<t<<" || "<<u<<"\n";
        //if(t!=u) return 0;
    }
    return 1;
}

