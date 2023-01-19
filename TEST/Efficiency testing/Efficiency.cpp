#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

// #include <iostream>
// #include <chrono>
// typedef std::chrono::high_resolution_clock Clock;
// main() {
//    auto start_time = Clock::now();
//    for(int i = 0; i<200000000; i++ ){ //create one empty loop
//       ;
//    }
//    auto end_time = Clock::now();
//    std::cout << "Time difference: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time    - start_time).count() << " nanoseconds" << std::endl;
// }

int main()
{
    auto start_time = Clock::now();
    for (int i = 0; i < 1000000000; i++)
    {
        double a;
        a = 1000 * 0.2;
    }
    auto end_time = Clock::now();
    auto deltatime = end_time - start_time;

    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(deltatime).count() << std::endl;
    return 0;
}