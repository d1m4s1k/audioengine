#pragma once
#include <list>
#include "real_audio_common.h"
#include "protobuf_packet.h"
#include "user_service.h"
#include "base/timer.h"
#include "user_list.h"
typedef std::function<void( std::string userid,int login_result )> LoginHandle;


//״̬ͼ
/*  LS_NONE----LS_CONNECTING-----LS_CONNECTED----LS_VERIFY_ACCOUNT
      \                                                           \
	   \													       \
		\												            LS_LOGINED
         \                                                         /
 		  \										                  /
           -----------------------LS_CONNECTED------LS_LOGOUT-----
*/														     

enum LoginState{
    LS_NONE,          // δ���ӷ�����
	LS_CONNECTING,    // �������ӷ�����
	LS_CONNECTED,     // �Ѿ����ӷ�����
	LS_VERIFY_ACCOUNT,// ���ӳɹ�����֤�˺���Ч��
	LS_LOGOUT,        // ���ڵǳ�
	LS_LOGINED,       // ��֤ͨ������½�������ꡣ
};


struct UserInfo
{

    std::string  user_id;
    std::string user_name;
    std::string extend;
	int device_type;
    int64_t  token;
};

class UserManager: ProtoPacketizer
{
public:
    UserManager( std::shared_ptr<UserService>  proto_packet = nullptr );
    ~UserManager();
public:
    void SetEventCallback( LoginHandle handle );
    int  Login(std::string userid);
	void Logout();
    int  GetLoginState();
private:
	void DoLogout();
	void ConnectServer();
	void DisConnectServer();
    void VerifyAccount();
	void OnTimer();
	void Transform();
public:
    virtual bool RecvPacket( std::shared_ptr<audio_engine::RAUserMessage> pb );
    virtual bool HandleError( int server_type, std::error_code ec );
    virtual bool HandleConnect( int server_type );
	void Update();
    LoginHandle _login_handle;
    std::shared_ptr<UserService> _user_service;
    UserInfo _user_info;
    bool _connect_server = false;
	STimerPtr   _timer;
	int        _server_status = 0;
	LoginState _cur_state = LS_NONE;
	LoginState _target_state = LS_NONE;
	UserList _user_list;
};