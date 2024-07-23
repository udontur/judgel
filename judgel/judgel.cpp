/**
 * judgel - v1.0
 * Made with passion, by Hadrian (@udontur)
 * Source - github.com/udontur/judgel
 * MIT license - Copyright © 2024 Hadrian Lau (github.com/udontur)
 **/
//API header file
#include <Lmcons.h>
#include <shlobj.h>
#include <windows.h>
//Standard library
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>
//Namespace shortcut
using namespace std;
using namespace std::chrono;
using namespace std::filesystem;
//Terminal color modifier
#define ColorReset "\033[0m"
#define ColorRed "\033[31m"
#define ColorGreen "\033[32m"
#define ColorYellow "\033[33m"
#define ColorBlue "\033[34m"
#define ColorGray "\033[90m"
//Declare standard paths
char my_documents[MAX_PATH];
HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
const path TemporaryDirectoryPath = result + "\\judgel-temp";
const path TemporaryOutputPath = TemporaryDirectoryPath.u8string() + "\\out.txt";
const path TemporaryUserProgramPath = TemporaryDirectoryPath.u8string() + "\\prg.exe";
const int TerminalColumnWidth = 8;
//Declare functions, located at the end of judgel.cpp
int OutputCompare(path TestCaseOutput, path UserProgramOutput);
string MakeCompileCommand(path CurrentFilePath);
string MakeRunCommand(path CurrentFilePath);
void MakeNewFile(path CurrentFilePath);
string CurrentTestCaseCountOutputConverter(path CurrentTestCase);
void PrintHelpPage();
//Driver function
int main(int argc, char *argv[]) {
    //TLE request limit error handler
    try {
        remove_all(TemporaryDirectoryPath);
    } catch (const filesystem_error) {
        cout << ColorRed << "\nJUDGEL: Please wait before requesting\n" << ColorReset;
        cout << ColorGray << "Learn more at github.com/udontur/judgel/README.md#request-limit\n" << ColorReset;
        _exit(0);
        return 0;
    }
    //Invalid argument error handler
    if (argc != 4) {
        if (argc != 1) cout << ColorRed << "\nInvalid arguments\n" << ColorReset;
        PrintHelpPage();
        _exit(0);
        return 0;
    }
    //Create standard temporary folder, guarantee to be clear
    create_directory(TemporaryDirectoryPath);
    //Store arguments
    int RuntimeTimeLimit = stoi(argv[1]);
    path TestCaseDirectoryPath = argv[2];
    path UserProgramCppPath = argv[3];
    //Compile user's C++ code
    string CompileCommand = MakeCompileCommand(UserProgramCppPath);
    system("CLS");
    cout << ColorGray << "Compiling program...\n\n" << ColorReset;
    system(CompileCommand.c_str());
    cout << "\n";
    //Compilation Error handler
    if (!exists(TemporaryUserProgramPath)) {
        cout << ColorYellow << "\nJUDGEL (ABORT): Compilation Error\n" << ColorReset;
        remove_all(TemporaryDirectoryPath);
        _exit(0);
        return 0;
    }
    system("CLS");
    //User's C++ code is compiled to standard prg.exe file path
    cout << ColorGray << "\nTest    Verdict   Time\n" << ColorReset;
    int TestCaseCount = 0, MaximumRuntime = 0, FinalVerdict = 0, CurrentTestCaseRuntime;
    //Iterate through the given testcase folder
    for (path CurrentTestCase : directory_iterator(TestCaseDirectoryPath)) {
        //Each case consists of .in and .out, each alternating in an even and odd manner
        if (TestCaseCount % 2) { //User output checker
            //Get final verdict
            MaximumRuntime = max(MaximumRuntime, CurrentTestCaseRuntime);
            //Compare testcase output to user output
            int CurrentVerdictResult = OutputCompare(CurrentTestCase, TemporaryOutputPath);
            FinalVerdict = max(FinalVerdict, CurrentVerdictResult);
            //Output by verdict
            if (CurrentVerdictResult == 0) { //Runtime error
                //Terminal output message
                cout << left << setw(TerminalColumnWidth) << CurrentTestCaseCountOutputConverter(CurrentTestCase) + ":";
                cout << ColorYellow << "  RTE  " << ColorReset;
                cout << "   --ms\n\n";
                cout << ColorYellow << "JUDGEL (ABORT): Runtime Error\n" << ColorReset;
                //Session abortion
                remove_all(TemporaryDirectoryPath);
                _exit(0);
                return 0;
            } else if (CurrentVerdictResult == 2) {
                //Terminal output message
                cout << left << setw(TerminalColumnWidth) << CurrentTestCaseCountOutputConverter(CurrentTestCase) + ":";
                cout << ColorRed << "  WA    " << ColorReset;
                cout << right << setw(TerminalColumnWidth - 1) << to_string(CurrentTestCaseRuntime) + "ms\n";
            } else if (CurrentVerdictResult == 1) {
                //Terminal output message
                cout << left << setw(TerminalColumnWidth) << CurrentTestCaseCountOutputConverter(CurrentTestCase) + ":";
                cout << ColorGreen << "  AC    " << ColorReset;
                cout << right << setw(TerminalColumnWidth - 1) << to_string(CurrentTestCaseRuntime) + "ms\n";
            }
            //Delete the temporary user output file, and create a new one
            remove(TemporaryOutputPath);
            MakeNewFile(TemporaryOutputPath);
        } else { //Run testcase on the user program (prg.exe)
            //Make the "run user program with the current testcase" windows command
            string RunCommand = MakeRunCommand(CurrentTestCase);
            //Declare the time 
            time_point<high_resolution_clock> UserProgramStart;
            time_point<high_resolution_clock> UserProgramStop;
            //Declare the lambda function that runs the execution command, and measures the runtime
            auto RunSystem = [&UserProgramStart, &RunCommand, &UserProgramStop]() {
                UserProgramStart = high_resolution_clock::now();
                system(RunCommand.c_str());
                UserProgramStop = high_resolution_clock::now();
            };
            //Run the lambda function asynchronously
            auto UserProgramFuture = async(launch::async, RunSystem);
            //"Terminate" the running function after the time limit is exceeded
            if (UserProgramFuture.wait_for(seconds(RuntimeTimeLimit)) == future_status::timeout) {
                //Terminal output message
                cout << left << setw(TerminalColumnWidth) << CurrentTestCaseCountOutputConverter(CurrentTestCase) + ":";
                cout << ColorYellow << "  TLE  " << ColorReset;
                cout << right << setw(TerminalColumnWidth - 1) << ">" + to_string(RuntimeTimeLimit * 1000) + "ms\n\n";
                cout << ColorYellow << "JUDGEL (ABORT): Time Limit Exceed\n" << ColorReset;
                //Session abortion, cannot remove file as it is still in use refer to github.com/udontur/judgel/README.md#request-limit
                _exit(0);
                return 0;
            }
            //Calculate and update testcase runtime
            const auto UserProgramThreadRuntime = UserProgramStop - UserProgramStart;
            CurrentTestCaseRuntime = duration_cast<milliseconds>(UserProgramThreadRuntime).count();
        }
        //Increment testcase counter
        TestCaseCount++;
    }
    cout << "\n";
    //Final verdict judger
    if (FinalVerdict == 1) {
        //Terminal output message
        cout << ColorGreen;
        cout << "JUDGEL: Accepted ";
        cout << MaximumRuntime << "ms\n";
        cout << ColorReset;
    } else if (FinalVerdict == 2) {
        //Terminal output message
        cout << ColorRed;
        cout << "JUDGEL: Wrong Answer ";
        cout << MaximumRuntime << "ms\n";
        cout << ColorReset;
    } else {
        //Terminal output message
        cout << ColorRed;
        cout << "JUDGEL: A bug has been detected, please report it via github.com/udontur/judgel/issues. Thank you.\n";
        cout << ColorReset;
    }
    //Remove created temporary folder
    remove_all(TemporaryDirectoryPath);
    //End the program
    _exit(0);
    return 0;
}
//User program output and testcase output comparer
int OutputCompare(path TestCaseOutput, path UserProgramOutput) {
    //Runtime error checker
    if (file_size(UserProgramOutput) == 0) return 0;
    //File input
    ifstream TestCaseFout(TestCaseOutput);
    ifstream UserProgramFout(UserProgramOutput);
    //Check 2 time in both files to check for equality
    //Testcase iteration
    while (!TestCaseFout.eof()) {
        string CurrentStringTestCase, CurrentStringUserProgram;
        TestCaseFout >> CurrentStringTestCase;
        UserProgramFout >> CurrentStringUserProgram;
        //Return wrong answer
        if (CurrentStringUserProgram != CurrentStringTestCase) return 2;
    }
    //User program iteration
    while (!UserProgramFout.eof()) {
        string CurrentStringTestCase, CurrentStringUserProgram;
        TestCaseFout >> CurrentStringTestCase;
        UserProgramFout >> CurrentStringUserProgram;
        //Return wrong answer
        if (CurrentStringUserProgram != CurrentStringTestCase) return 2;
    }
    //Return accepted
    return 1;
}
//Make the user program compilation windows command
string MakeCompileCommand(path CurrentFilePath) {
    string compileCmd = "g++ ";
    compileCmd += CurrentFilePath.u8string();
    compileCmd += " -o ";
    compileCmd += TemporaryUserProgramPath.u8string();
    return compileCmd;
}
//Make the user program running with testcase windows command
string MakeRunCommand(path CurrentFilePath) {
    string run = TemporaryUserProgramPath.u8string();
    run += " < ";
    run += CurrentFilePath.u8string();
    run += " > ";
    run += TemporaryOutputPath.u8string();
    return run;
}
//Make new file function
void MakeNewFile(path CurrentFilePath) {
    ifstream tmp;
    tmp.open(CurrentFilePath);
}
//Testcase name parser
string CurrentTestCaseCountOutputConverter(path TestCasePath) {
    string TestCaseOutput = "";
    TestCaseOutput += TestCasePath.stem().u8string();
    return TestCaseOutput;
}
//Help page
void PrintHelpPage() {
    //Usage guide
    cout << ColorGray << "\nUsage: " << ColorReset;
    cout << ColorGreen << "judgel ";
    cout << ColorYellow << "<TIME_LIMIT(s, int)> ";
    cout << ColorBlue << "<TESTCASE/PATH> <CPPFILE_PATH>\n\n";
    cout << ColorReset;
    //Judgel program description
    cout << ColorGreen << "Judgel" << ColorReset;
    cout << " - Simple local C++ judge\n";
    cout << "Made with ";
    cout << ColorGreen << "passion" << ColorReset;
    cout << ", by ";
    cout << ColorGreen << "Hadrian (@udontur)\n" << ColorReset;
    cout << ColorGray << "Source: github.com/udontur/judgel\n";
    cout << "MIT license - Copyright (c) 2024 Hadrian Lau (github.com/udontur)\n\n" << ColorReset;
}
//Judgel.cpp ends
/**
 * judgel - v1.0
 * Made with passion, by Hadrian (@udontur)
 * Source - github.com/udontur/judgel
 * MIT license - Copyright © 2024 Hadrian Lau (github.com/udontur)
 **/