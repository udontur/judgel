/**
 * judgel - v1.0
 * Made with passion, by Hadrian (@udontur)
 * Source - github.com/udontur/judgel
 * MIT license - Copyright © 2024 Hadrian Lau (github.com/udontur)
**/
#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include <Lmcons.h>
#include <shlobj.h>
using namespace std; 
using namespace std::chrono;
using namespace std::filesystem;
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE    "\033[34m"
#define GREY   "\033[90m"
CHAR my_documents[MAX_PATH];
HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
const path TemporaryDirectoryPath=result+"\\judgel-temp";
const path TemporaryOutputPath=TemporaryDirectoryPath.u8string()+"\\out.txt";
const path TemporaryUserProgramPath=TemporaryDirectoryPath.u8string()+"\\prg.exe";
const int TerminalColumnWidth=8;
int OutputCompare(path TestCaseOutput, path UserProgramOutput);
void MakeNewDirectory(path Directory);
string MakeCompileCommand(path CurrentFilePath);
string MakeRunCommand(path CurrentFilePath);
void MakeNewFile(path CurrentFilePath);
string CurrentTestCaseCountOutputConverter(path CurrentTestCase);
string CurrentRuntimeOutputConverter(int Runtime);
void PrintHelpPage();
int main(int argc, char *argv[]){
    remove_all(TemporaryDirectoryPath);
    if(argc==1){
        PrintHelpPage();
        return 0;
    }
    if(argc!=3){
        cout<<RED<<"\nInvalid Argument\n"<<RESET;
        PrintHelpPage();
        return 0;
    }
    MakeNewDirectory(TemporaryDirectoryPath);
    path TestCaseDirectoryPath=argv[1];
    path UserProgramCppPath=argv[2];
    //cout<<"Test Case path: "<<TestCaseDirectoryPath<<"\n";
    //cout<<"User Program path: "<<UserProgramCppPath<<"\n";
    string CompileCommand=MakeCompileCommand(UserProgramCppPath);
    //cout<<"Compile command: "<<compileCmd<<"\n";
    system ("CLS");
    cout<<"Compiling program...\n\n";
    system(CompileCommand.c_str());
    if(!exists(TemporaryUserProgramPath)){
        cout<<YELLOW<<"\nJUDGEL (ABORT): Compilation Error\n\n"<<RESET;
        remove_all(TemporaryDirectoryPath);
        _exit(0);
        return 0;
    }
    cout<<"Test    Verdict   Time\n";
    int TestCaseCount=0, MaximumRuntime=0, FinalVerdict=0, CurrentTestCaseRuntime;
    for(path CurrentTestCase: directory_iterator(TestCaseDirectoryPath)){
        if(TestCaseCount%2){
            MaximumRuntime=max(MaximumRuntime, CurrentTestCaseRuntime);
            int CurrentVerdictResult=OutputCompare(CurrentTestCase, TemporaryOutputPath);
            if(CurrentVerdictResult==0){
                FinalVerdict=max(FinalVerdict, 0);
                cout<<left<<setw(TerminalColumnWidth)<<CurrentTestCaseCountOutputConverter(CurrentTestCase)+":";
                cout<<YELLOW<<"  RTE  "<<RESET;
                cout<<right<<setw(TerminalColumnWidth-1)<<"--ms\n\n";
                cout<<YELLOW<<"JUDGEL (ABORT): Runtime Error\n\n"<<RESET;
                remove_all(TemporaryDirectoryPath);
                _exit(0);
                return 0;
            }else if(CurrentVerdictResult==2){
                FinalVerdict=max(FinalVerdict, 2);
                cout<<left<<setw(TerminalColumnWidth)<<CurrentTestCaseCountOutputConverter(CurrentTestCase)+":";
                cout<<RED<<"  WA    "<<RESET;
                cout<<right<<setw(TerminalColumnWidth-1)<<to_string(CurrentTestCaseRuntime)+"ms\n";
            }else if(CurrentVerdictResult==1){
                FinalVerdict=max(FinalVerdict, 1);
                cout<<left<<setw(TerminalColumnWidth)<<CurrentTestCaseCountOutputConverter(CurrentTestCase)+":";
                cout<<GREEN<<"  AC    "<<RESET;
                cout<<right<<setw(TerminalColumnWidth-1)<<to_string(CurrentTestCaseRuntime)+"ms\n";
            }
            remove(TemporaryOutputPath);
            MakeNewFile(TemporaryOutputPath);
        }else{
            string RunCommand=MakeRunCommand(CurrentTestCase);
            //cout<<"Program run command: "<<run<<"\n";
            time_point<high_resolution_clock> UserProgramThreadStart;
            time_point<high_resolution_clock> UserProgramThreadStop;
            auto RunSystem=[&UserProgramThreadStart, &RunCommand, &UserProgramThreadStop](){
                UserProgramThreadStart=high_resolution_clock::now();
                system(RunCommand.c_str());
                UserProgramThreadStop=high_resolution_clock::now();
            };
            auto UserProgramFuture=async(launch::async, RunSystem);
            if(UserProgramFuture.wait_for(seconds(2))==future_status::timeout){
                cout<<left<<setw(TerminalColumnWidth)<<CurrentTestCaseCountOutputConverter(CurrentTestCase)+":";
                cout<<YELLOW<<"  TLE  "<<RESET;
                cout<<right<<setw(TerminalColumnWidth-1)<<">2000ms\n\n";
                cout<<YELLOW<<"JUDGEL (ABORT): Time Limit Exceed\n\n"<<RESET;
                //cannot remove
                _exit(0);
                return 0;
            }
            const auto UserProgramThreadRuntime=UserProgramThreadStop-UserProgramThreadStart;
            CurrentTestCaseRuntime=duration_cast<milliseconds>(UserProgramThreadRuntime).count();
        }
        TestCaseCount++;
    }
    remove_all(TemporaryDirectoryPath);
    cout<<"\n";
    if(FinalVerdict==2){
        cout<<RED;
        cout<<"JUDGEL: Wrong Answer ";
        cout<<MaximumRuntime<<"ms\n";
        cout<<RESET;
    }else if(FinalVerdict==1){
        cout<<GREEN;
        cout<<"JUDGEL: Accepted ";
        cout<<MaximumRuntime<<"ms\n";
        cout<<RESET;
    }
    cout<<"\n";
    _exit(0);
    return 0;
}
int OutputCompare(path TestCaseOutput, path UserProgramOutput){
    if(file_size(UserProgramOutput)==0) return 0;
    ifstream TestCaseFout(TestCaseOutput);
    ifstream UserProgramFout(UserProgramOutput);
    while(!TestCaseFout.eof()){
        string CurrentStringTestCase, CurrentStringUserProgram;
        TestCaseFout>>CurrentStringTestCase;
        UserProgramFout>>CurrentStringUserProgram;
        if(CurrentStringUserProgram!=CurrentStringTestCase) return 2;
    }
    while(!UserProgramFout.eof()){
        string CurrentStringTestCase, CurrentStringUserProgram;
        TestCaseFout>>CurrentStringTestCase;
        UserProgramFout>>CurrentStringUserProgram;
        if(CurrentStringUserProgram!=CurrentStringTestCase) return 2;
    }
    return 1;
}
void MakeNewDirectory(path Directory){
    ifstream tmp;
    create_directory(Directory);
}
string MakeCompileCommand(path CurrentFilePath){
    //cout<<"MakeRunCommand: "<<CurrentFilePath.u8string()<<" "<<TemporaryUserProgramPath.u8string()<<"\n";
    string compileCmd="g++ ";
    compileCmd+=CurrentFilePath.u8string();
    compileCmd+=" -o ";
    compileCmd+=TemporaryUserProgramPath.u8string();
    return compileCmd;
}
string MakeRunCommand(path CurrentFilePath){
    //cout<<"MakeRunCommand: "<<CurrentFilePath.u8string()<<" "<<TemporaryOutputPath.u8string()<<"\n";
    string run=TemporaryUserProgramPath.u8string();
    run+=" < ";
    run+=CurrentFilePath.u8string();
    run+=" > ";
    run+=TemporaryOutputPath.u8string();
    return run;
}
void MakeNewFile(path CurrentFilePath){
    ifstream tmp;
    tmp.open(CurrentFilePath);
}
string CurrentTestCaseCountOutputConverter(path TestCasePath){
    string TestCaseOutput="";
    TestCaseOutput+=TestCasePath.stem().u8string();
    return TestCaseOutput;
}
string CurrentRuntimeOutputConverter(int Runtime){
    string CurrentRuntimeOutput="";
    if(Runtime<10) CurrentRuntimeOutput+="  ";
    else if(Runtime<100) CurrentRuntimeOutput+=" ";
    CurrentRuntimeOutput+=to_string(Runtime);
    return CurrentRuntimeOutput;
}
void PrintHelpPage(){
    cout<<GREY<<"\nUsage: "<<RESET;
    cout<<GREEN<<"judgel ";
    cout<<YELLOW<<"<TIME_LIMIT> ";
    cout<<BLUE<<"<TESTCASE/PATH> <CPPFILE_PATH>\n\n";
    cout<<RESET;
    cout<<GREEN<<"Judgel"<<RESET;
    cout<<" - Simple local C++ judge\n";
    cout<<"Made with ";
    cout<<GREEN<<"passion"<<RESET;
    cout<<", by ";
    cout<<GREEN<<"Hadrian (@udontur)\n"<<RESET;
    cout<<GREY<<"Source: github.com/udontur/judgel\n";
    cout<<"MIT license - Copyright (c) 2024 Hadrian Lau (github.com/udontur)\n\n"<<RESET;
}