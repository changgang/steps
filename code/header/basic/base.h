#ifndef BASE_H
#define BASE_H

class POWER_SYSTEM_DATABASE;

class BASE
{
    public:
        BASE();
        virtual ~BASE();

        void set_power_system_database(POWER_SYSTEM_DATABASE* db);
        POWER_SYSTEM_DATABASE* get_power_system_database() const;

        virtual bool is_valid() const = 0;
        virtual void check() = 0;
        virtual void clear()  = 0;
    private:
        POWER_SYSTEM_DATABASE* db;
};

#endif // BASE_H
