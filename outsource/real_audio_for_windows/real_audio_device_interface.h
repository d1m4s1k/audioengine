#ifndef REAL_AUDIO_DEVICE_INTERFACE
#define REAL_AUDIO_DEVICE_INTERFACE
#pragma once
#include<stdint.h>

#ifdef REAL_AUDIO_EXPORTS
#define REAL_AUDIO_API __declspec(dllexport)

#else
#define REAL_AUDIO_EXPORTS __declspec(dllimport)

#endif


#define REAL_AUDIO_CALL __stdcall

extern "C"
{
    /*�ɼ��˻ص�����ָ��*/
    typedef void( REAL_AUDIO_CALL*LPRECORDINGDATACALLBACK )( const void* pcm16_data, int len_of_byte );
    typedef intptr_t DID;

    /*����/����һ�������豸*/
    REAL_AUDIO_API DID REAL_AUDIO_CALL CreateDevice();
    REAL_AUDIO_API void REAL_AUDIO_CALL DestroyDevice( DID device_id );


    /*����/��ȡ�ɼ��Ͳ��ŵĲ������Լ�����*/
    REAL_AUDIO_API void REAL_AUDIO_CALL SetSampleRate( DID device_id, int32_t rec_sample_rate, int16_t rec_channel, int32_t ply_sample_rate, int16_t ply_channel );
    REAL_AUDIO_API void REAL_AUDIO_CALL GetSampleRate( DID device_id, int32_t* rec_sample_rate, int16_t* rec_channel, int32_t* ply_sample_rate, int16_t* ply_channel );

    /*���òɼ�����Ƶ���ݵĽ��պ���*/
    REAL_AUDIO_API void REAL_AUDIO_CALL SetRecordingDataCallback( DID device_id, LPRECORDINGDATACALLBACK  cb );
    /*���Ŷ���Ƶ������亯��*/
    REAL_AUDIO_API void REAL_AUDIO_CALL FillPlayoutData( DID device_id, const void*pcm16_data, int len_of_byte );

    /*��ʼ/ֹͣ¼��*/
    REAL_AUDIO_API bool REAL_AUDIO_CALL StartRecording( DID device_id );
    REAL_AUDIO_API void REAL_AUDIO_CALL StopRecording( DID device_id );

    /*��ʼ/ֹͣ����*/
    REAL_AUDIO_API bool REAL_AUDIO_CALL StartPlayout( DID device_id );
    REAL_AUDIO_API void REAL_AUDIO_CALL StopPlayout( DID device_id );


    REAL_AUDIO_API void REAL_AUDIO_CALL SetFrameSize( DID device_id, int32_t len_of_byte );
    REAL_AUDIO_API int32_t REAL_AUDIO_CALL GetFrameSize( DID device_id );
}



#endif