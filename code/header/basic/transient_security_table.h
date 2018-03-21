#ifndef TRANSIENT_SECURITY_TABLE_H
#define TRANSIENT_SECURITY_TABLE_H

class TRANSIENT_SECURITY_TABLE
{
    public:
        TRANSIENT_SECURITY_TABLE();
        TRANSIENT_SECURITY_TABLE(const TRANSIENT_SECURITY_TABLE& table);
        virtual ~TRANSIENT_SECURITY_TABLE();
        virtual TRANSIENT_SECURITY_TABLE& operator=(const TRANSIENT_SECURITY_TABLE& table);

        void set_threshold(double threshold);
        void set_time_span(double time);
        void set_check_for_greater_flag(bool logic);

        double get_threshold() const;
        double get_time_span() const;
        bool get_check_for_greater_flag() const;

    private:
        void copy_from_const_table(const TRANSIENT_SECURITY_TABLE& table);
        double threshold;
        double time_span;
        bool check_for_greater;

};

#endif // TRANSIENT_SECURITY_TABLE_H
