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
int main(int argc, char *argv[]){
    system ("CLS");
    remove_all(TemporaryDirectoryPath);
    cout<<"\n";
    if(argc==1){
        cout<<"Judgel - Simple local Competitive programming Judge\n";
        return 0;
    }
    if(argc!=3){
        cout<<"Invalid Argument\n";
        return 0;
    }
    MakeNewDirectory(TemporaryDirectoryPath);
    path TestCaseDirectoryPath=argv[1];
    path UserProgramCppPath=argv[2];
    //cout<<"Test Case path: "<<TestCaseDirectoryPath<<"\n";
    //cout<<"User Program path: "<<UserProgramCppPath<<"\n";
    string CompileCommand=MakeCompileCommand(UserProgramCppPath);
    //cout<<"Compile command: "<<compileCmd<<"\n";
    cout<<"Compiling program...\n\n";
    system(CompileCommand.c_str());
    if(!exists(TemporaryUserProgramPath)){
        cout<<YELLOW<<"\nABORT: Compilation Error\n\n"<<RESET;
        remove_all(TemporaryDirectoryPath);
        _exit(0);
        return 0;
    }
    cout<<left<<setw(TerminalColumnWidth)<<"Test";
    cout<<setw(TerminalColumnWidth)<<"Verdict";
    cout<<right<<setw(TerminalColumnWidth)<<"Time\n";
    int TestCaseCount=0, MaximumRuntime=0, FinalVerdict=0, CurrentTestCaseRuntime;
    for(path CurrentTestCase: directory_iterator(TestCaseDirectoryPath)){
        if(TestCaseCount%2){
            MaximumRuntime=max(MaximumRuntime, CurrentTestCaseRuntime);
            int CurrentVerdictResult=OutputCompare(CurrentTestCase, TemporaryOutputPath);
            if(CurrentVerdictResult==0){
                FinalVerdict=max(FinalVerdict, 0);
                cout<<left<<setw(TerminalColumnWidth)<<CurrentTestCaseCountOutputConverter(CurrentTestCase)+":";
                cout<<setw(TerminalColumnWidth)<<YELLOW<<"RTE"<<RESET;
                cout<<right<<setw(TerminalColumnWidth)<<"--ms\n\n";
                cout<<YELLOW<<"ABORT: Runtime Error\n\n"<<RESET;
                remove_all(TemporaryDirectoryPath);
                _exit(0);
                return 0;
            }else if(CurrentVerdictResult==2){
                FinalVerdict=max(FinalVerdict, 2);
                cout<<left<<setw(TerminalColumnWidth)<<CurrentTestCaseCountOutputConverter(CurrentTestCase)+":";
                cout<<setw(TerminalColumnWidth)<<RED<<"WA"<<RESET;
                cout<<right<<setw(TerminalColumnWidth)<<CurrentRuntimeOutputConverter(CurrentTestCaseRuntime)+"ms\n";
            }else if(CurrentVerdictResult==1){
                FinalVerdict=max(FinalVerdict, 1);
                cout<<left<<setw(TerminalColumnWidth)<<CurrentTestCaseCountOutputConverter(CurrentTestCase)+":";
                cout<<setw(TerminalColumnWidth)<<GREEN<<"AC"<<RESET;
                cout<<right<<setw(TerminalColumnWidth)<<CurrentRuntimeOutputConverter(CurrentTestCaseRuntime)+"ms\n";
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
                cout<<setw(TerminalColumnWidth)<<YELLOW<<"TLE"<<RESET;
                cout<<right<<setw(TerminalColumnWidth)<<CurrentRuntimeOutputConverter(CurrentTestCaseRuntime)+"2000ms+\n\n";
                cout<<YELLOW<<"ABORT: Time Limit Exceed\n\n"<<RESET;
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
        cout<<"JUDGE: Wrong Answer ";
        cout<<MaximumRuntime<<"ms\n";
        cout<<RESET;
    }else if(FinalVerdict==1){
        cout<<GREEN;
        cout<<"JUDGE: Accepted ";
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