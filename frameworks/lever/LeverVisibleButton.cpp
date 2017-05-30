//
//  LeverVisibleButton.cpp
//  WhereWood
//
//  Created by 皮明震 on 15/9/19.
//
//

#include "LeverVisibleButton.h"

USING_NS_CC;
using namespace ui;

LeverVisibleButton::LeverVisibleButton():
Button(),
normalTransparent_(nullptr)
{}

LeverVisibleButton::~LeverVisibleButton()
{
    delete[] normalTransparent_;
}

LeverVisibleButton* LeverVisibleButton::create()
{
    LeverVisibleButton* widget = new LeverVisibleButton();
    if (widget && widget->init()) {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

LeverVisibleButton* LeverVisibleButton::create(const std::string& normalImage,
                                         const std::string& selectedImage,
                                         const std::string& disableImage,
                                         TextureResType texType)
{
    LeverVisibleButton* btn = new LeverVisibleButton;
    if (btn && btn->init(normalImage, selectedImage, disableImage, texType)) {
        btn->autorelease();
        return btn;
    }
    CC_SAFE_DELETE(btn);
    return nullptr;
}

bool LeverVisibleButton::init(const std::string &normalImage,
                           const std::string& selectedImage ,
                           const std::string& disableImage,
                           TextureResType texType)
{
    bool ret = true;
    do {
        if (!Button::init(normalImage, selectedImage, disableImage, texType)) {
            ret = false;
            break;
        }
    } while (0);
    loadNormalTransparentInfo(normalImage);
    return ret;
}

bool LeverVisibleButton::init()
{
    if (Button::init())
    {
        return true;
    }
    
    return false;
}

void LeverVisibleButton::loadNormalTransparentInfo(const std::string& normalImage_str)
{
    Image* normalImage = new Image();
    normalImage->initWithImageFile(normalImage_str);
    normalImageWidth_ = normalImage->getWidth();
    normalImageHeight_ = normalImage->getHeight();
    
    auto dataLen = normalImage->getDataLen();
    if (normalTransparent_ != nullptr) {
        delete[] normalTransparent_;
    }
    auto normalPixels = normalImage->getData();
    normalTransparent_ = new bool[dataLen / (sizeof(unsigned char) * 4)];
    for (auto i = 0; i < normalImageHeight_; i++) {
        for (auto j = 0; j < normalImageWidth_; j++) {
            normalTransparent_[i * normalImageWidth_ + j] = (normalPixels[(i * normalImageWidth_ + j) * 4] == 0);
        }
    }
    
    delete normalImage;
}

// get visible of the point
bool LeverVisibleButton::getIsTransparentAtPoint(cocos2d::Vec2 point)
{
    point.y = _buttonNormalRenderer->getContentSize().height - point.y;
    int x = (int) point.x - 1;
    if (x < 0) {
        x = 0;
    } else if (x >= normalImageWidth_) {
        x = normalImageWidth_ - 1;
    }
    int y = (int) point.y - 1;
    if (y < 0) {
        y = 0;
    } else if (y >= normalImageHeight_) {
        y = normalImageHeight_ - 1;
    }
    return normalTransparent_[normalImageWidth_ * y + x];
}

//overrride the button hit test with visibility check
bool LeverVisibleButton::hitTest(const Vec2 &pt)
{
    Vec2 localLocation = _buttonNormalRenderer->convertToNodeSpace(pt);
    Rect validTouchedRect;
    validTouchedRect.size = _buttonNormalRenderer->getContentSize();
    if (validTouchedRect.containsPoint(localLocation) && getIsTransparentAtPoint(localLocation) == false)
    {
        return true;
    }
    return false;
}