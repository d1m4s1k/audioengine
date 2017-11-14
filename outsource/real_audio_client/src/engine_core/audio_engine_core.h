#pragma once
#include <set>
#include <memory>
#include "device/include/audio_device.h"
#include "audio_processing.h"
#include "audio_mixer.h"
#include "audio_neteq.h"
#include "codec_conveter.h"
#include "base/time_cvt.hpp"
#include "audio_resample.h"
#include "audio_buffer.h"
namespace audio_engine
{


	class IAudioDataCallback
	{
	public:
		virtual void CaptureData( AudioBufferPtr buffer ){}//ԭʼ����
		virtual bool IsNeedProcess(){ return false; }//�Ƿ���ҪԤ����
		virtual void ProcessData( AudioBufferPtr buffer ){}//ǰ��Ԥ��������aec��ns��vad�ȵ�
		virtual bool IsNeedDefaultEncode(){ return false; }//�Ƿ���ҪĬ�ϱ�������opus
		virtual void BeforeEncodeData( AudioBufferPtr buffer ){}//δ�������ݣ���������Ҫ�Ŀ����д���
		virtual void PostEncodeData(AudioBufferPtr buffer){}//Ĭ�ϱ��������
		virtual bool BeforeDecodeData(){ return false; }//����ǰ
		virtual void AfterDecodeData( AudioBufferPtr buffer ){ }//�����
		virtual void MixerData( AudioBufferPtr buffer ){}//������
		virtual void PlayoutData( AudioBufferPtr buffer ){}//�͵��豸���ŵ�����
	};

	class AudioEngineCore:public AudioBufferProc
	{
	public:
		AudioEngineCore();
		~AudioEngineCore();
		void Initialize();
		void Terminate();
		void StartRecording();
		void StartPlayout();
		void StopRecording();
		void StopPlayout();
		void SetAudioDataCallback( IAudioDataCallback* cb );
		void RemoveAudioDataCallback( IAudioDataCallback* cb );
	protected:
		virtual void RecordingDataIsAvailable( const void* data, size_t size_in_byte );
		virtual size_t NeedMorePlayoutData( void* data, size_t size_in_byte );
		virtual void ErrorOccurred( AudioError /*aeCode*/ );
	private:
		AudioBufferPtr PreProcess(const void* data, size_t size_in_byte);
		void ProcessEncode();
	private:
		AudioDevice* _audio_device;
		AudioProcess _audio_process;
		AudioMixer   _audio_mixer;
		AudioNetEq   _audio_neteq;
		CodecConveter* _codec_conveter;
		std::set<IAudioDataCallback*> _audio_data_cb_list;
		AudioResample _rec_resampler;
	};
}