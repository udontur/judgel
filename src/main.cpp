#include "lib/judgel.h"

int main(int argc, char* argv[]){
    
    if(!(argc==1||argc==2)){
        cout<<"Error: Invalid argument\n";
        ClearCache();
        PrintHelpPage();
        return 0;
    }

    string times="1";
    if(argc==2) times=argv[1];
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
    system("clear");

    string runcmd="g++ main.cpp -o "+static_cast<string>(usrprg);
    system(runcmd.c_str());

    if(!exists(usrprg)){
        cout<<"CA\n";
        ClearCache();
        return 0;
    }
    
    //cout << ColorRed;
    //cout << "\nTest    Verdict   Time\n";
    //cout << ColorReset;

    int incr=0;
    vector<string> tculp;
    for(path CurrentTestCase: recursive_directory_iterator("testcase")){
        string t=CurrentTestCase.u8string();
        int n=t.size();
        if(t[n-1]=='n'){
            for(int j =0; j<3; j++) t.pop_back();
            tculp.push_back(t);
        }
    }
    sort(tculp.begin(), tculp.end());
    /*for(auto i: tculp){
        cout<<i<<"\n";
    }*/
    for(string CurrentTestCase: tculp){
        //if(exists(usrout)) system("rm ~/.cache/judgel/out.txt");
        string ctcin=CurrentTestCase+".in";
        string ctcout=CurrentTestCase+".out";
        string RunCommand="~/.cache/judgel/usr.out < ";
        RunCommand+=ctcin; 
        RunCommand+=" > ~/.cache/judgel/out.txt";
        time_point<high_resolution_clock> UserProgramStart;
        time_point<high_resolution_clock> UserProgramStop;
        int state;
        auto RunSystem = [&UserProgramStart, &RunCommand, &UserProgramStop, &state]() {
            UserProgramStart = high_resolution_clock::now();
            state=system(RunCommand.c_str());
            UserProgramStop = high_resolution_clock::now();
        };
        auto UserProgramFuture = async(launch::async, RunSystem);
       
        if(UserProgramFuture.wait_for(seconds(RunTimeLimit)) == future_status::timeout){
            cout<<"TLE\n";
            system("pkill -9 -f ~/.cache/judgel/usr.out");
            ClearCache();
            return 0;
        }
        if(WEXITSTATUS(state)!=0){
            cout<<"RTE\n";
            return 0;
        }
        bool ac=isac(ctcout);
        
        if(ac==1){
            cout<<"AC\n";
        }else if(ac==0){
            cout<<"WA\n";
        }
    }
    ClearCache();
    return 0;
}

