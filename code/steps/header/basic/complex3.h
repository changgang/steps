#ifndef COMPLEX3_H
#define COMPLEX3_H

#include <string>
#include <complex>
using namespace std;

class COMPLEX3
{
    public:
        COMPLEX3();
        COMPLEX3(complex<double> x, complex<double> y, complex<double> z);
        COMPLEX3(const COMPLEX3& V);
        COMPLEX3& operator=(const COMPLEX3& V);
        void copy_from_const_complex3(const COMPLEX3& V);
        virtual ~COMPLEX3();

        void set_complex_value_0(complex<double> x);
        void set_complex_value_1(complex<double> y);
        void set_complex_value_2(complex<double> z);

        complex<double> get_complex_value_0() const;
        complex<double> get_complex_value_1() const;
        complex<double> get_complex_value_2() const;

        complex<double> operator[](char index) const;
    private:
        complex<double> x, y, z;
};
#endif // COMPLEX3_H
