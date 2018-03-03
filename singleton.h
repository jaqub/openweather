#ifndef SINGLETON_H
#define SINGLETON_H
#include <utility>

template <typename T>
class Singleton
{
public:
    template <typename... Params>
    static T* get(Params... aParams)
    {
        static T mInstance(std::forward<Params>(aParams)...);
        return &mInstance;
    }

protected:
    Singleton(){}

    virtual ~Singleton(){}

private:
    Singleton(Singleton const &);

    Singleton &operator=(Singleton const &);
};

#endif // SINGLETON_H
