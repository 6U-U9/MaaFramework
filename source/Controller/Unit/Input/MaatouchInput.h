#pragma once

#include "InputBase.h"

#include "../Utils/InvokeApp.h"

MAA_CTRL_UNIT_NS_BEGIN

class MaatouchInput : public TouchInputBase, public KeyInputBase, public UnitBase
{
public:
    MaatouchInput() { children_.emplace_back(invoke_app_); }

    bool parse(const json::value& config) override;

    bool init(int swidth, int sheight) override;

    bool click(int x, int y) override;
    bool swipe(const std::vector<Step>& steps) override;
    bool press_key(int key) override;

private:
    void scale_point(int& x, int& y);

    std::shared_ptr<InvokeApp> invoke_app_ = std::make_shared<InvokeApp>();
    std::shared_ptr<IOHandler> shell_handler_ = nullptr;

    std::string root_;
    std::string package_name_;
    int width_ = 0;
    int height_ = 0;
    double xscale_ = 0;
    double yscale_ = 0;
    int press_ = 0;
};

MAA_CTRL_UNIT_NS_END