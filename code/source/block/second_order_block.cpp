#include "header/block/second_order_block.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

SECOND_ORDER_BLOCK::SECOND_ORDER_BLOCK()
{
    set_a(0.0);
    set_b(0.0);
    set_c(0.0);
    set_d(0.0);
    set_e(0.0);
    set_limiter_type(NO_LIMITER);
}

SECOND_ORDER_BLOCK::~SECOND_ORDER_BLOCK()
{
    ;
}

void SECOND_ORDER_BLOCK::set_a(double a)
{
    this->a = a;
}

void SECOND_ORDER_BLOCK::set_b(double b)
{
    this->b = b;
}

void SECOND_ORDER_BLOCK::set_c(double c)
{
    this->c = c;
}

void SECOND_ORDER_BLOCK::set_d(double d)
{
    this->d = d;
}

void SECOND_ORDER_BLOCK::set_e(double e)
{
    this->e = e;
}

double SECOND_ORDER_BLOCK::get_a() const
{
    return a;
}

double SECOND_ORDER_BLOCK::get_b() const
{
    return b;
}

double SECOND_ORDER_BLOCK::get_c() const
{
    return c;
}

double SECOND_ORDER_BLOCK::get_d() const
{
    return d;
}

double SECOND_ORDER_BLOCK::get_e() const
{
    return e;
}

void SECOND_ORDER_BLOCK::initialize()
{
    // when d!=0
    // (ds+e)/(as^2+bs+c)=k(s+h)/[(s+f)(s+g)]
    // where: f = -[-b+sqrt(b^2-4ac)]/(2a)
    //        g = -[-b-sqrt(b^2-4ac)]/(2a)
    //        h = e/d
    //        k = d/a
    // k(s+h)/[(s+f)(s+g)]=k(s+g+h-g)/[(s+f)(s+g)]=k/(s+f)*[1+(h-g)/(s+g)]
    // therefore, block1 is: k/(s+f)
    //            block2 is: (h-g)/(s+g)
    // the whole block is: block1*(1+block2)

    // when d=0
    // (ds+e)/(as^2+bs+c)=k/[(s+f)(s+g)]
    // where: f = -[-b+sqrt(b^2-4ac)]/(2a)
    //        g = -[-b-sqrt(b^2-4ac)]/(2a)
    //        k = e/a
    // k/[(s+f)(s+g)] = k/(s+f)*(1/(s+g))
    // therefore, block1 is: k/(s+f)
    //            block2 is: 1/(s+g)
    // the whole block is: block1*block2

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    block1.set_toolkit(toolkit);
    block2.set_toolkit(toolkit);

    double b2_minus_4ac = b*b-4.0*a*c;
    double root_of_b2_minus_4ac = sqrt(b2_minus_4ac);
    double f = -(-b+root_of_b2_minus_4ac)/(2.0*a);
    double g = -(-b-root_of_b2_minus_4ac)/(2.0*a);
    if(d!=0)
    {
        double h = e/d;
        double k = d/a;
        block1.set_K(k);
        block1.set_T_in_s(f);
        block2.set_K(h-g);
        block2.set_T_in_s(g);
    }
    else
    {
        double k = e/a;
        block1.set_K(k);
        block1.set_T_in_s(f);
        block2.set_K(1.0);
        block2.set_T_in_s(g);
    }

    double y = get_output();
    //double input = y/(e/c);
    double input = y*c/e;
    set_input(input);
    if(d!=0)
    {
        double output1 = input*block1.get_K();
        block1.set_output(output1);
        block1.initialize();
        double output2 = y-output1;
        block2.set_output(output2);
        block2.initialize();
    }
    else
    {
        block2.set_output(y);
        block2.initialize();
        double output1 = block2.get_input();
        block1.set_output(output1);
        block1.initialize();
    }
}

void SECOND_ORDER_BLOCK::run(DYNAMIC_MODE mode)
{
    if(mode==INTEGRATE_MODE)
        integrate();
    if(mode==UPDATE_MODE)
        update();
}

void SECOND_ORDER_BLOCK::integrate()
{
    double input = get_input();
    block1.set_input(input);
    block1.run(INTEGRATE_MODE);
    input = block1.get_output();
    block2.set_input(input);
    block2.run(INTEGRATE_MODE);
    set_output(block1.get_output()+block2.get_output());
}

void SECOND_ORDER_BLOCK::update()
{
    double input = get_input();
    block1.set_input(input);
    block1.run(UPDATE_MODE);

    input = block1.get_output();
    block2.set_input(input);
    block2.run(UPDATE_MODE);

    set_output(block1.get_output()+block2.get_output());
}

void SECOND_ORDER_BLOCK::check()
{
    check_limiter();
}
