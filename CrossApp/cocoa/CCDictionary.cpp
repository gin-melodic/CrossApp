/****************************************************************************
 Copyright (c) 2012 - 2013 CrossApp.org
 
 http://9miao.com
 
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

#include "CCDictionary.h"
#include "CCString.h"
#include "platform/CCFileUtils.h"

using namespace std;

NS_CC_BEGIN

// -----------------------------------------------------------------------
// CCDictElement

CCDictElement::CCDictElement(const char* pszKey, CAObject* pObject)
{
    CCAssert(pszKey && strlen(pszKey) > 0, "Invalid key value.");
    m_iKey = 0;
    const char* pStart = pszKey;
    
    int len = strlen(pszKey);
    if (len > MAX_KEY_LEN )
    {
        char* pEnd = (char*)&pszKey[len-1];
        pStart = pEnd - (MAX_KEY_LEN-1);
    }
    
    strcpy(m_szKey, pStart);
    
    m_pObject = pObject;
    memset(&hh, 0, sizeof(hh));
}

CCDictElement::CCDictElement(intptr_t iKey, CAObject* pObject)
{
    m_szKey[0] = '\0';
    m_iKey = iKey;
    m_pObject = pObject;
    memset(&hh, 0, sizeof(hh));
}

CCDictElement::~CCDictElement()
{

}

// -----------------------------------------------------------------------
// CCDictionary

CCDictionary::CCDictionary()
: m_pElements(NULL)
, m_eDictType(kCCDictUnknown)
{

}

CCDictionary::~CCDictionary()
{
    removeAllObjects();
}

unsigned int CCDictionary::count()
{
    return HASH_COUNT(m_pElements);
}

CCArray* CCDictionary::allKeys()
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;

    CCArray* pArray = CCArray::createWithCapacity(iKeyCount);

    CCDictElement *pElement, *tmp;
    if (m_eDictType == kCCDictStr)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            CCString* pOneKey = new CCString(pElement->m_szKey);
            pArray->addObject(pOneKey);
            CC_SAFE_RELEASE(pOneKey);
        }
    }
    else if (m_eDictType == kCCDictInt)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            CCString* pOneKey = CCString::createWithFormat("%d", (int)pElement->m_iKey);
            pArray->addObject(pOneKey);
        }
    }
    
    return pArray;
}

CCArray* CCDictionary::allKeysForObject(CAObject* object)
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;
    CCArray* pArray = CCArray::create();

    CCDictElement *pElement, *tmp;

    if (m_eDictType == kCCDictStr)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            if (object == pElement->m_pObject)
            {
                CCString* pOneKey = new CCString(pElement->m_szKey);
                pArray->addObject(pOneKey);
                CC_SAFE_RELEASE(pOneKey);
            }
        }
    }
    else if (m_eDictType == kCCDictInt)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            if (object == pElement->m_pObject)
            {
                CCString* pOneKey = CCString::createWithFormat("%d", (int)pElement->m_iKey);
                pArray->addObject(pOneKey);
            }
        }
    }
    return pArray;
}

CAObject* CCDictionary::objectForKey(const std::string& key)
{
    // if dictionary wasn't initialized, return NULL directly.
    if (m_eDictType == kCCDictUnknown) return NULL;
    // CCDictionary only supports one kind of key, string or integer.
    // This method uses string as key, therefore we should make sure that the key type of this CCDictionary is string.
    CCAssert(m_eDictType == kCCDictStr, "this dictionary does not use string as key.");

    CAObject* pRetObject = NULL;
    CCDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement != NULL)
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

CAObject* CCDictionary::objectForKey(intptr_t key)
{
    // if dictionary wasn't initialized, return NULL directly.
    if (m_eDictType == kCCDictUnknown) return NULL;
    // CCDictionary only supports one kind of key, string or integer.
    // This method uses integer as key, therefore we should make sure that the key type of this CCDictionary is integer.
    CCAssert(m_eDictType == kCCDictInt, "this dictionary does not use integer as key.");

    CAObject* pRetObject = NULL;
    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    if (pElement != NULL)
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

const CCString* CCDictionary::valueForKey(const std::string& key)
{
    CCString* pStr = dynamic_cast<CCString*>(objectForKey(key));
    if (pStr == NULL)
    {
        pStr = CCString::create("");
    }
    return pStr;
}

const CCString* CCDictionary::valueForKey(intptr_t key)
{
    CCString* pStr = dynamic_cast<CCString*>(objectForKey(key));
    if (pStr == NULL)
    {
        pStr = CCString::create("");
    }
    return pStr;
}

void CCDictionary::setObject(CAObject* pObject, const std::string& key)
{
    CCAssert(key.length() > 0 && pObject != NULL, "Invalid Argument!");
    if (m_eDictType == kCCDictUnknown)
    {
        m_eDictType = kCCDictStr;
    }

    CCAssert(m_eDictType == kCCDictStr, "this dictionary doesn't use string as key.");

    CCDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
        CAObject* pTmpObj = pElement->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pElement);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }
}

void CCDictionary::setObject(CAObject* pObject, intptr_t key)
{
    CCAssert(pObject != NULL, "Invalid Argument!");
    if (m_eDictType == kCCDictUnknown)
    {
        m_eDictType = kCCDictInt;
    }

    CCAssert(m_eDictType == kCCDictInt, "this dictionary doesn't use integer as key.");

    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    if (pElement == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
        CAObject* pTmpObj = pElement->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pElement);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }

}

void CCDictionary::removeObjectForKey(const std::string& key)
{
    if (m_eDictType == kCCDictUnknown)
    {
        return;
    }
    
    CCAssert(m_eDictType == kCCDictStr, "this dictionary doesn't use string as its key");
    CCAssert(key.length() > 0, "Invalid Argument!");
    CCDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    removeObjectForElememt(pElement);
}

void CCDictionary::removeObjectForKey(intptr_t key)
{
    if (m_eDictType == kCCDictUnknown)
    {
        return;
    }
    
    CCAssert(m_eDictType == kCCDictInt, "this dictionary doesn't use integer as its key");
    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    removeObjectForElememt(pElement);
}

void CCDictionary::setObjectUnSafe(CAObject* pObject, const std::string& key)
{
    pObject->retain();
    CCDictElement* pElement = new CCDictElement(key.c_str(), pObject);
    HASH_ADD_STR(m_pElements, m_szKey, pElement);
}

void CCDictionary::setObjectUnSafe(CAObject* pObject, const intptr_t key)
{
    pObject->retain();
    CCDictElement* pElement = new CCDictElement(key, pObject);
    HASH_ADD_PTR(m_pElements, m_iKey, pElement);
}

void CCDictionary::removeObjectsForKeys(CCArray* pKeyArray)
{
    CAObject* pObj = NULL;
    CCARRAY_FOREACH(pKeyArray, pObj)
    {
        CCString* pStr = (CCString*)pObj;
        removeObjectForKey(pStr->getCString());
    }
}

void CCDictionary::removeObjectForElememt(CCDictElement* pElement)
{
    if (pElement != NULL)
    {
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        CC_SAFE_DELETE(pElement);
    }
}

void CCDictionary::removeAllObjects()
{
    CCDictElement *pElement, *tmp;
    HASH_ITER(hh, m_pElements, pElement, tmp) 
    {
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        CC_SAFE_DELETE(pElement);

    }
}

CAObject* CCDictionary::copyWithZone(CAZone* pZone)
{
    CCAssert(pZone == NULL, "CCDictionary should not be inherited.");
    CCDictionary* pNewDict = new CCDictionary();

    CCDictElement* pElement = NULL;
    CAObject* pTmpObj = NULL;

    if (m_eDictType == kCCDictInt)
    {
        CCDICT_FOREACH(this, pElement)
        {
            pTmpObj = pElement->getObject()->copy();
            pNewDict->setObject(pTmpObj, pElement->getIntKey());
            pTmpObj->release();
        }
    }
    else if (m_eDictType == kCCDictStr)
    {
        CCDICT_FOREACH(this, pElement)
        {
            pTmpObj = pElement->getObject()->copy();
            pNewDict->setObject(pTmpObj, pElement->getStrKey());
            pTmpObj->release();
        }
    }

    return pNewDict;
}

CAObject* CCDictionary::randomObject()
{
    if (m_eDictType == kCCDictUnknown)
    {
        return NULL;
    }
    
    CAObject* key = allKeys()->randomObject();
    
    if (m_eDictType == kCCDictInt)
    {
        return objectForKey(((CCString*)key)->getCString());
    }
    else if (m_eDictType == kCCDictStr)
    {
        return objectForKey(((CCString*)key)->getCString());
    }
    else
    {
        return NULL;
    }
}

CCDictionary* CCDictionary::create()
{
    CCDictionary* pRet = new CCDictionary();
    if (pRet != NULL)
    {
        pRet->autorelease();
    }
    return pRet;
}

CCDictionary* CCDictionary::createWithDictionary(CCDictionary* srcDict)
{
    CCDictionary* pNewDict = (CCDictionary*)srcDict->copy();
    pNewDict->autorelease();
    return pNewDict;
}

CCDictionary* CCDictionary::createWithContentsOfFileThreadSafe(const char *pFileName)
{
    return CCFileUtils::sharedFileUtils()->createCCDictionaryWithContentsOfFile(pFileName);
}

CCDictionary* CCDictionary::createWithContentsOfFile(const char *pFileName)
{
    CCDictionary* pRet = createWithContentsOfFileThreadSafe(pFileName);
    pRet->autorelease();
    return pRet;
}

bool CCDictionary::writeToFile(const char *fullPath)
{
    return CCFileUtils::sharedFileUtils()->writeToFile(this, fullPath);
}


NS_CC_END
