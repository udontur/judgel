
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace std::filesystem; 

#define ColorReset "\033[0m"
#define ColorInv "\033[30m"
#define ColorRed "\033[31m"
#define ColorGreen "\033[32m"
#define ColorYellow "\033[33m"
#define ColorBlue "\033[34m"
#define ColorGray "\033[90m"
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
int main(int argc, char* argv[]){
    if(argc!=2){
        cout<<"Error: Invalid argument\n";
        PrintHelpPage();
        return 0;
    }
    string times=argv[1];
    for(int i =0; i<times.size(); i++){
        if(argv[1][i]<='0'||argv[1][i]>='9'){
            cout<<"Error: Time limit is not an integer\n";
            PrintHelpPage();
            return 0;
        }
    }
    int RunTimeLimit=stoi(argv[1]);
    if(!exists("~/.cache/judgel"))
        system("mkdir ~/.cache/judgel");

    if(!exists("testcase")){
        cout<<"Error: \"testcase\" not found\n";
        PrintHelpPage();
        return 0;
    }
    if(!exists("main.cpp")){
        cout<<"Error: \"main.cpp\" code file not found\n";
        PrintHelpPage();
        return 0;
    }
    system("clear");
    cout << ColorGray;
    cout << "Compiling program...\n\n";
    cout << ColorReset;
    system("g++ main.cpp -o ~/.cache/judgel/usr.out");
    cout << ColorRed;
    cout << "\nTest    Verdict   Time\n";
    cout << ColorReset;
    int incr=0;
    for(path CurrentTestCase: directory_iterator("testcase")){
    
        if(incr%2){
            cout<<"WA/AC/RTE\n";
        }else{
            system("touch ~/.cache/judgel/out.txt");
            time_point<high_resolution_clock> UserProgramStart;
            time_point<high_resolution_clock> UserProgramStop;
            string RunCommand="~/.cache/judgel/usr.out < ";
            RunCommand+=CurrentTestCase.u8string();
            RunCommand+=" > ~/.cache/judgel/out.txt";
            //Lambda function to run the async process
            auto RunSystem = [&UserProgramStart, &CurrentTestCase, &RunCommand, &UserProgramStop]() {
                UserProgramStart = high_resolution_clock::now();
                system(RunCommand.c_str());
                UserProgramStop = high_resolution_clock::now();
            };
            //Run the judgelprg.exe with .in testcase asyncly
            auto UserProgramFuture = async(launch::async, RunSystem);
            if(UserProgramFuture.wait_for(seconds(RunTimeLimit)) == future_status::timeout){
                cout<<"TLE\n";
                system("pkill -9 -f ~/.cache/judgel/usr.out");
            }
        }
        incr++;
    }

}