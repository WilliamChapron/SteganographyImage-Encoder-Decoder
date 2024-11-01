#pragma once

#include <windows.h>
#include <iostream>

class ClientApp;

template<typename MethodType, typename InstanceType>
struct ThreadData {
    MethodType method;           
    InstanceType* instance;      
};

class Thread {
public:

    template<typename InstanceType, typename MethodType>
    static DWORD WINAPI StaticThreadFunction(LPVOID lpParam) {
        auto* data = reinterpret_cast<ThreadData<MethodType, InstanceType>*>(lpParam);
        (data->instance->*(data->method))();
        delete data;
        return 0;
    }

    template<typename InstanceType, typename MethodType>
    HANDLE Init(InstanceType* instance, MethodType method) {
        auto* threadData = new ThreadData<MethodType, InstanceType>{ method, instance };

        HANDLE threadHandle = CreateThread(
            NULL,
            0,
            StaticThreadFunction<InstanceType, MethodType>,
            threadData,
            0,
            NULL
        );

        return threadHandle; 
    }

    HANDLE m_handle; 
};
