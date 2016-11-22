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


    /*����/����һ�������豸*/
    REAL_AUDIO_API long REAL_AUDIO_CALL CreateDevice();
    REAL_AUDIO_API void REAL_AUDIO_CALL DestroyDevice( long device_id );


    /*����/��ȡ�ɼ��Ͳ��ŵĲ������Լ�����*/
    REAL_AUDIO_API void REAL_AUDIO_CALL SetSampleRate( long device_id, int32_t rec_sample_rate, int16_t rec_channel, int32_t ply_sample_rate, int16_t ply_channel );
    REAL_AUDIO_API void REAL_AUDIO_CALL GetSampleRate( long device_id, int32_t* rec_sample_rate, int16_t* rec_channel, int32_t* ply_sample_rate, int16_t* ply_channel );

    /*���òɼ�����Ƶ���ݵĽ��պ���*/
    REAL_AUDIO_API void REAL_AUDIO_CALL SetRecordingDataCallback( long device_id, LPRECORDINGDATACALLBACK  cb );
    /*���Ŷ���Ƶ������亯��*/
    REAL_AUDIO_API void REAL_AUDIO_CALL FillPlayoutData( long device_id, const void*pcm16_data, int len_of_byte );

    /*��ʼ/ֹͣ¼��*/
    REAL_AUDIO_API bool REAL_AUDIO_CALL StartRecording( long device_id );
    REAL_AUDIO_API void REAL_AUDIO_CALL StopRecording( long device_id );

    /*��ʼ/ֹͣ����*/
    REAL_AUDIO_API bool REAL_AUDIO_CALL StartPlayout( long device_id );
    REAL_AUDIO_API void REAL_AUDIO_CALL StopPlayout( long device_id );


    REAL_AUDIO_API void REAL_AUDIO_CALL SetFrameSize( long device_id,int32_t len_of_byte );
    REAL_AUDIO_API int32_t REAL_AUDIO_CALL GetFrameSize( long device_id );
}



#endif