#ifndef RATING_H
#define RATING_H

class RATING
{
    public:
        RATING();
        virtual ~RATING();

        void set_rating_A_MVA(double rate);
        void set_rating_B_MVA(double rate);
        void set_rating_C_MVA(double rate);

        double get_rating_A_MVA() const;
        double get_rating_B_MVA() const;
        double get_rating_C_MVA() const;

        double operator[](char index) const;
        virtual RATING& operator= (const RATING& rating);

        void clear();

    private:
        double rating_A_MVA;
        double rating_B_MVA;
        double rating_C_MVA;
};

#endif // RATING_H
