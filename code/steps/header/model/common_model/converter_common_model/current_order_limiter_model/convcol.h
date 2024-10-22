#ifndef CONVCOL0_H
#define CONVCOL0_H

#include "header/model/common_model/common_model.h"

class CONVCOL0:public COMMON_MODEL
{
    public:
        CONVCOL0(STEPS& toolkit);
        ~CONVCOL0();
        string get_model_name() const;

        void set_PQ_priority_flag(COL_PQ_PRIORITY pq_flag);
        void set_Imax(double I);

        COL_PQ_PRIORITY get_PQ_priority_flag() const;
        double get_Imax() const;

        complex<double> get_limited_PQ_current_order(complex<double> I) const;
    private:
        COL_PQ_PRIORITY pq_priority;
        double Imax;
};

#endif // CONVCOL0_H
