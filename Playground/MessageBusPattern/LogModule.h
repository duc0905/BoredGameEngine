#pragma once
#include <iostream>
#include "Module.h"
#include "LogMessage.h"

//template<class T>
//class Singleton
//{
//private:
//    static T* instance_;
//    
//    Singleton<T>() {};
//    ~Singleton<T>() { instance_ = nullptr; }
//public:
//    inline static T* GetInstance() const { return instance_; }
//};

class LogModule :
    public Module
    //, public Singleton<LogModule>
{
public:
    // Inherited via Module
    virtual void OnMessage(Message& msg) override;
    ~LogModule() = default;
};

