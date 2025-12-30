#pragma once

#include "DefinesMacros.hpp"
#include "WindowsException.hpp"

#include <windows.h>

template <typename HandleType, auto CloseFunction>
class SmartHandleBase final
{
public:
	explicit SmartHandleBase(HandleType handle) :
		m_handle(handle)
    {
    }

    SmartHandleBase(const SmartHandleBase&) = delete;
    SmartHandleBase(SmartHandleBase&& other) = delete;
    SmartHandleBase& operator=(const SmartHandleBase&) = delete;
    SmartHandleBase& operator=(SmartHandleBase&& other) = delete;

    ~SmartHandleBase()
    {
        try
        {
            // This stub exists to get rid of the advapi32 dependency in my stage1
            BOOL close_result = FALSE;

            if constexpr (std::is_same_v<decltype(CloseFunction), decltype(&CloseServiceHandle)>)
            {
                close_result = RESOLVE(advapi32.dll, CloseServiceHandle)(m_handle);
            }
            else
            {
                close_result = CloseFunction(m_handle);
            }

            if (close_result == FALSE)
            {
                throw WindowsException(ArcaneErrors::ErrorCodes::CloseHandleCloseServiceHandleFailed);
            }
        }
        CATCH_ALL(OBFUSCATE("Exception in SmartHandleBase Destructor!"))
    }

    HandleType get() const
    {
        return m_handle;
    }

    explicit operator bool() const
    {
		return m_handle != INVALID_HANDLE_VALUE;
    }

private:
    HandleType m_handle;
};

using SmartHandle = SmartHandleBase<HANDLE, CloseHandle>;
using SmartSCHandle = SmartHandleBase<SC_HANDLE, CloseServiceHandle>;