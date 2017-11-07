#include "master_control.h"
#include "error_code.h"
namespace audio_engine{
	MasterControl::MasterControl()
		:_conn_mgr(this)
	{

    }
	MasterControl::~MasterControl()
	{

	}

	void MasterControl::Start()
	{
		_conn_mgr.StartListen();
	}

	int MasterControl::JoinRoom( RAUserMessagePtr pb, UserConnPtr conn )
	{
		auto reqLogin = pb->request_login();
		auto roomkey = reqLogin.roomkey();
		auto it = _rooms.find(roomkey);
		if(it != _rooms.end())
		{
			if(it->second.FindMember(reqLogin.userid()))
			{
				// �Ѿ���һ��ͬ���û������档
			}
			else
			{
				it->second.HandleConnection(conn);
			}
		}
		return ERR_OK;
	}
}