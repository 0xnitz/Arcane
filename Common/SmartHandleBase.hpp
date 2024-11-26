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
            const BOOL close_result = CloseFunction(m_handle);
            if (close_result == FALSE)
            {
                throw WindowsException(ArcaneErrors::ErrorCodes::CloseHandleCloseServiceHandleFailed);
            }
        }
        CATCH_ALL("Exception in SmartHandleBase Destructor!")
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