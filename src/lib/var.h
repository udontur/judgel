#include <stdlib.h>
#include <stdio.h>
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
const string homefolder = getenv("HOME");
const path usroutput = homefolder + "/.cache/judgel/out.txt";
