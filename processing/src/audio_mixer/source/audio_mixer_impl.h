#pragma once
#include <list>

#include "../include/audio_mixer.h"
class AudioMixerImpl :public AudioMixer
{
    struct MixerParticipantInfo
    {
        MixerParticipant*    participant = nullptr;     // ��������Դ
        webrtc::AudioFrame*  audioFrame = nullptr;       // ��ǰ����֡
        bool                 isMixed = false;            // ֮ǰ�Ƿ�������
        bool                 isSilent = true;            // �Ƿ���
        int32_t              needMixCount = 0;           // β���������ʣ��֡�������ﲢ������ֹͣ���������ǽ�������
        float                weightFactor = 1.0f;        // ����ϵ��
        uint32_t             energy = 0;

    };
    using MixerParticipantList = std::list < MixerParticipantInfo>;
public:
    AudioMixerImpl( int samplerate, int channel );
    ~AudioMixerImpl();
    virtual void Release() override; // �ͷ�һ������ʵ��
    virtual void AddParticipant( MixerParticipant *participant )override; // ���һ�������������
    virtual void RemoveParticipant( MixerParticipant *participant )override;// �Ƴ������������
    virtual bool GetMixerAudio( webrtc::AudioFrame* audioFrame ) override; // ���ϵ�����ȡ����
    void LimitParticipantCount( int32_t count )override;
protected:
    webrtc::AudioFrame* PopAudioFrame();
    void PushAudioFrame(webrtc::AudioFrame* pAudioFrame);
    uint32_t CalculateEnergy( const webrtc::AudioFrame* audioFrame );
    void MixFrameList( MixerParticipantList & mixlist, webrtc::AudioFrame* audioFrame);
private:
    MixerParticipantList m_participants;
    int32_t m_limitCount = 3;
    std::list<webrtc::AudioFrame*> m_audioFramePool;
    
};