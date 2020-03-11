#ifndef SATURATION_BLOCK_H
#define SATURATION_BLOCK_H

#include "header/basic/steps_enum.h"


class SATURATION_BLOCK
{
    // sK/(1+sT), a.k.a., washout
    public:
        SATURATION_BLOCK();
        virtual ~SATURATION_BLOCK();

        void set_saturation_type(SATURATION_TYPE sat_type);
        void set_V1(double V);
        void set_V2(double V);
        void set_S1(double S);
        void set_S2(double S);

        SATURATION_TYPE get_saturation_type() const;
        double get_V1() const;
        double get_V2() const;
        double get_S1() const;
        double get_S2() const;

        double get_saturation(double V);
        bool is_saturation_considered() const;
        virtual void check();
    private:
        inline bool is_ABC_initialized() const;

        SATURATION_TYPE type;
        bool is_initialized;
        double A, B, C;
        double V1, V2, S1, S2;
        //S=B(V-A)^2/V or
        //S=A*V^X
};
#endif // SATURATION_BLOCK_H
