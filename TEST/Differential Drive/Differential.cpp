#include <cmath>
#include <iostream>
#include <fstream>

double ddx(double speed, double gain, double differential, double pos)
{
    return speed - speed * gain * pos / 5 - speed * differential * std::pow(pos / 5, 2);
}

double da(double speed, double ddx)
{
    return speed - ddx;
}

double px(double speed, double gain, double pos)
{
    return speed - speed * gain * pos / 5;
}

double dp(double speed, double px)
{
    return speed - px;
}

int main()
{
    double x = -5.0;
    double a = 0.0;
    double da = 0.0;
    double p = 0.0;
    double dp = 0.0;

    std::ofstream af;
    std::ofstream daf;
    std::ofstream pf;
    std::ofstream dpf;

    af.open("a.txt");
    af << "";
    af.close();

    pf.open("p.txt");
    af << "";
    pf.close();

    daf.open("da.txt");
    daf << "";
    daf.close();

    dpf.open("dp.txt");
    dpf << "";
    dpf.close();

    af.open("a.txt", std::ios_base::app); // append instead of overwrite
    daf.open("da.txt", std::ios_base::app);
    pf.open("p.txt", std::ios_base::app);
    dpf.open("dp.txt", std::ios_base::app);

    for (double i = -5.0; i < 5; i += 0.1)
    {
        a = ddx(1, 1, 0.5, x);
        std::cout << a << std::endl;
        af << a << std::endl;

        da = ::da(1, a);
        std::cout << da << std::endl;
        daf << da << std::endl;

        p = px(1, 1, x);
        std::cout << p << std::endl;
        pf << p << std::endl;

        dp = ::dp(1, p);
        std::cout << dp << std::endl;
        dpf << dp << std::endl;

        x += 0.1;
    }

    return 0;
}