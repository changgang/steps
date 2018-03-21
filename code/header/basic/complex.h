#ifndef COMPLEX_H
#define COMPLEX_H

#include <string>
using namespace std;

class COMPLEX
{
    public:
        COMPLEX();
        COMPLEX(double x);
        COMPLEX(double x, double y);
        COMPLEX(const COMPLEX& z);
        COMPLEX& operator=(const COMPLEX& z);
        void copy_from_const_complex(const COMPLEX& z);
        COMPLEX& operator=(const double x);
        virtual ~COMPLEX();

        COMPLEX& operator+(const COMPLEX& z);
        COMPLEX& operator+(const double x);

        COMPLEX& operator+=(const COMPLEX& z);
        COMPLEX& operator+=(const double x);

        COMPLEX& operator-(const COMPLEX& z);
        COMPLEX& operator-(const double x);

        COMPLEX& operator-=(const COMPLEX& z);
        COMPLEX& operator-=(const double x);

        COMPLEX& operator*(const COMPLEX& z);
        COMPLEX& operator*(const double x);

        COMPLEX& operator/(const COMPLEX& z);
        COMPLEX& operator/(const double x);

        double real() const;
        double imag() const;
        double mag() const;
        double arg() const;
        COMPLEX conj() const;
    private:
        double x, y;
};
#endif // COMPLEX_H
