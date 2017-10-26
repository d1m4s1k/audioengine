#pragma once
#include <list>
#include "real_audio_common.h"
#include "protobuf_packet.h"
#include "user_service.h"
#include "base/timer.h"
#include "user_list.h"

//״̬ͼ
/*  LS_NONE----LS_CONNECTING-----LS_CONNECTED----LS_VERIFY_ACCOUNT
      \                                                           \
	   \													       \
		\												            LS_LOGINED
         \                                                         /
 		  \										                  /
           -----------------------LS_CONNECTED------LS_LOGOUT-----
*/														     
//������ʱ״̬����Ҫ�Ӷ�ʱ������ֵΪ�������������
enum LoginState{
    LS_NONE            =0,          // δ���ӷ�����
	LS_CONNECTING      = 1,         // �������ӷ�����
	LS_CONNECTED       = 2,         // �Ѿ����ӷ�����
	LS_VERIFY_ACCOUNT  = 3,         // ���ӳɹ�����֤�˺���Ч��
	LS_LOGINED         = 4,         // ��֤ͨ������½�������ꡣ
	LS_LOGOUT          = 5,         // ����ִ�еǳ�����
};


class UserEventHandler
{
public:
	virtual ~UserEventHandler(){}
	virtual void UpdateLoginState(LoginState state) = 0;
	virtual void UserEnterRoom(MemberPtr user) = 0;
	virtual void UserLeaveRoom(std::string user_id) = 0;
	virtual void UpdateUserState(std::string user_id, int state) = 0;
	virtual void UpdateUserExtend(std::string user_id, std::string extend) = 0;
};


class UserManager: ProtoPacketizer
{
public:
    UserManager( std::shared_ptr<UserService>  proto_packet = nullptr );
    ~UserManager();
public:
    void SetEventCallback(UserEventHandler* handler );
    int  Login(std::string userid,std::string roomkey);
	void Logout();
    int  GetCurState();
	int  GetTargetState();
	std::string GetUserID();
	std::string GetRoomKey();
private:
	void DoLogout();
	void ConnectServer();
	void DisConnectServer();
    void VerifyAccount();
	void OnTimer();
	void Transform(LoginState state);
public:
    virtual bool RecvPacket( std::shared_ptr<audio_engine::RAUserMessage> pb );
    virtual bool HandleError( int server_type, std::error_code ec );
    virtual bool HandleConnect( int server_type );
	void Update(LoginState state);
	UserEventHandler* _event_handle;
	UserServicePtr _user_service;
	STimerPtr   _timer;
	std::string  _user_id;
	std::string _roomkey;
	std::string _user_name;
	std::string _extend;
	int      _device_type;
	int64_t  _token;
	LoginState _cur_state = LS_NONE;
	LoginState _target_state = LS_NONE;
	uint64_t   _cur_state_time = 0;
};