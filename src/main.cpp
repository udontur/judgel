#include "lib/judgel.h"

int main(int argc, char * argv[]) {

    if (!(argc == 1 || argc == 2)) {
        cout << ColorRed;
        cout << "Error: ";
        cout << ColorReset;
        cout << "Invalid arguments\n";
        ClearCache();
        PrintHelpPage();
        return 0;
    }

    string times = "1";
    if (argc == 2)
        times = argv[1];
    for (int i = 0; i < times.size(); i++) {
        if (argv[1][i] <= '0' || argv[1][i] >= '9') {
            cout << ColorRed;
            cout << "Error: ";
            cout << ColorReset;
            cout << "Time limit is not an integer\n";
            ClearCache();
            PrintHelpPage();
            return 0;
        }
    }
    int RunTimeLimit = stoi(times);

    if (!exists("testcase")) {
        cout << ColorRed;
        cout << "Error: ";
        cout << ColorReset;
        cout << "\"testcase\" not found\n";
        ClearCache();
        PrintHelpPage();
        return 0;
    }
    if (!exists("main.cpp")) {
        cout << ColorRed;
        cout << "Error: ";
        cout << ColorReset;
        cout << "\"main.cpp\" not found\n";
        ClearCache();
        PrintHelpPage();
        return 0;
    }

    ClearCache();

    //system("clear");
    cout << ColorGray;
    cout << "Compiling program...\n";
    cout << ColorReset;

    string runcmd = "g++ main.cpp -o " + static_cast < string > (usrprg);
    system(runcmd.c_str());

    if (!exists(usrprg)) {
        cout << ColorRed;
        cout << "Error: ";
        cout << ColorReset;
        cout << "Compilation error\n";
        ClearCache();
        return 0;
    }

    cout<<"\033[1A\r\033[K";

    vector < string > tculp;
    for (path CurrentTestCase: recursive_directory_iterator("testcase")) {
        string t = static_cast < string > (CurrentTestCase);
        int n = t.size();
        if (t[n - 1] == 'n') {
            for (int j = 0; j < 3; j++) t.pop_back();
            tculp.push_back(t);
        }
    }
    sort(tculp.begin(), tculp.end());

    cout << "\033[90mTest       Verdict         Time\033[0m\n";
    int finalverdict=-1;
    bool skip = 0;
    for (string CurrentTestCase: tculp) {

        string ctcin = CurrentTestCase + ".in";
        string ctcout = CurrentTestCase + ".out";
        string ctcname = strip(CurrentTestCase);
        string RunCommand = "~/.cache/judgel/usr.out < " + ctcin + " > ~/.cache/judgel/out.txt";

        if (skip) {
            cout << format("{:<10} Skipped          N/A\n", ctcname);
            continue;
        }

        int state;
        time_point < high_resolution_clock > UserProgramStart;
        time_point < high_resolution_clock > UserProgramStop;

        auto RunSystem = [ & UserProgramStart, & RunCommand, & UserProgramStop, & state]() {
            UserProgramStart = high_resolution_clock::now();
            state = system(RunCommand.c_str());
            UserProgramStop = high_resolution_clock::now();
        };

        auto UserProgramFuture = async (launch::async, RunSystem);

        if (UserProgramFuture.wait_for(seconds(RunTimeLimit)) == future_status::timeout) {
            cout << format("{:<10} \033[33mTime Limit\033[0m   >{}s\n", ctcname, to_string(RunTimeLimit) + ".000");
            system("pkill -9 -f ~/.cache/judgel/usr.out");
            skip = 1;
            finalverdict=3;
            continue;
        }

        const auto RuntimeTime = duration_cast < milliseconds > (UserProgramStop - UserProgramStart);
        int RuntimeInt = RuntimeTime.count();
        double Runtime = RuntimeInt / 1000.0;

        cout << fixed << setprecision(3);

        if (WEXITSTATUS(state) != 0) {
            cout << format("{:<10} \033[33mRuntime Error\033[0m {}s\n", ctcname, Runtime);
            skip = 1;
            finalverdict=2;
            continue;
        }

        bool accepted = OutputComparer(ctcout);

        if (accepted == 1) {
            cout << format("{:<10} \033[32mAccepted\033[0m      {}s\n", ctcname, Runtime);
            finalverdict=0;
        } else if (accepted == 0) {
            cout << format("{:<10} \033[31mWrong Answer\033[0m  {}s\n", ctcname, Runtime);
            finalverdict=1;
        }

    }
    
    if(finalverdict==0) cout<<"\n\033[32mFinal Verdict: Accepted\033[0m\n";
    else if(finalverdict==1) cout<<"\n\033[31mFinal Verdict: Wrong Answer\033[0m\n";
    else if(finalverdict==2) cout<<"\n\033[33mFinal Verdict: Runtime Error\033[0m\n";
    else if(finalverdict==3) cout<<"\n\033[33mFinal Verdict: Time Limit Exceed\033[0m\n";
    else cout<<"\033[34mYou’ve wandered off the map!\033[0m\n";

    ClearCache();
    return 0;

}