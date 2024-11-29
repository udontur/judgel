
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include "lib/judgel.h"
#include "lib/color.h"
#include "lib/var.h"

using namespace std;
using namespace std::chrono;
using namespace std::filesystem; 

int main(int argc, char* argv[]){
    
    if(argc!=2){
        cout<<"Error: Invalid argument\n";
        ClearCache();
        PrintHelpPage();
        return 0;
    }
    string times=argv[1];
    for(int i =0; i<times.size(); i++){
        if(argv[1][i]<='0'||argv[1][i]>='9'){
            cout<<"ERROR: Time limit is not an integer\n";
            ClearCache();
            PrintHelpPage();
            return 0;
        }
    }
    int RunTimeLimit=stoi(argv[1]);
    ClearCache();
    if(!exists("testcase")){
        cout<<"ERROR: \"testcase\" not found\n";
        ClearCache();
        PrintHelpPage();
        return 0;
    }
    if(!exists("main.cpp")){
        cout<<"ERROR: \"main.cpp\" not found\n";
        ClearCache();
        PrintHelpPage();
        return 0;
    }
    system("clear");
    cout << ColorGray;
    cout << "Compiling program...\n\n";
    cout << ColorReset;

    string usrpg=homefolder+"/.cache/judgel/usr.out";

    cout<<"*Compile: [";
    string runcmd="g++ main.cpp -o "+static_cast<string>(usrpg);
    system(runcmd.c_str());
    cout<<"]\n";

    /*cout<<"*ls: [";
    string checker="ls $HOME/.cache/judgel";
    system(checker.c_str());
    cout<<"]\n";

    cout<<"*Does usr.out exist: ";
    system("ls -l ~/.cache/judgel/usr.out");
    //cout<<exists(usrpg)<<"\n";

    return 0;*/

    system("test -f $HOME/.cache/judgel/usr.out && echo $?");
    if(!exists(usrpg)){
        //cout<<"CHECK\n";
        //WHY IT DOES NOT WORK
        cout<<"Detected not here!\n";
        ClearCache();
        return 0;
    }
    
    cout << ColorRed;
    cout << "\nTest    Verdict   Time\n";
    cout << ColorReset;
    int incr=0;
    vector<path> tculp;
    for(path CurrentTestCase: recursive_directory_iterator("testcase")){
        tculp.push_back(CurrentTestCase);
    }
    sort(tculp.begin(), tculp.end());
    for(path CurrentTestCase: tculp){
        //To Do: Check runtime error status
        if(incr%2){
            bool ac=isac(CurrentTestCase);
            if(ac){
                cout<<"AC\n";
            }else{
                cout<<"WA\n";
            }
            
        }else{
            if(exists("rm $HOME/.cache/judgel/out.txt")) system("rm $HOME/.cache/judgel/out.txt");
            
            time_point<high_resolution_clock> UserProgramStart;
            time_point<high_resolution_clock> UserProgramStop;
            string RunCommand="~/.cache/judgel/usr.out < ";
            RunCommand+=CurrentTestCase.u8string();
            RunCommand+=" > ~/.cache/judgel/out.txt";
            cout<<RunCommand<<"\n";
            //cout<<CurrentTestCase.u8string()<<":P:\n";
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
            system("cat ~/.cache/judgel/out.txt");
        }
        incr++;
    }
    ClearCache();
    return 0;
}