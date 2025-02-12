/****************************************************************************
 Copyright (c) 2013 9miao.com
 
 http://www.9miao.com
 
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

#ifndef __DICTIONARYHELPER_H__
#define __DICTIONARYHELPER_H__

#include "CrossAppExt.h"
#include "ExtensionMacros.h"

#define DICTOOL DictionaryHelper::shareHelper()

NS_CC_EXT_BEGIN
/**
*   @js NA
*   @lua NA
*/
class DictionaryHelper
{
public:
    DictionaryHelper();
    ~DictionaryHelper();
    static DictionaryHelper* shareHelper();
	static void purgeDictionaryHelper();
    CrossApp::CCDictionary* getSubDictionary(CrossApp::CCDictionary* root,const char* key);
    int getIntValue(CrossApp::CCDictionary* root,const char* key);
    float getFloatValue(CrossApp::CCDictionary* root,const char* key);
    const char* getStringValue(CrossApp::CCDictionary* root,const char* key);
    bool getBooleanValue(CrossApp::CCDictionary* root,const char* key);
    CrossApp::CCArray* getArrayValue(CrossApp::CCDictionary* root,const char* key);
    CrossApp::CAObject* checkObjectExist(CrossApp::CCDictionary* root,const char* key);
    int objectToIntValue(CrossApp::CAObject* obj);
    float objectToFloatValue(CrossApp::CAObject* obj);
    const char* objectToStringValue(CrossApp::CAObject* obj);
    bool objectToBooleanValue(CrossApp::CAObject* obj);
    CrossApp::CCArray* objectToCCArray(CrossApp::CAObject* obj);
    
    cs::CSJsonDictionary* getSubDictionary_json(cs::CSJsonDictionary* root,const char* key);
    int getIntValue_json(cs::CSJsonDictionary* root,const char* key);
    float getFloatValue_json(cs::CSJsonDictionary* root,const char* key);
    const char* getStringValue_json(cs::CSJsonDictionary* root,const char* key);
    bool getBooleanValue_json(cs::CSJsonDictionary* root,const char* key);
    int getArrayCount_json(cs::CSJsonDictionary* root,const char* key);
    int getIntValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx);
    float getFloatValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx);
    bool getBoolValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx);
    const char* getStringValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx);
    cs::CSJsonDictionary* getDictionaryFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx);
    bool checkObjectExist_json(cs::CSJsonDictionary* root,const char* key);
};

NS_CC_EXT_END

#endif /* defined(__CocoGUI__DictionaryHelper__) */
