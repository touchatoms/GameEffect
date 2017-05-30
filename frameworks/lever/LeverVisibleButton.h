//
//  LeverVisibleButton.h
//
//  Created by 皮明震 on 15/9/19.
//
//

#ifndef __LEVERVISIBLEBUTTON_BUTTON_H__
#define __LEVERVISIBLEBUTTON_BUTTON_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LeverVisibleButton : public cocos2d::ui::Button
{
public:
    LeverVisibleButton ();
    virtual ~LeverVisibleButton ();
    
    static LeverVisibleButton* create();
    static LeverVisibleButton* create(const std::string& normalImage,
                                   const std::string& selectedImage = "",
                                   const std::string& disableImage = "",
                                   cocos2d::ui::Widget::TextureResType texType = cocos2d::ui::Widget::TextureResType::LOCAL);
    virtual bool hitTest(const cocos2d::Vec2 &pt) override;
    
protected:
    virtual bool init() override;
    virtual bool init(const std::string& normalImage,
                      const std::string& selectedImage = "",
                      const std::string& disableImage = "",
                      cocos2d::ui::Widget::TextureResType texType = cocos2d::ui::Widget::TextureResType::LOCAL) override;
    void loadNormalTransparentInfo(const std::string& normalImage);
    bool getIsTransparentAtPoint(cocos2d::Vec2 point);
    
private:
    int normalImageWidth_;
    int normalImageHeight_;
    bool* normalTransparent_;
    
};


#endif /* __LEVERVISIBLEBUTTON_BUTTON_H__ */