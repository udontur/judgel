#include "var.h"
#include "color.h"
#include <format>

void ClearCache(){
    system("rm -rf ~/.cache/judgel");
    system("mkdir -p ~/.cache/judgel");
}

string strip(string s){
    int n=s.size();
    string ans="";
    for(int i =9; i<n; i++) ans+=s[i];
    return ans;
}

void PrintHelpPage() {
    
    cout << "\nCommand: ";
    cout << ColorGreen;
    cout << "judgel ";
    cout << ColorYellow;
    cout << "<TIME_LIMIT> \n";
    cout << ColorReset;
    cout << ColorGray;
    cout << "Leave the time limit empty for a 1 second time limit\n\n";
    cout << ColorReset;
    cout << "Prerequisites\n";
    cout << "   - Testcase folder name: \033[34m\"testcase\"\033[0m\n";
    cout << "   - Code file name: \033[34m\"main.cpp\"\033[0m\n";
    cout << "   - Testcases format: \033[34mABC.in\033[0m and \033[34mABC.out\033[0m\n\n";
    cout << ColorGray;
    cout << "More: https://github.com/udontur/judgel\n";
    cout << ColorReset;
}

bool OutputComparer(path ctcout){
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
