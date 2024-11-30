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

using namespace std;
using namespace std::chrono;
using namespace std::filesystem; 

const string homefolder = getenv("HOME");
const path usrout = homefolder + "/.cache/judgel/out.txt";
const path usrprg = homefolder + "/.cache/judgel/usr.out";