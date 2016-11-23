#pragma once
/*!
 * \file voicetotext.h
 *
 * \author zhangnaigan
 * \date ʮ�� 2016
 *
 * \note ��װ�˿ƴ�Ѷ�ɵ�����ת�ı����ܣ����߰�
 */
#include <string>
#include <memory>
class SpeechToText
{
public:
    static SpeechToText* Create();
    virtual ~SpeechToText(){}
    virtual int Write( const char* audioSample, std::size_t nSamples ) = 0;
    virtual int GetResult(std::string &strText) = 0; // ���stringΪ�գ����ʾ�޷�ʶ��ԭ�������û�����������������ã���������̫ģ�������߸�����û˵��
    virtual bool Start() = 0;
    virtual void Cancel() = 0;
    virtual void Finish() = 0;
    virtual bool isStop() = 0;
};