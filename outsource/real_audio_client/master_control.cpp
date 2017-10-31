#include "master_control.h"
#include "base/log.h"
namespace audio_engine{
	static audio_engine::Logger Log;
	MasterControl::MasterControl()
	{
		Log.setLevel( audio_engine::LEVEL_VERBOSE );
	}


	MasterControl::~MasterControl()
	{
	}

	void MasterControl::Initialize()
	{
		_user_mgr.SetEventCallback( this );
	}

	void MasterControl::Terminate()
	{
		_user_mgr.SetEventCallback( nullptr );
	}

	void MasterControl::UpdateLoginState( LoginState state )
	{
		// ֻ�ϱ������������½�ɹ���ʧ�ܣ��ǳ��ɹ��������ڲ�Ҫ��ӡ����Ĳ���
		if(state == LS_LOGINED && _user_mgr.GetTargetState() == LS_LOGINED)
		{
			_event_handler->RespondLogin( _user_mgr.GetRoomKey().c_str(), _user_mgr.GetUserID().c_str(), 0 );
		}
		else if(state == LS_NONE && _user_mgr.GetTargetState() == LS_NONE)
		{
			_event_handler->RespondLogout( _user_mgr.GetRoomKey().c_str(), _user_mgr.GetUserID().c_str(), 0 );
		}
		else if(_user_mgr.GetTargetState() == LS_LOGINED && state == LS_NONE)
		{
			_event_handler->RespondLogin( _user_mgr.GetRoomKey().c_str(), _user_mgr.GetUserID().c_str(), -2 );
		}
	}

	void MasterControl::UserEnterRoom( MemberPtr user )
	{
		_room_member_list.Add( user );
	}

	void MasterControl::UserLeaveRoom( int64_t token )
	{
		_room_member_list.Remove( token );
	}

	void MasterControl::UpdateUserState( int64_t src_token, int64_t dst_token, int state, int ec )
	{
		if(_room_member_list.Update( dst_token, state ))
		{
			//���ﴦ������ģ���߼�
			if(src_token == _user_mgr.GetToken())
			{
				//˵�����Լ������ģ�֪ͨ�ϲ㡣
			}
			else
			{

			}
		}
	}

	void MasterControl::UpdateUserExtend( int64_t token, std::string extend, int ec )
	{
		Log.d( "MasterControl::UpdateUserExtend:%s\n", extend.c_str() );
		if(_room_member_list.Update( token, extend ))
		{
			//���ﴦ������ģ���߼�
			if(token == _user_mgr.GetToken())
			{
				//˵�����Լ����õģ�֪ͨ�ϲ㡣
			}
			else
			{

			}
		}
	}

	void MasterControl::UpdateUserList( const std::vector<MemberPtr>& users )
	{
		_room_member_list.UpdateList( users );
	}

	void MasterControl::RegisterEventHandler( IAsyncEventHandler * handler )
	{
		_event_handler = handler;
	}

	void MasterControl::Login( std::string roomkey, std::string uid )
	{
		_user_mgr.Login( roomkey, uid );
	}

	void MasterControl::Logout()
	{
		_user_mgr.Logout();
	}

	int MasterControl::GetLoginState()
	{
		int state = _user_mgr.GetCurState();
		if(state == LS_LOGINED)
		{
			return 1;
		}
		else if(state == LS_NONE)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}

	void MasterControl::SetUserExtend( std::string extend )
	{
		_user_mgr.SetUserExtend( extend );
	}
}