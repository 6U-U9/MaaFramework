#include "ScreencapBase.h"

#include "Utils/Logger.hpp"
#include "Utils/NoWarningCV.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4068)
#endif
#include <gzip/decompress.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

MAA_CTRL_UNIT_NS_BEGIN

void ScreencapBase::set_wh(int w, int h)
{
    width_ = w;
    height_ = h;
}

std::optional<cv::Mat> ScreencapBase::process_data(
    std::string& buffer, std::function<std::optional<cv::Mat>(const std::string& buffer)> decoder)
{
    bool tried_clean = false;

    if (end_of_line_ == EndOfLine::CRLF) {
        tried_clean = true;
        if (!clean_cr(buffer)) {
            LogInfo << "end_of_line is set to CRLF but no `\\r\\n` found, set it to LF";
            end_of_line_ = EndOfLine::LF;
        }
    }

    auto res = decoder(buffer);

    if (res) {
        if (end_of_line_ == EndOfLine::UnknownYet) {
            LogInfo << "end_of_line is LF";
            end_of_line_ = EndOfLine::LF;
        }
        return res;
    }

    LogInfo << "data is not empty, but image is empty";
    if (tried_clean) {
        LogError << "skip retry decoding and decode failed!";
        return std::nullopt;
    }

    LogInfo << "try to cvt lf";
    if (!clean_cr(buffer)) {
        LogError << "no `\\r\\n` found, skip retry decode";
        return std::nullopt;
    }

    res = decoder(buffer);

    if (!res) {
        LogError << "convert lf and retry decode failed!";
        return std::nullopt;
    }

    if (end_of_line_ == EndOfLine::UnknownYet) {
        LogInfo << "end_of_line is CRLF";
    }
    else {
        LogInfo << "end_of_line is changed to CRLF";
    }
    end_of_line_ = EndOfLine::CRLF;

    return res;
}

std::optional<cv::Mat> ScreencapBase::decode_raw(const std::string& buffer)
{
    if (buffer.size() < 8) {
        return std::nullopt;
    }

    auto data = static_cast<const uint8_t*>(static_cast<const void*>(buffer.c_str()));
    uint32_t im_width, im_height;
    memcpy(&im_width, data, 4);
    memcpy(&im_height, data + 4, 4);

    if (int(im_width) != width_ || int(im_height) != height_) {
        LogError << "screencap size image" << VAR(im_width) << VAR(im_height) << "don't match" << VAR(width_)
                 << VAR(height_);
        return std::nullopt;
    }

    auto size = 4ull * im_width * im_height;

    if (buffer.size() < size) {
        return std::nullopt;
    }

    auto header_size = buffer.size() - size;
    auto im_data = data + header_size;

    cv::Mat temp(height_, width_, CV_8UC4, const_cast<uint8_t*>(im_data));
    if (temp.empty()) {
        return std::nullopt;
    }

    const auto& br = *(temp.end<cv::Vec4b>() - 1);
    if (br[3] != 255) { // only check alpha
        return std::nullopt;
    }
    cv::cvtColor(temp, temp, cv::COLOR_RGBA2BGR);
    return temp.clone(); // temp只是引用data, 需要clone确保数据拥有所有权
}

std::optional<cv::Mat> ScreencapBase::decode_gzip(const std::string& buffer)
{
    auto buf = gzip::decompress(buffer.c_str(), buffer.size());
    return decode_raw(buf);
}

std::optional<cv::Mat> ScreencapBase::decode_png(const std::string& buffer)
{
    cv::Mat temp = cv::imdecode({ buffer.data(), int(buffer.size()) }, cv::IMREAD_COLOR);
    if (temp.empty()) {
        return std::nullopt;
    }

    return temp.clone();
}

bool ScreencapBase::clean_cr(std::string& buffer)
{
    if (buffer.size() < 2) {
        return false;
    }

    auto check = [](std::string::iterator it) { return *it == '\r' && *(it + 1) == '\n'; };

    auto scan = buffer.end();
    for (auto it = buffer.begin(); it != buffer.end() - 1; ++it) {
        if (check(it)) {
            scan = it;
            break;
        }
    }
    if (scan == buffer.end()) {
        return false;
    }

    // TODO: 应该可以优化为若干次copy+find, 效率应该会好一点, 但是没必要
    auto last = buffer.end() - 1;
    auto ptr = scan;
    while (++scan != last) {
        if (!check(scan)) {
            *ptr = *scan;
            ++ptr;
        }
    }
    *ptr = *last;
    ++ptr;
    buffer.erase(ptr, buffer.end());
    return true;
}

MAA_CTRL_UNIT_NS_END
