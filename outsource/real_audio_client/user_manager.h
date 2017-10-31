#pragma once
#include <list>
#include <mutex>
#include <atomic>
#include "real_audio_common.h"
#include "protobuf_packet.h"
#include "user_service.h"
#include "base/timer.h"
#include "user_list.h"

//״̬ͼ
/*             LS_CONNECTING-----LS_CONNECTED-----LS_VERIFY_ACCOUNT
               /                                          \
	   		  /										       \
LS_NONE-----LS_INIT	                     			       LS_LOGINED
              \                                           /
 		       \									     /
               ---------LS_CONNECTED-------------LS_LOGOUT
*/														     
//������ʱ״̬����Ҫ�Ӷ�ʱ������ֵΪ�������������
namespace audio_engine{
	enum LoginState{
		LS_NONE = 0,
		LS_INIT = 2,         // δ���ӷ�����
		LS_CONNECTING = 3,         // �������ӷ�����
		LS_CONNECTED = 4,         // �Ѿ����ӷ�����
		LS_VERIFY_ACCOUNT = 5,         // ���ӳɹ�����֤�˺���Ч��
		LS_LOGINED = 6,         // ��֤ͨ������½�������ꡣ
		LS_LOGOUT = 7,         // ����ִ�еǳ�����

	};

#define MAX_TRY_LOGIN 5

	class UserEventHandler
	{
	public:
		virtual ~UserEventHandler(){}
		virtual void UpdateLoginState( LoginState state ) = 0;
		virtual void UserEnterRoom( MemberPtr user ) = 0;
		virtual void UserLeaveRoom( int64_t token ) = 0;
		virtual void UpdateUserState( int64_t src_token, int64_t dst_token, int state, int ec ) = 0;
		virtual void UpdateUserExtend( int64_t token, std::string extend, int ec ) = 0;
		virtual void UpdateUserList( const std::vector<MemberPtr>&users ) = 0;
	};


	class UserManager : ProtoPacketizer
	{
	public:
		UserManager( std::shared_ptr<UserService>  proto_packet = nullptr );
		~UserManager();
	public:
		void SetEventCallback( UserEventHandler* handler );
		int  Login( std::string roomkey, std::string userid );
		void Logout();
		int  GetCurState();
		int  GetTargetState();
		std::string GetUserID();
		std::string GetRoomKey();
		int64_t GetToken();
		void SetUserExtend( std::string& extend );
		void SetUserState( int64_t dst_token, int state );
	private:
		void DoLogout();
		void ConnectServer();
		void DisConnectServer();
		void VerifyAccount();
		void Transform( LoginState state );
	public:
		virtual bool RecvPacket( std::shared_ptr<RAUserMessage> pb );
		virtual bool HandleError( std::error_code ec );
		virtual bool HandleConnect();
		void Update( LoginState state );
		UserEventHandler* _event_handle;
		UserServicePtr _user_service;
		AsyncTask*    _task = nullptr;
		STimerPtr   _timer;
		std::string  _user_id;
		std::string _roomkey;
		std::string _extend;
		audio_engine::DEVICE_TYPE _device_type = audio_engine::DEVICE_WINDOWS;
		int  _user_state = audio_engine::STATE_PLAYOUT;
		int64_t  _token;
		LoginState _cur_state = LS_NONE;
		std::atomic<LoginState> _target_state_internel = LS_NONE;
		LoginState _target_state = LS_NONE;
		int   _cur_state_time = 0;
		uint32_t _try_login_count = 0;
		int _error_code = 0;
		std::vector<MemberPtr> _cache_userlist;
		std::recursive_mutex _mutex;
	};
}