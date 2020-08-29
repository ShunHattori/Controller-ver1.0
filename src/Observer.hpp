#pragma once

template <typename T>
class Observer
{
private:
    T cache;

public:
    Observer(){};
    virtual ~Observer(){};

    bool isChanged(T param);
};

template <typename T>
inline bool Observer<T>::isChanged(T param)
{
    bool flag = false;
    if (param != cache)
        flag = true;
    cache = param;
    return flag;
}
