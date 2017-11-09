#include "master_control.h"
#include <chrono>

#include "error_code.h"
#include "server_module.h"
using namespace std::chrono;
namespace audio_engine{
	MasterControl::MasterControl()
		:_conn_mgr(this)
		, _timer(ServerModule::GetInstance()->GetTimerThread())
	{

    }
	MasterControl::~MasterControl()
	{
		
	}

	void MasterControl::Start()
	{
		_conn_mgr.StartListen();
		_timer.AddTask( 60s,std::bind(&MasterControl::CheckRoom,this) );
	}

	int MasterControl::JoinRoom( RAUserMessagePtr pb, UserConnPtr conn )
	{
		std::unique_lock<std::mutex> lock( _mutex );
		auto reqLogin = pb->request_login();
		auto roomkey = reqLogin.roomkey();
		auto it = _rooms.find(roomkey);
		if(it != _rooms.end())
		{
			if(it->second->FindMember(reqLogin.userid()))
			{
				// �Ѿ���һ��ͬ���û������档�������û����ܰ�ǰ����û�������
				return ERR_INVALID_USER_ID;
			}
			else
			{
				it->second->HandleConnection(conn);
			}
		}
		else
		{
			Room* room = new Room;
			_rooms[roomkey] = room;
			room->HandleConnection( conn );
		}
		return ERR_OK;
	}
	void MasterControl::CheckRoom()
	{
		std::unique_lock<std::mutex> lock( _mutex );
		for(auto it = _rooms.begin(); it != _rooms.end(); )
		{
			if(it->second->IsEmpty())
			{
				printf( "ɾ���շ���%s\n", it->first.c_str());
				delete it->second;
				it = _rooms.erase(it);
			}
			else
			{
				++it;
			}
		}

		_timer.AddTask( 60s, std::bind( &MasterControl::CheckRoom, this ) );
	}
}