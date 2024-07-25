/**
 * judgel - v1.0
 * Made with passion, by Hadrian (@udontur)
 * Source - github.com/udontur/judgel
 * MIT license - Copyright © 2024 Hadrian Lau (github.com/udontur)
**/

#include <Lmcons.h>
#include <shlobj.h>
#include <windows.h>
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

char my_documents[MAX_PATH];
HRESULT DocumentFolderPath = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

const path TemporaryDirectoryPath = DocumentFolderPath + "\\judgel-temp";
const path TemporaryOutputPath = TemporaryDirectoryPath.u8string() + "\\out.txt";
const path TemporaryUserProgramPath = TemporaryDirectoryPath.u8string() + "\\judgelprg.exe";
const int TerminalColumnWidth = 8;

int OutputCompare(path TestCaseOutput, path UserProgramOutput);
string MakeCompileCommand(path CurrentFilePath);
string MakeRunCommand(path CurrentFilePath);
void MakeNewFile(path CurrentFilePath);
string CurrentTestCaseCountOutputConverter(path CurrentTestCase);
void PrintHelpPage();

int main(int argc, char* argv[]) {

    if (argc != 4 && argc != 2) {
        if (argc != 1){
            cout << ColorRed;
            cout << "\nInvalid arguments\n";
            cout << ColorReset;
        }
        PrintHelpPage();
        _exit(0);
        return 0;
    }

    //The rte to tle problem checker
    if (exists(TemporaryDirectoryPath)) {
        try {
            remove_all(TemporaryDirectoryPath);
        }
        catch (filesystem_error) {
            cout << ColorYellow;
            cout << "\nJUDGEL: Please wait a while before submitting the next command\n\n";
            cout << ColorReset;
            _exit(0);
            return 0;
        }
    }

    create_directory(TemporaryDirectoryPath);

    int RuntimeTimeLimit;
    try {
        RuntimeTimeLimit = stoi(argv[1]);
    } catch (invalid_argument) {
        cout << ColorRed;
        cout << "\nInvalid time argument\n";
        cout << ColorReset;
        PrintHelpPage();
        _exit(0);
        return 0;
    }

    path TestCaseDirectoryPath, UserProgramCppPath;
    
    if (argc == 4) { 
        
        TestCaseDirectoryPath = argv[2];
        if (!exists(TestCaseDirectoryPath)) {
            cout << ColorRed;
            cout << "\nPath mode: Test Case path does not exist\n";
            cout << ColorReset;
            PrintHelpPage();
            _exit(0);
            return 0;
        }
        
        UserProgramCppPath = argv[3];
        if (!exists(UserProgramCppPath)) {
            cout << ColorRed;
            cout << "\nPath mode: Code file path does not exist\n";
            cout << ColorReset;
            PrintHelpPage();
            _exit(0);
            return 0;
        }
    } else { 
        
        TestCaseDirectoryPath = "testcase";
        if (!exists(TestCaseDirectoryPath)) {
            cout << ColorRed;
            cout << "\nCurrent Directory mode: Testcase folder is not named as \"testcase\"\n";
            cout << ColorReset;
            PrintHelpPage();
            _exit(0);
            return 0;
        }
        
        UserProgramCppPath = "main.cpp";
        if (!exists(UserProgramCppPath)) {
            cout << ColorRed;
            cout << "\nCurrent Directory mode: Code file is not named as \"main.cpp\"\n";
            cout << ColorReset;
            PrintHelpPage();
            _exit(0);
            return 0;
        }
    }
    
    string CompileCommand = MakeCompileCommand(UserProgramCppPath);
    
    system("CLS");
    cout << ColorGray;
    cout << "Compiling program...\n\n";
    cout << ColorReset;

    //Compiles the user program to judgelprg.exe (standard dir)
    //may output commands in user terminal
    system(CompileCommand.c_str());
    
    cout << "\n";
    
    if (!exists(TemporaryUserProgramPath)) {
        cout << ColorYellow;
        cout << "JUDGEL: Compilation Error\n\n";
        cout << ColorReset;

        remove_all(TemporaryDirectoryPath);
        _exit(0);
        return 0;
    }
    system("CLS");
    
    cout << ColorGray;
    cout << "\nTest    Verdict   Time\n";
    cout << ColorReset;
    int TestCaseCount = 0, MaximumRuntime = 0, FinalVerdict = -1, CurrentTestCaseRuntime;
    
    for (path CurrentTestCase : directory_iterator(TestCaseDirectoryPath)) {
        
        if (TestCaseCount % 2) { //.out file
            
            MaximumRuntime = max(MaximumRuntime, CurrentTestCaseRuntime);
            //Judger
            int CurrentVerdictResult = OutputCompare(CurrentTestCase, TemporaryOutputPath);
            FinalVerdict = max(FinalVerdict, CurrentVerdictResult);
            
            if (CurrentVerdictResult == 0) { 
                
                cout << left << setw(TerminalColumnWidth);
                cout << CurrentTestCaseCountOutputConverter(CurrentTestCase) + ":";
                cout << ColorYellow;
                cout << "  RTE  ";
                cout << ColorReset;
                cout << "   --ms\n";
                cout << ColorGray;
                cout << "---------ABORT--------\n\n";
                cout << ColorReset;
                cout << ColorYellow ;
                cout << "JUDGEL: Runtime Error\n\n";
                cout << ColorReset;
                
                remove_all(TemporaryDirectoryPath);
                
                _exit(0);
                return 0;
            } else if (CurrentVerdictResult == 2) { //.in file
                
                cout << left << setw(TerminalColumnWidth);
                cout << CurrentTestCaseCountOutputConverter(CurrentTestCase) + ":";
                cout << ColorRed;
                cout << "  WA    ";
                cout << ColorReset;
                cout << right << setw(TerminalColumnWidth - 1);
                cout << to_string(CurrentTestCaseRuntime) + "ms\n";
            } else if (CurrentVerdictResult == 1) {
                
                cout << left << setw(TerminalColumnWidth);
                cout << CurrentTestCaseCountOutputConverter(CurrentTestCase) + ":";
                cout << ColorGreen;
                cout << "  AC    ";
                cout << ColorReset;
                cout << right << setw(TerminalColumnWidth - 1);
                cout << to_string(CurrentTestCaseRuntime) + "ms\n";
            }
            
            remove(TemporaryOutputPath);
            MakeNewFile(TemporaryOutputPath);
        } else { 
            
            string RunCommand = MakeRunCommand(CurrentTestCase);
            
            time_point<high_resolution_clock> UserProgramStart;
            time_point<high_resolution_clock> UserProgramStop;
            //Lambda function to run the async process
            auto RunSystem = [&UserProgramStart, &RunCommand, &UserProgramStop]() {
                UserProgramStart = high_resolution_clock::now();
                system(RunCommand.c_str());
                UserProgramStop = high_resolution_clock::now();
            };
            //Run the judgelprg.exe with .in testcase asyncly
            auto UserProgramFuture = async(launch::async, RunSystem);
            //Listens for time out
            if (UserProgramFuture.wait_for(seconds(RuntimeTimeLimit)) == future_status::timeout) {
                bool rte = 0;
                //If the program wants to rte, but the time limit stops, you can't terminate the process
                try {
                    system("Taskkill /IM \"judgelprg.exe\" /F > nul 2>&1");
                    remove_all(TemporaryDirectoryPath);
                } catch (system_error) {
                    rte = 1;
                }
                if (rte) {
                    cout << left << setw(TerminalColumnWidth);
                    cout << CurrentTestCaseCountOutputConverter(CurrentTestCase) + ":";
                    cout << ColorYellow;
                    cout << "  RTE  ";
                    cout << ColorReset;
                    cout << "   --ms\n";
                    cout << ColorGray;
                    cout << "---------ABORT--------\n\n";
                    cout << ColorReset;
                    cout << ColorYellow;
                    cout << "JUDGEL: Runtime Error\n\n";
                    cout << ColorReset;
                    cout << ColorYellow;
                    cout << "JUDGEL: Please wait a while before submitting the next command\n\n";
                    cout << ColorReset;
                } else {
                    cout << left << setw(TerminalColumnWidth);
                    cout << CurrentTestCaseCountOutputConverter(CurrentTestCase) + ":";
                    cout << ColorYellow;
                    cout << "  TLE  ";
                    cout << ColorReset;
                    cout << right << setw(TerminalColumnWidth - 1);
                    cout << ">" + to_string(RuntimeTimeLimit * 1000) + "ms\n";
                    cout << ColorGray;
                    cout << "---------ABORT--------\n\n";
                    cout << ColorReset;
                    cout << ColorYellow;
                    cout << "JUDGEL: Time Limit Exceed\n\n";
                    cout << ColorReset;
                }

                _exit(0);
                return 0;
            }
            //Feed duration to the next cycle
            const auto UserProgramThreadRuntime = UserProgramStop - UserProgramStart;
            CurrentTestCaseRuntime = duration_cast<milliseconds>(UserProgramThreadRuntime).count();
        }
        
        TestCaseCount++;
    }
    cout << "\n";
    
    if (FinalVerdict == -1) {
        cout << ColorRed;
        cout << "There are no testcases\n\n";
        cout << ColorReset;
    } else if (FinalVerdict == 1) {
        
        cout << ColorGreen;
        cout << "JUDGEL: Accepted ";
        cout << MaximumRuntime << "ms\n\n";
        cout << ColorReset;
    } else if (FinalVerdict == 2) {
        
        cout << ColorRed;
        cout << "JUDGEL: Wrong Answer ";
        cout << MaximumRuntime << "ms\n\n";
        cout << ColorReset;
    } else {
        
        cout << ColorRed;
        cout << "JUDGEL: A bug has been detected, please report it via github.com/udontur/judgel/issues. Thank you.\n\n";
        cout << ColorReset;
    }
    
    remove_all(TemporaryDirectoryPath);
    
    _exit(0);
    return 0;
}

int OutputCompare(path TestCaseOutput, path UserProgramOutput) {
    
    if (file_size(UserProgramOutput) == 0) return 0;
    
    ifstream TestCaseFout(TestCaseOutput);
    ifstream UserProgramFout(UserProgramOutput);
    //Judge both files for accuracy
    while (!TestCaseFout.eof()) {
        string CurrentStringTestCase, CurrentStringUserProgram;
        TestCaseFout >> CurrentStringTestCase;
        UserProgramFout >> CurrentStringUserProgram;
        
        if (CurrentStringUserProgram != CurrentStringTestCase) return 2;
    }
    
    while (!UserProgramFout.eof()) {
        string CurrentStringTestCase, CurrentStringUserProgram;
        TestCaseFout >> CurrentStringTestCase;
        UserProgramFout >> CurrentStringUserProgram;
        
        if (CurrentStringUserProgram != CurrentStringTestCase) return 2;
    }
    
    return 1;
}

string MakeCompileCommand(path CurrentFilePath) {
    string compileCmd = "g++ ";
    compileCmd += CurrentFilePath.u8string();
    compileCmd += " -o ";
    compileCmd += TemporaryUserProgramPath.u8string();
    return compileCmd;
}

string MakeRunCommand(path CurrentFilePath) {
    string run = TemporaryUserProgramPath.u8string();
    run += " < ";
    run += CurrentFilePath.u8string();
    run += " > ";
    run += TemporaryOutputPath.u8string();
    return run;
}

void MakeNewFile(path CurrentFilePath) {
    ifstream tmp;
    tmp.open(CurrentFilePath);
}

string CurrentTestCaseCountOutputConverter(path TestCasePath) {
    string TestCaseOutput = "";
    TestCaseOutput += TestCasePath.stem().u8string();
    return TestCaseOutput;
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

/**
 * judgel - v1.0
 * Made with passion, by Hadrian (@udontur)
 * Source - github.com/udontur/judgel
 * MIT license - Copyright © 2024 Hadrian Lau (github.com/udontur)
**/