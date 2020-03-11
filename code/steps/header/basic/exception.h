#ifndef EXCEPTION_H
#define EXCEPTION_H
class EXCEPTION
{
    public:
        EXCEPTION(string& message)
        {
            this->exception_message = message;
        }
        string get_exception_message() const
        {
            return exception_message;
        }

    private:
        string exception_message;
};

#endif // STEPSEXCEPTION_H
