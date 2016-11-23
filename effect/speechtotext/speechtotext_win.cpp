
#ifdef _WIN32
#include <thread> 
#include <chrono>
#include "speechtotext_win.h"
#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"
#include "system/system.h"
#include "string_cvt.h"
#define HINTS_SIZE  100
const char* session_begin_params = "sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = gb2312";

SpeechToTextWin::SpeechToTextWin( std::string appid )
{
    aud_stat_ = MSP_AUDIO_SAMPLE_CONTINUE;		//��Ƶ״̬
    ep_stat_ = MSP_EP_LOOKING_FOR_SPEECH;		//�˵���
    rec_stat_ = MSP_REC_STATUS_SUCCESS;			//ʶ��״̬
    int	ret = -1;
    /* �û���¼ */
    std::string login_params = "appid="+appid+",work_dir=."; //��¼����,appid��msc���,��������Ķ�

    ret = MSPLogin( NULL, NULL, login_params.c_str() ); //��һ���������û������ڶ������������룬����NULL���ɣ������������ǵ�¼����
    if ( MSP_SUCCESS != ret )
    {
        logcat( "MSPLogin failed, error code: %d.\n", ret );
        return;
    }
}

SpeechToTextWin::~SpeechToTextWin()
{
    MSPLogout(); //�˳���¼
}

int  SpeechToTextWin::Write( const char* audioSample, std::size_t nSamples )
{
    aud_stat_ = MSP_AUDIO_SAMPLE_CONTINUE;		//��Ƶ״̬
    if ( count == 0 )
    {
        count += nSamples;
        aud_stat_ = MSP_AUDIO_SAMPLE_FIRST;
    }

    int ret = QISRAudioWrite( session_id_, (const void *)audioSample, nSamples, aud_stat_, &ep_stat_, &rec_stat_ );
    if ( MSP_REC_STATUS_SUCCESS == rec_stat_ )
    {
        // �Ѿ��в��ֽ��
        const char* rslt = QISRGetResult( session_id_, &rec_stat_, 0, &ret );
        if ( MSP_SUCCESS != ret )
        {
            return 1;
        }
        if ( rslt )
        {
            result_.append( rslt );
        }
    }
    if ( MSP_EP_AFTER_SPEECH == ep_stat_ )
    {
        return 0;
    }
    if ( MSP_SUCCESS != ret )
    {
        return -1;
    }
    printf( "write success!\n" );
    {
        return 1;
    }
}

bool SpeechToTextWin::Start()
{
    int ec = -1;
    session_id_ = QISRSessionBegin( nullptr, session_begin_params, &ec );
    if ( ec != MSP_SUCCESS || !session_id_ )
    {
        return false;
    }
    aud_stat_ = MSP_AUDIO_SAMPLE_FIRST;		//��Ƶ״̬
    return true;
}

void SpeechToTextWin::Finish()
{
    int ec = -1;
    ec = QISRAudioWrite( session_id_, NULL, 0, MSP_AUDIO_SAMPLE_LAST, &ep_stat_, &rec_stat_ );
    if ( MSP_SUCCESS != ec )
    {
        return;
    }
}

int SpeechToTextWin::GetResult( std::string& strText )
{
    int errcode = 1;
    int rec_stat = 0;
    char			hints[HINTS_SIZE] = { '\0' };               //hintsΪ�������λỰ��ԭ�����������û��Զ���
    if ( !session_id_ )
    {
        return -2;
    }
    const char *rslt = QISRGetResult( session_id_, &rec_stat, 0, &errcode );
    if ( MSP_SUCCESS != errcode )
    {
        QISRSessionEnd( session_id_, hints );
        session_id_ = nullptr;
        count = 0;
        return -1;
    }
    if ( NULL != rslt )
    {
        result_.append( rslt );
    }
    if ( MSP_REC_STATUS_COMPLETE == rec_stat )
    {
        QISRSessionEnd( session_id_, hints );
        session_id_ = nullptr;
        count = 0;
        strText = std::move( result_ );
        strText = mbstoutf8( strText ); //ȫ��תΪutf8��ʽ
        return 1;
    }
    else
    {
        strText = result_; // �ȷ��ز��ֽ��
        return 0;
    }
}

void SpeechToTextWin::Cancel()
{
    if (session_id_)
    {
        QISRSessionEnd( session_id_, "user cancel" );
    }
}


#endif//_WIN32