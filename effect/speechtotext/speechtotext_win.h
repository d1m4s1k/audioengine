/*!
 * \file speechtotext_win.h
 *
 * \author zhangnaigan
 * \date ʮ�� 2016
 *
 * ʵ��windows�汾������ת���ֹ���
 */
#pragma once
#ifdef _WIN32
#include <string>
#include "speechtotext.h"

class SpeechToTextWin:public SpeechToText
{
public:
    SpeechToTextWin(std::string appid);
    virtual ~SpeechToTextWin();
    int Write( const char* audioSample, std::size_t nSamples );
    int GetResult( std::string &strText ); // ���stringΪ�գ����ʾ�޷�ʶ��ԭ�������û�����������������ã���������̫ģ�������߸�����û˵��
    bool Start();
    void Cancel();
    void Finish();
    bool isStop() { return false; }
private:

    std::string grammar_id_;
    const char* session_id_ = nullptr;
    int 			aud_stat_;		//��Ƶ״̬
    int 			ep_stat_;		//�˵���
    int 			rec_stat_;		//ʶ��״̬	
    int count = 0;
    std::string result_;
};

#endif//_WIN32