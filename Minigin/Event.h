#pragma once
#include <string>

namespace dae
{
    class Event
    {
    public:
        virtual ~Event() = default;
        virtual std::string GetEventName() const = 0;
    };
}