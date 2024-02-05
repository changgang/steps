#ifndef CONVCOL0_H
#define CONVCOL0_H

#include "header/STEPS.h"
#include "header/basic/steps_enum.h"
class CONVCOL0
{
    public:
        CONVCOL0(STEPS& toolkit);
        ~CONVCOL0();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_PQ_priority_flag(COL_PQ_PRIORITY pq_flag);
        void set_Imax(double I);

        COL_PQ_PRIORITY get_PQ_priority_flag() const;
        double get_Imax() const;

        complex<double> get_limited_PQ_current_order(complex<double> I) const;
    private:
        STEPS* toolkit;
        COL_PQ_PRIORITY pq_priority;
        double Imax;
};

#endif // CONVCOL0_H
