#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

int main()
{
    int count = 100000000;
    auto start_time = Clock::now();
    for (int i = 0; i < count; i++)
    {
        double a;
        a = 1000 / 5.0;
    }
    auto end_time = Clock::now();
    auto deltatime = end_time - start_time;

    std::cout << "floating point division: " << std::chrono::duration_cast<std::chrono::nanoseconds>(deltatime).count() << std::endl;

    start_time = Clock::now();
    for (int i = 0; i < count; i++)
    {
        double a;
        a = 1000 / 5;
    }
    end_time = Clock::now();
    deltatime = end_time - start_time;

    std::cout << "division: " << std::chrono::duration_cast<std::chrono::nanoseconds>(deltatime).count() << std::endl;

    start_time = Clock::now();
    for (int i = 0; i < count; i++)
    {
        double a;
        a = 1000 * 0.2;
    }
    end_time = Clock::now();
    deltatime = end_time - start_time;
    std::cout << "floating point multiplication: " << std::chrono::duration_cast<std::chrono::nanoseconds>(deltatime).count() << std::endl;

    return 0;
}

/*Conclusion: floating point division > division > floating point multiplication*/