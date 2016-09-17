#pragma once
#include <cstdint>
#include "webrtc/modules/include/module_common_types.h"
class MixerParticipant
{
public:
    virtual bool GetAudioFrame( webrtc::AudioFrame* audioFrame ) = 0;
};

class AudioMixer
{
public:
    static AudioMixer* Create(int samplerate,int channel,int frame_size); // ����һ������ʵ��
    virtual void Release() = 0; // �ͷ�һ������ʵ��
    virtual void AddParticipant( MixerParticipant *participant ) = 0; // ���һ�������������
    virtual void RemoveParticipant( MixerParticipant *participant ) = 0;// �Ƴ������������
    virtual void LimitParticipantCount(int32_t count) = 0;              // ���ƻ���������
    virtual bool GetMixerAudio( webrtc::AudioFrame* audioFrame ) = 0; // ���ϵ�����ȡ����
};