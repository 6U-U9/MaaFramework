#include "Encode.h"

#include "Utils/Logger.hpp"

MAA_CTRL_UNIT_NS_BEGIN

bool ScreencapEncode::parse(const json::value& config)
{
    return parse_argv("ScreencapEncode", config, screencap_encode_argv_);
}

bool ScreencapEncode::init(int w, int h)
{
    set_wh(w, h);
    return true;
}

std::optional<cv::Mat> ScreencapEncode::screencap()
{
    LogFunc;

    if (!io_ptr_) {
        LogError << "io_ptr is nullptr";
        return std::nullopt;
    }

    auto cmd_ret = command(screencap_encode_argv_.gen(argv_replace_));

    if (!cmd_ret) {
        return std::nullopt;
    }

    return process_data(cmd_ret.value(), std::bind(&ScreencapBase::decode_png, this, std::placeholders::_1));
}

MAA_CTRL_UNIT_NS_END
