#pragma once
/*!
 * \file backward_redundant_frame.h
 *
 * \author zhangnaigan
 * \date ���� 2016
 *
 * ����Я��������Ϣ֡������ǰ�����ŵ�֡Я����ǰ��N��֡,����֮���ز�����24000Hz,����Ƶ�����
 */
#include <cstdint>
//#include "macrodef.h"
#include "webrtc/common_audio/resampler/include/resampler.h"
class BackwardRedunantFrame
{
    static const int NUM_FRAME = 4;             // ��֡��һ
    static const int OUT_SAMPLE_RATE = 48000;   // ��������Ĳ����ʣ���AAC����
    static const int BASE_SAMPLE_RATE = 24000;  // ��������24kHz������������Ƶ�����֮��ǡ����24kHz,AAC��Ҫ��֤�����ܴﵽ16kHz��Ƶ����Ϣ��������Ϣ�ᱻ�ض�

    struct BRFrame 
    {
        int       noise_type;                      // ��������
        int16_t   data[BASE_SAMPLE_RATE/100];      //240,˫����
    };

public:
    BackwardRedunantFrame();
    ~BackwardRedunantFrame();
    bool Init(int inSamplerate);
    void Process( int16_t* data, int nSamples, int16_t*outData, int &outSample );
private:
    BRFrame* brFrames[NUM_FRAME];
    webrtc::Resampler resample24000_;
};