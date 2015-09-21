#ifndef VALUEGETTER_HPP
#define VALUEGETTER_HPP

#include <functional>
#include <boost/bind/bind.hpp>

namespace Crown {
template <typename T>
struct Getter
{
    typedef T Type;
    typedef std::function<T()> GetterType;
    typedef Getter<T> SelfType;

    GetterType m_Getter;

    Getter(const T& value) :
        m_Getter(boost::bind(&SelfType::Get, this, value))
    {}

    Getter(GetterType getter) :
        m_Getter(getter)
    {}

    T operator ()() const
    {
        return m_Getter();
    }

private:
    T Get(T value)
    {
        return value;
    }
};

}
#endif // VALUEGETTER_HPP
