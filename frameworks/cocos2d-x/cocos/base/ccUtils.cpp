/****************************************************************************
 Copyright (c) 2010      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "base/ccUtils.h"

#include <stdlib.h>

#include "base/CCDirector.h"
//#include "base/CCAsyncTaskPool.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCRenderer.h"
#include "platform/CCImage.h"
#include "platform/CCFileUtils.h"

NS_CC_BEGIN

int max(int t1, int t2)
{
    return t1>t2?t1:t2;
}

int min(int t1, int t2)
{
    return t1<t2?t1:t2;
}

int ccNextPOT(int x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >>16);
    return x + 1;
}

namespace utils
{
    
    unsigned char*  GaussianBlur(unsigned char* img,  unsigned int width, unsigned int height, unsigned int radius)
    {
        unsigned int channels = 4;
        radius = min(max(1, radius), 248);
        unsigned int kernelSize = 1 + radius * 2;
        unsigned int* kernel = (unsigned int*)malloc(kernelSize* sizeof(unsigned int));
        memset(kernel, 0, kernelSize* sizeof(unsigned int));
        int(*mult)[256] = (int(*)[256])malloc(kernelSize * 256 * sizeof(int));
        memset(mult, 0, kernelSize * 256 * sizeof(int));
        
        int xStart = 0;
        int yStart = 0;
        width = xStart + width - max(0, (xStart + width) - width);
        height = yStart + height - max(0, (yStart + height) - height);
        int imageSize = width*height;
        int widthstep = width*channels;
        if (channels == 3 || channels == 4)
        {
            unsigned char *    CacheImg = nullptr;
            CacheImg = (unsigned char *)malloc(sizeof(unsigned char) * imageSize * 6);
            if (CacheImg == nullptr) return NULL;
            unsigned char *    rCache = CacheImg;
            unsigned char *    gCache = CacheImg + imageSize;
            unsigned char *    bCache = CacheImg + imageSize * 2;
            unsigned char *    r2Cache = CacheImg + imageSize * 3;
            unsigned char *    g2Cache = CacheImg + imageSize * 4;
            unsigned char *    b2Cache = CacheImg + imageSize * 5;
            int sum = 0;
            for (int K = 1; K < radius; K++){
                unsigned int szi = radius - K;
                kernel[radius + K] = kernel[szi] = szi*szi;
                sum += kernel[szi] + kernel[szi];
                for (int j = 0; j < 256; j++){
                    mult[radius + K][j] = mult[szi][j] = kernel[szi] * j;
                }
            }
            kernel[radius] = radius*radius;
            sum += kernel[radius];
            for (int j = 0; j < 256; j++){
                mult[radius][j] = kernel[radius] * j;
            }
            for (int Y = 0; Y < height; ++Y) {
                unsigned char*     LinePS = img + Y*widthstep;
                unsigned char*     LinePR = rCache + Y*width;
                unsigned char*     LinePG = gCache + Y*width;
                unsigned char*     LinePB = bCache + Y*width;
                for (int X = 0; X < width; ++X) {
                    int     p2 = X*channels;
                    LinePR[X] = LinePS[p2];
                    LinePG[X] = LinePS[p2 + 1];
                    LinePB[X] = LinePS[p2 + 2];
                }
            }
            int kernelsum = 0;
            for (int K = 0; K < kernelSize; K++){
                kernelsum += kernel[K];
            }
            float fkernelsum = 1.0f / kernelsum;
            for (int Y = yStart; Y < height; Y++){
                int heightStep = Y * width;
                unsigned char*     LinePR = rCache + heightStep;
                unsigned char*     LinePG = gCache + heightStep;
                unsigned char*     LinePB = bCache + heightStep;
                for (int X = xStart; X < width; X++){
                    int cb = 0;
                    int cg = 0;
                    int cr = 0;
                    for (int K = 0; K < kernelSize; K++){
                        unsigned    int     readPos = ((X - radius + K + width) % width);
                        int * pmult = mult[K];
                        cr += pmult[LinePR[readPos]];
                        cg += pmult[LinePG[readPos]];
                        cb += pmult[LinePB[readPos]];
                    }
                    unsigned int p = heightStep + X;
                    r2Cache[p] = cr* fkernelsum;
                    g2Cache[p] = cg* fkernelsum;
                    b2Cache[p] = cb* fkernelsum;
                }
            }
            for (int X = xStart; X < width; X++){
                int WidthComp = X*channels;
                int WidthStep = width*channels;
                unsigned char*     LinePS = img + X*channels;
                unsigned char*     LinePR = r2Cache + X;
                unsigned char*     LinePG = g2Cache + X;
                unsigned char*     LinePB = b2Cache + X;
                for (int Y = yStart; Y < height; Y++){
                    int cb = 0;
                    int cg = 0;
                    int cr = 0;
                    for (int K = 0; K < kernelSize; K++){
                        unsigned int   readPos = ((Y - radius + K + height) % height) * width;
                        int * pmult = mult[K];
                        cr += pmult[LinePR[readPos]];
                        cg += pmult[LinePG[readPos]];
                        cb += pmult[LinePB[readPos]];
                    }
                    int    p = Y*WidthStep;
                    LinePS[p] = (unsigned char)(cr * fkernelsum);
                    LinePS[p + 1] = (unsigned char)(cg * fkernelsum);
                    LinePS[p + 2] = (unsigned char)(cb* fkernelsum);
                }
            }
            free(CacheImg);
            free(kernel);
            free(mult);
            return img;
        }
        free(kernel);
        free(mult);
        
        return img;
    }
    
    /**
     * Capture screen implementation, don't use it directly.
     */
    void onCaptureScreen(const std::function<void(bool, const std::string&)>& afterCaptured, const std::string& filename)
    {
        auto glView = Director::getInstance()->getOpenGLView();
        auto frameSize = glView->getFrameSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        frameSize = frameSize * glView->getFrameZoomFactor() * glView->getRetinaFactor();
#endif
        
        int width = static_cast<int>(frameSize.width);
        int height = static_cast<int>(frameSize.height);
        
        bool succeed = false;
        std::string outputFile = "";
        
        do
        {
            std::shared_ptr<GLubyte> buffer(new GLubyte[width * height * 4], [](GLubyte* p){ CC_SAFE_DELETE_ARRAY(p); });
            if (!buffer)
            {
                break;
            }
            
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
            // The frame buffer is always created with portrait orientation on WP8.
            // So if the current device orientation is landscape, we need to rotate the frame buffer.
            auto renderTargetSize = glView->getRenerTargetSize();
            CCASSERT(width * height == static_cast<int>(renderTargetSize.width * renderTargetSize.height), "The frame size is not matched");
            glReadPixels(0, 0, (int)renderTargetSize.width, (int)renderTargetSize.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#else
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#endif
            
            std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[width * height * 4], [](GLubyte* p) { CC_SAFE_DELETE_ARRAY(p); });
            if (!flippedBuffer)
            {
                break;
            }
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
            if (width == static_cast<int>(renderTargetSize.width))
            {
                // The current device orientation is portrait.
                for (int row = 0; row < height; ++row)
                {
                    memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
                }
            }
            else
            {
                // The current device orientation is landscape.
                for (int row = 0; row < width; ++row)
                {
                    for (int col = 0; col < height; ++col)
                    {
                        *(int*)(flippedBuffer.get() + (height - col - 1) * width * 4 + row * 4) = *(int*)(buffer.get() + row * height * 4 + col * 4);
                    }
                }
            }
#else
            for (int row = 0; row < height; ++row)
            {
                memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
            }
#endif
            
            std::shared_ptr<Image> image(new Image);
            if (image)
            {
                image->initWithRawData(flippedBuffer.get(), width * height * 4, width, height, 8);
                if (FileUtils::getInstance()->isAbsolutePath(filename))
                {
                    outputFile = filename;
                }
                else
                {
                    CCASSERT(filename.find("/") == std::string::npos, "The existence of a relative path is not guaranteed!");
                    outputFile = FileUtils::getInstance()->getWritablePath() + filename;
                }
                succeed = image->saveToFile(outputFile);
            }
        }while(0);
        
        if (afterCaptured)
        {
            afterCaptured(succeed, outputFile);
        }
    }
    /*
     * Capture screen interface
     */
    void captureScreen(const std::function<void(bool, const std::string&)>& afterCaptured, const std::string& filename)
    {
        static CustomCommand captureScreenCommand;
        captureScreenCommand.init(std::numeric_limits<float>::max());
        captureScreenCommand.func = std::bind(onCaptureScreen, afterCaptured, filename);
        Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
    }
    
    /**
     * Capture screen implementation, don't use it directly.
     */
    void onCaptureScreenBlurTexture(const std::function<void(bool, Texture2D *)>& afterCaptured, unsigned int gaussianBlurRadius, unsigned int scaleFactor)
    {
        static bool startedCapture = false;
        
        if (startedCapture)
        {
            CCLOG("Screen capture is already working");
            if (afterCaptured)
            {
                afterCaptured(false, NULL);
            }
            return;
        }
        else
        {
            startedCapture = true;
        }
        
        auto glView = Director::getInstance()->getOpenGLView();
        auto frameSize = glView->getFrameSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        frameSize = frameSize * glView->getFrameZoomFactor() * glView->getRetinaFactor();
#endif
        
        int width = static_cast<int>(frameSize.width);
        int height = static_cast<int>(frameSize.height);
        
        do
        {
            std::shared_ptr<GLubyte> buffer(new GLubyte[width * height * 4], [](GLubyte* p){ CC_SAFE_DELETE_ARRAY(p); });
            if (!buffer)
            {
                break;
            }
            
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
            
            int thumbWidth = width /scaleFactor;
            int thumbHeight = height /scaleFactor;
            std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[thumbWidth * thumbHeight * 4], [](GLubyte* p) { CC_SAFE_DELETE_ARRAY(p); });
            if (!flippedBuffer)
            {
                break;
            }
            
            for (int row = 0; row < height; row = row + scaleFactor)
            {
                for (int col = 0; col < width; col = col + scaleFactor)
                {
                    memcpy(flippedBuffer.get() + ((row / scaleFactor ) * thumbWidth + col / scaleFactor) * 4, (buffer.get() + ((height - row - 1) * width  + col) * 4), 4);
                }
            }
            
            Image* image = new (std::nothrow) Image;
            if (image)
            {
                GaussianBlur(flippedBuffer.get(), thumbWidth, thumbHeight, gaussianBlurRadius);
                image->initWithRawData(flippedBuffer.get(), thumbWidth * thumbHeight * 4, thumbWidth, thumbHeight, 8);
                
                Texture2D * texture = new Texture2D();
                texture->initWithImage(image);
                texture->autorelease();
                if (afterCaptured) {
                    afterCaptured(true, texture);
                }
                
                delete image;
                
                startedCapture = false;
            }
            else
            {
                CCLOG("Malloc Image memory failed!");
                if (afterCaptured)
                {
                    afterCaptured(false, NULL);
                }
                startedCapture = false;
            }
        } while (0);
    }
    
    /*
     * Capture screen interface
     */
    static EventListenerCustom* s_captureScreenListenerTexture;
    static CustomCommand s_captureScreenCommandTexture;
    void captureScreenBlurTexture(const std::function<void(bool, Texture2D *)>& afterCaptured, unsigned int gaussianBlurRadius, unsigned int scaleFactor)
    {
        if (s_captureScreenListenerTexture)
        {
            CCLOG("Warning: CaptureScreen has been called already, don't call more than once in one frame.");
            return;
        }
        s_captureScreenCommandTexture.init(std::numeric_limits<float>::max());
        s_captureScreenCommandTexture.func = std::bind(onCaptureScreenBlurTexture, afterCaptured, gaussianBlurRadius, scaleFactor);
        s_captureScreenListenerTexture = Director::getInstance()->getEventDispatcher()->addCustomEventListener(Director::EVENT_AFTER_DRAW, [](EventCustom *event) {
            auto director = Director::getInstance();
            director->getEventDispatcher()->removeEventListener((EventListener*)(s_captureScreenListenerTexture));
            s_captureScreenListenerTexture = nullptr;
            director->getRenderer()->addCommand(&s_captureScreenCommandTexture);
            director->getRenderer()->render();
        });
    }
    
    
    
    std::vector<Node*> findChildren(const Node &node, const std::string &name)
    {
        std::vector<Node*> vec;
        
        node.enumerateChildren(name, [&vec](Node* nodeFound) -> bool {
            vec.push_back(nodeFound);
            return false;
        });
        
        return vec;
    }
    
#define MAX_ITOA_BUFFER_SIZE 256
    double atof(const char* str)
    {
        if (str == nullptr)
        {
            return 0.0;
        }
        
        char buf[MAX_ITOA_BUFFER_SIZE];
        strncpy(buf, str, MAX_ITOA_BUFFER_SIZE);
        
        // strip string, only remain 7 numbers after '.'
        char* dot = strchr(buf, '.');
        if (dot != nullptr && dot - buf + 8 <  MAX_ITOA_BUFFER_SIZE)
        {
            dot[8] = '\0';
        }
        
        return ::atof(buf);
    }
    
    double gettime()
    {
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        
        return (double)tv.tv_sec + (double)tv.tv_usec/1000000;
    }
    
    Rect getCascadeBoundingBox(Node *node)
    {
        Rect cbb;
        Size contentSize = node->getContentSize();
        
        // check all childrens bounding box, get maximize box
        Node* child = nullptr;
        bool merge = false;
        for(auto object : node->getChildren())
        {
            child = dynamic_cast<Node*>(object);
            if (!child->isVisible()) continue;
            
            const Rect box = getCascadeBoundingBox(child);
            if (box.size.width <= 0 || box.size.height <= 0) continue;
            
            if (!merge)
            {
                cbb = box;
                merge = true;
            }
            else
            {
                cbb.merge(box);
            }
        }
        
        // merge content size
        if (contentSize.width > 0 && contentSize.height > 0)
        {
            const Rect box = RectApplyAffineTransform(Rect(0, 0, contentSize.width, contentSize.height), node->getNodeToWorldAffineTransform());
            if (!merge)
            {
                cbb = box;
            }
            else
            {
                cbb.merge(box);
            }
        }
        
        return cbb;
    }
    
}

NS_CC_END
