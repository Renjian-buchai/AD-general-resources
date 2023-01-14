#include <cmath>
#include <iostream>
#include <fstream>

int main()
{
    double x = 0.0;
    double a = 0.0;
    double da = 0.0;

    std::ofstream af;
    std::ofstream daf;

    af.open("a.txt", std::ios_base::app); // append instead of overwrite
    daf.open("da.txt", std::ios_base::app);
    for (double i = 0.0; i <= 5; i += 0.1)
    {
        a = (x - x / 5 - std::pow(x / 5, 3)) / 3;
        std::cout << a << std::endl;
        af << a << std::endl;

        da = 1 - a;
        std::cout << da << std::endl;
        daf << da << std::endl;
        x += 0.1;
    }

    return 0;
}