#include <bits/stdc++.h> //g++ judgel.cpp -o judgel.exe
using namespace std; //.\judgel.bat C:\Users\hadri\code\github\judgel\testcase C:\Users\hadri\code\github\judgel\user\hello.cpp    
namespace fs=filesystem;
bool cmp(fs::path testcase, fs::path user){
    ifstream testcaseout(testcase);
    ifstream userout(user);
    while(!testcaseout.eof()){
        string test, usr;
        testcaseout>>test;
        userout>>usr;
        if(usr!=test) return 0;
    }
    while(!userout.eof()){
        string test, usr;
        testcaseout>>test;
        userout>>usr;
        if(usr!=test) return 0;
    }
    return 1;
}
int main(){
    fs::path testcasePath, prgcppPath;
    ifstream tempparseIn("C:\\Users\\hadri\\code\\github\\judgel\\temp\\parse.txt");
    tempparseIn>>testcasePath>>prgcppPath;
    cout<<"Test Case path: "<<testcasePath<<"\n";
    cout<<"User Program path: "<<prgcppPath<<"\n";
    string compileCmd="g++ ";
    compileCmd+=prgcppPath.u8string();
    compileCmd+=" -o ";
    compileCmd+="C:\\Users\\hadri\\code\\github\\judgel\\temp\\prg.exe";
    cout<<"Compile command: "<<compileCmd<<"\n";
    system(compileCmd.c_str()); //g++ prgcppPath -o Located path
    int cnt=0;
    for(fs::path testcase: fs::directory_iterator(testcasePath)){
        if(!(cnt%2)){
            string run="C:\\Users\\hadri\\code\\github\\judgel\\temp\\prg.exe < ";
            run+=testcase.u8string();
            run+=" > C:\\Users\\hadri\\code\\github\\judgel\\temp\\out.txt";
            cout<<"Program run command: "<<run<<"\n";
            system(run.c_str());
        }else{
            if(cmp(testcase, "C:\\Users\\hadri\\code\\github\\judgel\\temp\\out.txt")){
                cout<<cnt/2<<": AC\n";
            }else{
                cout<<cnt/2<<": WA\n";
            }
            remove("C:\\Users\\hadri\\code\\github\\judgel\\temp\\out.txt");
            ifstream tmp;
            tmp.open("C:\\Users\\hadri\\code\\github\\judgel\\temp\\out.txt");
        }
        cnt++;
    }
}