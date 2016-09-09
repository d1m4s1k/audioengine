#pragma once
#include <list>
#include <cstdint>
#include <complex>
#include "webrtc/common_audio/real_fourier.h"
// ��ʱ50ms����


class AudioNoiseSuppression
{
public:
    enum NoiseType
    {
        Speech,       // ��������
        Consonant,    // ����
        FullHighFreq, // �޵�Ƶ���� 
        Noise,        // ����
        Silent,       // ����
    };
    struct VoiceFrame
    {
        int16_t data[480*2]; // ����֡����
        int size;            // ��ǰ֡����
        int level;           // �����ȼ���Խ���ʾ����ԽС
        int silent;          // �Ƿ��Ǿ���
    };

    AudioNoiseSuppression();
    ~AudioNoiseSuppression();
    void Init( int samplerate, int nChannel );
    int Analyze( int16_t * speech_frame, int size, bool isSilent );
    VoiceFrame* Proceess(int16_t* data , int len, int level, int silent, int noise_type );
    void Reset();
    float snr() { return (float)m_nNoise / m_nAudio; }
private:
    std::list<int> m_future_list;
    std::list<int> m_history_list;
    int m_last_nosie_type = -1;
    std::list<VoiceFrame*> audio_cache;
    std::list<int> m_history_check;

    typedef std::complex<float> Complex;
    Complex* m_pData;
    float* m_proc_buf;
    int16_t* m_noise_buf;
    int m_recSamplerate = 16000;
    int m_recChannel = 1;
    int m_frame;
    float *m_energy = nullptr;
    float* m_pWindows = nullptr;
    std::list<VoiceFrame*> audio_cache_free;
    int   m_nNoise = 0;// ����ͳ��
    int   m_nAudio = 1;   // ȫ����Ƶ���ݰ�
    rtc::scoped_ptr<webrtc::RealFourier> m_fft;
};