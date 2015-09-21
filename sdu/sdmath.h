#pragma once


#ifndef SDMATH_H
#define SDMATH_H

#include "math2d.h"
#ifdef WIN
#include <boost/math/special_functions/fpclassify.hpp>
#endif
/**
* @file sdmath.h
* @author lw
* @brief 数学计算类
*
**/
namespace Crown
{
    /**
    * @defgroup 取最大最小值
    * @ingroup  Crown
    * @{
    */

    /**
    * @brief
    * 比较两个数据，返回最小值
    * @param const T & a ,const T & b : 进行比较的连个值
    * @return  T &  : 返回最小值
    */
    template<typename T >
    inline const T & SDMin(const T & __a, const T & __b)
    {
        return (__b < __a ) ? __b: __a;
    }

    /**
    * @brief
    * 比较两个数据，返回最大值
    * @param const T & a ,const T & b : 进行比较的连个值
    * @return T &  : 返回最大值
    */
    template<typename T >
    inline const T & SDMax(const T & __a, const T & __b)
    {
        return (__a < __b)? __b : __a;
    }

    /**
    * @brief
    * 把给定的数据限制在最大最小之间
    * @param const T& v, const T& m, const T& M : v给定将被限制的目标数据, m下限, M上限
    * @return T: 返回范围内的结果
    **/
    template <typename T>
    inline T SDClamp(const T& __v, const T& __m, const T& __M)
    {
        if (__v < __m) return __m;
        if (__v > __M) return __M;
        return __v;
    }

    template <typename T>
    inline bool isnan(T a)
    {
#ifdef WIN
        return boost::math::isnan(a);
#else
        return std::isnan(a);
#endif
    }

} //namespace Crown

#endif // 

