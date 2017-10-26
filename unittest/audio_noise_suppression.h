#pragma once
#include <list>
#include <cstdint>
#include <complex>
#include "webrtc/common_audio/real_fourier.h"
#include "webrtc/modules/audio_processing/rms_level.h"
#include "findpeaks.h"
// ��ʱ50ms����

#include "fft_wrapper.h"
#include "webrtc_agc.h"
#include "audio_parse_param.h"
namespace snail{
    namespace audio{
        struct SignalStat
        {
            bool lowSignal = true;
            std::list<float> silent_levels;
            std::list<size_t> speech_levels;
            int hilevelcount = 0;
            int LowerSourceTTL=20;
            int speech_diff_level = 0;
            float coef = 0.01f;
            float min_height = 0.0f;
        };
        class AudioNoiseSuppression :public IParseParamNotify
        {
            typedef std::complex<float> Complex;
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
                int16_t data[480 * 2]; // ����֡����
                int size;            // ��ǰ֡����
                int level;           // �����ȼ���Խ���ʾ����ԽС
                int silent;          // �Ƿ��Ǿ��� 
            };


            struct tProcess
            {
                int NoSpeechCnt = 0;
                int NoiseCnt = 0;
                int ConsonantCnt = 0;
                int SilentCnt = 0;
            };

            struct NoiseCoef
            {
                float std;             // ��ֵ����ɢ��
                float max_freq;        // �������Ƶ�ʷ���
                float low_level;       // �ж��Ƿ������
                float silent_level;    // ����ƽ������
                float min_level;       // ��С����֡������
                int   num_peak;        // ��ǰ֡�ж��ٷ�ֵ��
                int   peak_height;     // Ѱ�ҷ�ֵ����͸߶�
                int   peak_distance;   // Ѱ�ҷ�ֵ����С����
            };

            AudioNoiseSuppression();
            ~AudioNoiseSuppression();
            void Init( int samplerate, int nChannel );
            VoiceFrame* Proceess( int16_t* data, size_t len, bool silent );
            void Reset();
            void enable( bool bEnable ) { m_bEnable = bEnable; }
            virtual void ParseParamNotify( const std::string& Param );
            bool ProcessSpec( std::valarray< std::complex<float> >&frec, std::valarray<float>& amplitide, std::valarray<float>& angle );
            bool ProcessMMSE( std::valarray<float>& sig );
            VoiceFrame* ProceessNS( int16_t* data, size_t size, int level );
            int AnalyzePeaks( std::valarray<float>& amplitide );
            void ProcessLowLevelSignal( int16_t* data, size_t len );
            float Std(const std::vector<std::pair<float,size_t>>& pks);
        private:
            std::list<int> m_future_list;
            std::list<VoiceFrame*> audio_cache;
            std::list<VoiceFrame*> audio_cache_free;
            WebrtcAgc m_agc;
            webrtc::RMSLevel m_rms;

            int m_recSamplerate = 16000;
            int m_recChannel = 1;
            int m_frame;


            tProcess m_process;


            double m_silent_Threshold = 0.01;
            bool  m_bEnable = true;
            bool  m_disableForRecordingMsg = false;


            FFTWrapper m_fft;
            std::valarray<float> m_noise;
            std::valarray<float> m_Xk_prev;
            int m_init_num = 0;

            bool m_bsilent = false;
            int m_noise_type = Speech;
            bool m_bEnableMMSE = false;
            float *m_energy = nullptr;
            FindPeaks m_find_peaks;
            int m_audio_type_list[24];
            FILE*m_file=nullptr;
            NoiseCoef m_noise_coef;
        };

    }
}
