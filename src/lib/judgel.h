#include "var.h"
#include "color.h"

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

bool isac(path ctcout){
    ifstream tin (ctcout);
    ifstream uin (usrout);
    string t, u;
    while(!tin.eof()){
        uin>>u;
        tin>>t;
        if(t!=u) return 0;
    }
    return 1;
}

