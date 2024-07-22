#include <iostream>
#include <chrono>
#include <filesystem>
#include <fstream>
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
bool OutputCompare(path TestCaseOutput, path UserProgramOutput){
    ifstream TestCaseFout(TestCaseOutput);
    ifstream UserProgramFout(UserProgramOutput);
    while(!TestCaseFout.eof()){
        string CurrentStringTestCase, CurrentStringUserProgram;
        TestCaseFout>>CurrentStringTestCase;
        UserProgramFout>>CurrentStringUserProgram;
        if(CurrentStringUserProgram!=CurrentStringTestCase) return 0;
    }
    while(!UserProgramFout.eof()){
        string CurrentStringTestCase, CurrentStringUserProgram;
        TestCaseFout>>CurrentStringTestCase;
        UserProgramFout>>CurrentStringUserProgram;
        if(CurrentStringUserProgram!=CurrentStringTestCase) return 0;
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
string CurrentTestCaseCountOutputConverter(int TestCaseCount){
    string TestCaseCountOutput="";
    if(TestCaseCount<10) TestCaseCountOutput+="00";
    else if(TestCaseCount) TestCaseCountOutput+="0";
    TestCaseCountOutput+=to_string(TestCaseCount);
    return TestCaseCountOutput;
}
string CurrentRuntimeOutputConverter(int Runtime){
    string CurrentRuntimeOutput="";
    if(Runtime<10) CurrentRuntimeOutput+="  ";
    else if(Runtime<100) CurrentRuntimeOutput+=" ";
    CurrentRuntimeOutput+=to_string(Runtime);
    return CurrentRuntimeOutput;
}
int main(int argc, char *argv[]){
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
    system(CompileCommand.c_str());
    cout<<"Test  Verdict  Time\n";
    int TestCaseCount=0, MaximumRuntime=0, FinalVerdict=0, CurrentTestCaseRuntime;
    for(path CurrentTestCase: directory_iterator(TestCaseDirectoryPath)){
        if(TestCaseCount%2){
            MaximumRuntime=max(MaximumRuntime, CurrentTestCaseRuntime);
            if(CurrentTestCaseRuntime>=1000){
                FinalVerdict=max(FinalVerdict, 2);
                cout<<CurrentTestCaseCountOutputConverter(TestCaseCount/2);
                cout<<":    "<<YELLOW<<"TLE"<<RESET<<"     ";
                cout<<CurrentRuntimeOutputConverter(CurrentTestCaseRuntime)<<"ms\n";
                cout<<RED<<"ABORT: "<<RESET<<YELLOW<<"Time Limit Exceed "<<RESET;
                cout<<RED<<MaximumRuntime<<"ms"<<RESET<<"\n\n";
                return 0;
            }else if(OutputCompare(CurrentTestCase, TemporaryOutputPath)){
                FinalVerdict=max(FinalVerdict, 0);
                cout<<CurrentTestCaseCountOutputConverter(TestCaseCount/2);
                cout<<":    "<<GREEN<<"AC"<<RESET<<"     ";
                cout<<CurrentRuntimeOutputConverter(CurrentTestCaseRuntime)<<"ms\n";
            }else{
                FinalVerdict=max(FinalVerdict, 1);
                cout<<CurrentTestCaseCountOutputConverter(TestCaseCount/2);
                cout<<":    "<<RED<<"WA"<<RESET<<"     ";
                cout<<CurrentRuntimeOutputConverter(CurrentTestCaseRuntime)<<"ms\n";
            }
            remove(TemporaryOutputPath);
            MakeNewFile(TemporaryOutputPath);
        }else{
            string RunCommand=MakeRunCommand(CurrentTestCase);
            //cout<<"Program run command: "<<run<<"\n";
            const auto UserProgramRuntimeStart=high_resolution_clock::now();
            system(RunCommand.c_str());
            const auto UserProgramRuntimeStop=high_resolution_clock::now();
            const auto UserProgramRuntime=UserProgramRuntimeStop-UserProgramRuntimeStart;
            CurrentTestCaseRuntime=duration_cast<milliseconds>(UserProgramRuntime).count();
        }
        TestCaseCount++;
    }
    remove_all(TemporaryDirectoryPath);
    cout<<"\n";
    if(FinalVerdict==0){
        cout<<GREEN;
        cout<<"JUDGE: Accepted ";
        cout<<MaximumRuntime<<"ms\n";
        cout<<RESET;
    }else if(FinalVerdict==1){
        cout<<RED;
        cout<<"JUDGE: Wrong Answer ";
        cout<<MaximumRuntime<<"ms\n";
        cout<<RESET;
    }
    cout<<"\n";
    return 0;
}