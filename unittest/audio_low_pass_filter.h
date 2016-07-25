#pragma once
/*!
 * \file audio_low_pass_filter.h
 * \date 2016/06/01 13:31
 *
 * \author zhangnaigan
 * Contact: zhangng@snailgame.net
 *
 * \brief ��ͨ�˲�����һ�㲻ʹ�ã��������Ч���ܲ��ã����Կ������������ʵ�ͬʱ���������ܴ󲿷ֵ�����
 *
 * TODO: Ŀǰ��ͨ��matlab���ɲ���������ɸ�����Ҫͨ��cpp���ע�ʹ����޸ġ�
 *
 * \note
*/
#include "stdint.h"
#include "webrtc/common_audio/fir_filter.h"

#define PROCESSING_LENGTH 480
class LowPassFilter
{
public:
    const static int kFFTSize = 160;
    const static int kWindowSize = 256;
    const static int kFilterHz = 64;
    LowPassFilter();
    ~LowPassFilter();
public:
    void Enable( bool bEnable );
    bool IsEnable()const;
    void SetSampleRate(int samplerate);
    void Processing( const int16_t* in, int16_t* out, size_t len );
private:
    webrtc::FIRFilter* m_filter;
    bool m_bEnable;
    float* m_fin;
    float* m_fout;
    float* m_pFilterCocc;

};