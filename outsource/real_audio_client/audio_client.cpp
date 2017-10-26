#include <iostream>
#include <cassert>
#include <ctime>
#include "audio_client.h"
#include <functional>
#include "cmd.h"

AudioClient::AudioClient(int mid )
{
    _mid = mid;

    int ec = CreateAudioRoom( &_room );
    _room->RegisterEventHandler( this, false );
     _message    = (IMessageModule*)_room->GetModule( MESSAGE_MODULE );
     _real_audio = (IRealAudioModule*) _room->GetModule( REAL_ADUIO_MODULE );
     _mic_order  = (IMicOrderModule*)  _room->GetModule( MIC_ORDER_MODULE );
     _user       = (IUserModule*)_room->GetModule( USER_MODULE );
     _volume     = (IVolumeModule*)_room->GetModule(VOLUME_MODULE);
    _message->RegisterEventHandler( this );
    _real_audio->RegisterEventHandler( this );
    _mic_order->RegisterEventHandler( this );
    _user->RegisterEventHandler( this );
}

AudioClient::~AudioClient()
{
    _room->Logout();
    _room->Release( true );
}

void AudioClient::Login( int roomid,std::string uid )
{
    if ( uid.empty() )
    {
        std::srand( (unsigned int)std::time( 0 ) ); // use current time as seed for random generator
        int random_variable = std::rand();
        uid = std::to_string( random_variable );
    }
    _userid = uid;
   _room->Login( std::to_string( roomid ).c_str(), _userid.c_str() );
}

void AudioClient::Logout()
{
    _room->Logout();
}

void AudioClient::GetUserList()
{
    logprint( "\n%.20s\t%.20s\t%.20s\t%.20s\n\n", "userid", "extend", "����", "blocked" );
    UserListPtr userlist;
    if ( _user->GetUserList( userlist ) == 0 )
    {
        int count = userlist->size();
        for ( int i = 0; i < count; i++ )
        {
            auto user = ( *userlist )[i];
            auto ext = user->extends();
            logprint( "%s\n", ext );
            logprint( "%d. %.20s\t%.20s\t%.20s\t%.20s\n\n", i, user->userid(), "", user->IsDisableSpeak() ? "��" : "��", user->IsBlocked() ? "��" : "��" );
        }
        logprint( "���乲�У�%d���û�\n", count );
    }
}

int AudioClient::GetLoginedStatus( )
{
    return _room->GetLoginStatus();
}

bool AudioClient::IsRecordingMsg()
{
    return _message->IsRecordingAudioMsg();
}

bool AudioClient::IsPlayingMsg()
{
    int ret = _message->IsPlayingAudioMsg();
    return ret > 0;
}

void AudioClient::EnableSpeak( bool enable )
{
    _real_audio->EnableSpeak( enable );
}

void AudioClient::EnablePlay( bool enable )
{
    _real_audio->EnablePlayout( enable );
}


void AudioClient::RecordMsg(bool bStop, bool stt)
{
    if (bStop)
    {
        _message->StopRecord();
    }
    else
    {
        _message->StartRecord( stt );
    }
}

void AudioClient::PlayMsg( const char* msgid, bool bStop )
{
    if (bStop)
    {
        _message->StopPlayout();
    }
    else
    {
        _message->StartPlayout( msgid );
    }
}

void AudioClient::GetMsgList( int index, int count )
{
    _message->GetHistoryMsgList( index, count );
}

void AudioClient::StartSTT( const char* msgid )
{
    _message->StartSpeechToText( msgid );
}


void AudioClient::CancelSTT()
{
    _message->StopSpeechToText( false );
}

bool AudioClient::IsSTT()
{
    return _message->IsSpeechToTextNow();
}

const char* AudioClient::GetUserid()
{
    return _userid.c_str();
}

void AudioClient::TurnOnSpeakingStatus( bool status,std::string uid )
{
    _turn_on_speak = status;
    _turn_on_speak_uid = uid;
}


void AudioClient::SetRoomAttr( std::string key, std::string value )
{
    _room->SetRoomAttr( key.c_str(), value.c_str() );
}

void AudioClient::SetUserAttr( std::string uid, std::string key, std::string value )
{
    _user->SetUserAttr( uid.c_str(), key.c_str(), value.c_str() );
}

//////////////////////////����handler �첽�¼�///////////////////
void AudioClient::RespondLogin( const char* roomkey, UID uid, int ec )
{
    if ( ec == 0)
    {
        logprint( "[%d] ��½�ɹ��� ����id:%s,�û�id:%s\n",_mid, roomkey, uid  );
    }
    else
    {
        logprint( "[%d]��½ʧ��, ����id:%s,�û�id:%s,������:%d\n", _mid, roomkey, uid, ec );
    }
}

void AudioClient::NotifyReConnected( const char* roomkey, UID uid )
{
    logprint("[%d]���µ�½������\n",_mid);
}

void AudioClient::RespondLogout( const char* roomkey, UID uid, int ec )
{
    if ( ec == 0 )
    {
        logprint( "[%d] �ǳ�����ɹ�. ����id:%s,�û�id:%s\n", _mid, roomkey, uid );
    }
    else
    {
        logprint( "[%d]�ȳ�����ʧ�ܡ�����id:%s,�û�id:%s,������:%d\n", _mid, roomkey, uid, ec );
    }
}

void AudioClient::NotifyUserEnterRoom( UID uid )
{
    logprint( "[%d]�û���%s������\n", _mid,uid );
}

void AudioClient::NotifyUserLeaveRoom( UID uid )
{
    logprint( "[%d]�û���%s������\n", _mid, uid );
}


void AudioClient::NotifyUserSpeaking( UID uid, int volume )
{
    if ( _turn_on_speak )
    {
        logprint( "[%d]�û�(%s) ���ڽ���(%d)...\n", _mid, uid, volume );
    }

}

void AudioClient::NotifyRoomClose( const char* roomkey, int reason )
{
    logprint( "[%d]���䣨%s���ر�,ԭ���ǣ�%d\n",_mid,roomkey,reason );
}

void AudioClient::NotifyAudioMsgRecordEnd( const char* url, int ec )
{
    if (ec == 0)
    {
        std::string strEvent = "¼���ɹ� msgid:";
        strEvent += url;
        _message->SendMsg( 2, (char*)url, strlen( (char*)url ), nullptr, nullptr );
        const char* text = nullptr;
        ec = _message->GetTextOfSpeech( url,&text );
        if ( text )
//            logprint( "[%d]ת�ı����:%s\n", _mid, snail::tools::UTF8ToGBK( text ).c_str() );

            logprint( "[%d]%s\n", _mid, strEvent.c_str() );
    }
    else
    {
        logprint( "[%d]¼��ʧ�ܣ������룺%d\n",_mid, ec );
    }
}

void AudioClient::NotifyRecvMsg( const Message* msg )
{
    std::string strEvent = "�յ�������Ϣ��";
    if ( msg )
    {
        strEvent += msg->content();
        if ( msg->extends() && strlen( msg->extends() ) )
        {
            strEvent += "\n��չ��Ϣ��";
            strEvent += msg->extends();
        }
    }
    logprint("[%d]%s\n",_mid, strEvent.c_str() );
}

void AudioClient::RespondGetHitoryMsgList( int ec, const MessageList* msglist )
{
    int count = msglist->size();
    std::string strEvent = "�յ�һ��������Ϣ(";
    strEvent += std::to_string( count ) + "��)\n";
    strEvent += "\t����\t\t����\t\t\n";
    for ( int i = 0; i < count; i++ )
    {
        Message* msg = (*msglist)[i];
        std::string strMsgType;

        if ( msg->msgtype() == msg_text )
        {
            strMsgType = "�ı���Ϣ";
        }
        else
        {
            strMsgType = "������Ϣ";
        }

		std::string content = msg->content();// snail::tools::UTF8ToGBK(msg->content());
        strEvent = strEvent + std::to_string( msg->msgid() ) + "\t" + strMsgType + "\t" + content + "\t\n";
    }
    logprint("[%d]%s\n",_mid,strEvent.c_str());
}

void AudioClient::RespondSendMsg( int msg_type, const char* data, int length, const char* extend, UID to_user, int msg_id, int ec )
{
    if ( ec == 0)
    {
        logprint( "[%d]��Ϣ���ͳɹ�\n", _mid );
    }
    else
    {
        logprint( "[%d]��Ϣ����ʧ��,�����룺%d\n",_mid,ec );
    }
}


void AudioClient::NotifyConnectionLost()
{
    logprint( "[%d]���粻ͨ\n",_mid );
}

void AudioClient::NotifyRoomAttrChanged( const char* name, const char* value )
{
    logprint( "[%d]�������Ա仯��name:%s,value:%s\n", _mid,name, value );
}

void AudioClient::NotifyUserAttrChanged( UID uid, const char* name, const char* value )
{
    logprint( "[%d]�û�(%s)���Ա仯��name:%s,value:%s\n",_mid,uid,name,value );
}

void AudioClient::NotifyAudioMsgRecordBegin()
{
    logprint( "[%d]��ʼ¼����...\n",_mid );
}

void AudioClient::NotifyAudioMsgPlayBegin( const char* url )
{
    logprint( "[%d]������Ϣ��%s����ʼ������...\n", _mid, url );
}

void AudioClient::NotifyAudioMsgPlayEnd( const char* url, int ec )
{
    logprint( "[%d]������Ϣ��%s���������,�����룺%d\n",_mid,url,ec );
}

void AudioClient::RespondSpeechToText( const char* url, const char* text, int ec )
{
    if ( ec == 0 )
    {
//        logprint( "[%d]����ת�ı��ɹ���url:%s��ת�������%s\n", _mid, url, snail::tools::UTF8ToGBK( text ).c_str() );
    }
    else
    {
        logprint( "[%d]����ת�ı�ʧ��,�����룺%d\n",_mid, ec );
    }
}

void AudioClient::RespondKickOff( UID uid, int ec )
{
    if (ec == 0)
    {
        logprint( "[%d]�߳��û���%s���ɹ�\n",_mid,uid );
    }
    else
    {
        logprint( "[%d]�߳��û���%s��ʧ��,�����룺%d\n", _mid, uid, ec );
    }
}

void AudioClient::NotifyKiceOff( UID uid )
{
    logprint( "[%d]�û���%s�����߳�����\n",_mid,uid );
}

void AudioClient::NotifyDuplicateLogined()
{
    logprint( "[%d]�˺��������ط���½\n",_mid );
}

void AudioClient::RespondDisableSpeaking( UID uid, bool disable, int ec )
{
    if ( ec == 0 )
    {
        if ( disable )
        {
            logprint( "[%d]�����û���%s���ɹ�\n", _mid, uid );
        }
        else
        {
            logprint( "[%d]ȡ�������û���%s���ɹ�\n", _mid, uid );
        }
    }
    else
    {
        if ( disable )
        {
            logprint( "[%d]�����û���%s��ʧ��,�����룺%d\n", _mid, uid,ec );
        }
        else
        {
            logprint( "[%d]ȡ�������û���%s��ʧ��,�����룺%d\n", _mid, uid,ec );
        }
    }
}

void AudioClient::NotifyDisableSpeaking( UID uid, bool disable )
{
    if (disable)
    {
        logprint( "[%d]�û�(%s)������\n",_mid,uid );
    }
    else
    {
        logprint( "[%d]�û�(%s)���������\n",_mid,uid );
    }
}

void AudioClient::RespondSetRoomAttr( const char* name, const char* value, int ec )
{
    if ( ec == 0)
    {
        logprint( "[%d]���÷�������(name:%s,value:%s)�ɹ�\n",_mid,name,value );
    }
    else
    {
        logprint( "[%d]���÷�������(name:%s,value:%s)ʧ�ܣ������룺%d\n", _mid,name,value,ec );
    }

}

void AudioClient::RespondSetUserAttr( UID uid, const char* name, const char* value, int ec )
{
    if ( ec ==0 )
    {
        logprint( "[%d]�����û���%s������(name:%s,value:%s)�ɹ�\n",_mid,uid,name,value );
    }
    else
    {
        logprint( "[%d]�����û���%s������(name:%s,value:%s)ʧ�ܣ������룺%d\n", _mid, uid, name, value,ec );
    }
}

void AudioClient::RespondBlockUser( UID uid, bool block, int ec )
{
    if ( ec == 0 )
    {
        if (block )
        {
            logprint( "[%d]�����û���%s���ɹ�\n", _mid, uid );
        }
        else
        {
            logprint( "[%d]ȡ�������û���%s���ɹ�\n", _mid, uid );
        }
    }
    else
    {
        if (block)
        {
            logprint( "[%d]�����û���%s��ʧ�ܣ������룺%d\n", _mid, uid,ec );
        }
        else
        {
            logprint( "[%d]ȡ�������û���%s��ʧ�ܣ������룺%d\n", _mid, uid, ec );
        }

    }
}

void AudioClient::GetUserAttr( std::string uid, std::string key )
{
    if ( key.empty())
    {
        key = "ext";
    }
    UserPtr ptr;
    _user->GetUser( uid.c_str(),ptr );
    if (ptr)
    {
        auto str = ptr->attr( key.c_str() );
        if (str)
        {
            logprint( "[%d]%s\n", _mid, str );
        }
    }
}

void AudioClient::GetRoomAttr( std::string key )
{
    StringPtr ptr;
    _room->GetRoomAttr( key.c_str(), ptr );
    if (ptr)
    {
        logprint( "[%d]%s\n", ptr->c_str() );
    }
}

void AudioClient::KickOff( std::string uid )
{
    _user->KickOff( uid.c_str() );
}

void AudioClient::BlockUser( std::string uid, bool enable )
{
    _real_audio->BlockUser( uid.c_str(), enable );
}

void AudioClient::DisableSpeak( std::string uid, bool enable )
{
    _real_audio->DisableSpeaking( uid.c_str(), enable );
}


