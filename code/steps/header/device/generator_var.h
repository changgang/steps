#ifndef GENERATOR_VAR_H
#define GENERATOR_VAR_H

class GENERATOR_VAR
{
    public:
        GENERATOR_VAR();
    public:
        double R1, X1_sync, X1_transient, X1_subtransient;
        double R2, X2, R0, X0;
        double Rground, Xground;
};
#endif // GENERATOR_VAR_H
