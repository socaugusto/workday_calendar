#pragma once
#include "commoncalendar.h"
#include <chrono>
#include <format>
#include <string>

class SimpleDateFormat
{
    std::string fmt_;

  public:
    constexpr SimpleDateFormat(std::string_view fmt) : fmt_("{:" + std::string(fmt) + "}")
    {
    }

    std::string format(const DateTime &dt) const
    {
        auto tp = std::chrono::sys_days(dt.date) + dt.time.to_duration();
        return std::vformat(fmt_, std::make_format_args(tp));
    }
};