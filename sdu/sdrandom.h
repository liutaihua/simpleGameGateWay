#pragma once

#ifndef SDRANDOM_H
#define SDRANDOM_H
/**
* @file sdrandom.h
* @author lw
* @brief 随机数系列
*
**/
#include "sdtype.h"
#include <cstdlib>

namespace Crown
{
    /**
    * @defgroup grouprandom 随机数操作
    * @ingroup  Crown
    * @{
    */

    /**
    * @brief
    * C-API方式，设置随机数种子
    * @param dwSeed ：随机数种子
    * @return void
    */
    void SDSetSeed(uint32 dwSeed);

    /**
    * @brief
    * C-API方式，获取随机数
    * @return 返回随机数
    */
    int32 SDRandom();

    /**
    * @brief
    * C-API方式，获取随机数
    * @return 返回指定范围内的随机数
    */
    uint32 SDRandom(uint32 dwRange);

    float SDRandomFloat(float range);
    /**
    *@brief 随机数操作接口
    */
    class CSDRandom
    {
    public:
        CSDRandom();

        /**
        * @brief
        * 设置随机数种子
        * @param dwSeed ：随机数种子
        * @return void
        */
        inline void SetSeed(uint32 dwSeed)
        {
            m_seed = dwSeed;
        }

        /**
        * @brief
        * 获取随机数
        * @return 返回随机数
        */
        uint32 Random();


        /**
        * @brief
        * 获取随机数，范围是[0, unRange - 1]
        * @param unRange ：最大值
        * @return 返回随机数
        */
        uint32 Random(uint32 dwRange);


        /**
        * @brief
        * 获取随机数，范围是[unMinRange, unMaxRange - 1]
        * @param dwMin ：最小值
        * @param dwMax ：最大值
        * @return 返回随机数
        */
        uint32 Random(uint32 dwMin, uint32 dwMax);


    private:
        uint32 m_seed;
    };

}

#endif
