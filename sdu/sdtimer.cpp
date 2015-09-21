#include "SDuPCH.h"
#include "sdtimer.h"
#include "sdtimer_impl.h"

namespace Crown
{

    CSDTimer::CSDTimer()
    {
        m_pTimerImpl = CnNew CSDTimerImpl();
    }
    CSDTimer::~CSDTimer()
    {
        CnDelete m_pTimerImpl;
    }

    bool SDAPI CSDTimer::SetTimer(ISDTimer* poTimerHandler, uint32 dwTimerId, uint32 dwElapse, uint32 dwLoop/* = 0xFFFFFFFF */)
    {
        return m_pTimerImpl->SetTimer(poTimerHandler,dwTimerId,dwElapse,dwLoop);
    }

    bool SDAPI CSDTimer::KillTimer(uint32 dwTimerId)
    {
        return m_pTimerImpl->KillTimer(dwTimerId);
    }

    /**
    * @brief
    * 时钟消息处理
    * @return 如果还有事件没处理完返回true，如果所有事件都处理完了，返回false
    */
    bool SDAPI CSDTimer::Run()
    {
        return  m_pTimerImpl->Run();
    }

    /**
    * @brief
    * 打印时钟信息
    * @return void
    */
	std::string SDAPI CSDTimer::DumpTimerInfo()
    {
        return m_pTimerImpl->DumpTimerInfo();
    }
};

