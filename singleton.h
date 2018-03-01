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
        if(!mInstance)
        {
            mInstance = new T(std::forward<Params>(aParams)...);
        }
        return mInstance;
    }

    static void del()
    {
        delete mInstance;
        mInstance = nullptr;
    }

protected:
    Singleton(){}

    virtual ~Singleton(){}

private:
    Singleton(Singleton const &);

    Singleton &operator=(Singleton const &);

    static T* mInstance;
};

template <typename T> T* Singleton<T>::mInstance = nullptr;

#endif // SINGLETON_H
