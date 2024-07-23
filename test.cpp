#include <iostream>
#include <future>
#include <chrono>
#include <thread>

// Example of a long-running function
void longRunningFunction() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Function finished" << std::endl;
}

int main() {
    // Launch the long-running function asynchronously
    auto future = std::async(std::launch::async, longRunningFunction);

    // Wait for the function to complete for up to 3 seconds
    if (future.wait_for(std::chrono::seconds(3)) == std::future_status::timeout) {
        std::cout << "Function did not finish within 3 seconds" << std::endl;
    } else {
        // If the function finished within 3 seconds, get the result
        future.get();
        std::cout << "Function finished within 3 seconds" << std::endl;
    }

    std::cout << "Program continues..." << std::endl;
    return 0;
}