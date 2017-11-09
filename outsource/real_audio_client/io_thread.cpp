#include <iostream>
#include <string>
#include "message_queue.h"
#include <vector>
#include <regex>
#include <thread>
#include <fstream>
#include <atomic>
#include <iostream>
#include <sstream>
namespace audio_engine{
	std::mutex g_android_cmd_lock;
	std::list<std::string> android_cmd_list;
	using namespace std;
	void help();
	void version();

	std::vector<string> Tokenize( const string str, const std::regex regex )
	{
		using namespace std;

		std::vector<string> result;

		sregex_token_iterator it( str.begin(), str.end(), regex, -1 );
		sregex_token_iterator reg_end;

		for(; it != reg_end; ++it)
		{
			if(!it->str().empty()) //token could be empty:check
				result.emplace_back( it->str() );
		}

		return result;
	}
	std::vector<string> TokenizeDefault( const string str )
	{
		using namespace std;

		regex re( "[\\s,]+" );

		return Tokenize( str, re );
	}

	stMSG* SendCmd( std::vector<string>& vec_str )
	{
		bool bsuc = true;
		stMSG* msg = new stMSG;
		msg->cmd = CMD_INVALID;
		for(auto it = vec_str.begin(); it != vec_str.end(); it++)
		{
			std::string str = *it;
			if(str == "q" || str == "quit")
			{
				msg->cmd = CMD_IO_QUIT;
				break;
			}
			else if(str == "help" || str == "h")
			{
				help();
				vec_str.clear();
				break;
			}
			else if(str == "clear" || str == "c")
			{
				system( "cls" );
				vec_str.clear();
				break;
			}
			else if(str == "sleep")
			{
				int i = 1000;
				if(++it != vec_str.end())
				{
					i = atoi( it->c_str() );
				}
				std::this_thread::sleep_for( std::chrono::milliseconds( i ) );
				vec_str.clear();
				break;
			}
			else if(str == "nm")
			{
				msg->cmd = CMD_IO_NEW_CLIENT;
			}
			else if(str == "dm")
			{
				msg->cmd = CMD_IO_DELETE_CLIENT;
			}
			else if(str == "login")
			{
				msg->cmd = CMD_IO_LOGIN;
			}
			else if(str == "logout")
			{
				msg->cmd = CMD_IO_LOGOUT;
			}
			else if(str == "ul")
			{
				msg->cmd = CMD_IO_UL;
			}
			else if(str == "ls")
			{
				msg->cmd = CMD_IO_LS;
			}
			else if(str == "rs")
			{
				msg->cmd = CMD_IO_RS;
			}
			else if(str == "ps")
			{
				msg->cmd = CMD_IO_PS;
			}
			else if(str == "speak")
			{
				msg->cmd = CMD_IO_SPEAK;
			}
			else if(str == "play")
			{
				msg->cmd = CMD_IO_PLAY;
			}
			else if(str == "recmsg")
			{
				msg->cmd = CMD_IO_REC_MSG;
			}
			else if(str == "plymsg")
			{
				msg->cmd = CMD_IO_PLAY_MSG;
			}
			else if(str == "msglist")
			{
				msg->cmd = CMD_IO_MSG_LIST;
				msg->index = 0;
				msg->count = 0x20;
			}
			else if(str == "stt")
			{
				msg->cmd = CMD_IO_STT;
			}
			else if(str == "ts")
			{
				msg->cmd = CMD_IO_TS;
			}
			else if(str == "extend")
			{
				msg->cmd = CMD_IO_USER_EXTEND;
			}
			else if(str == "rattr")
			{
				msg->cmd = CMD_IO_ROOM_ATTR;
			}
			else if(str == "kickoff")
			{
				msg->cmd = CMD_IO_KICK_OFF;
			}
			else if(str == "block")
			{
				msg->cmd = CMD_IO_BLOCK;
			}
			else if(str == "disspeak")
			{
				msg->cmd = CMD_IO_DISABLE_SPEAK;
			}
			else if(str == "-uid")
			{
				++it;
				if(it == vec_str.end())
				{
					bsuc = false;
					break;
				}
				msg->uid = *it;
			}
			else if(str == "-mid")
			{
				it++;
				if(it == vec_str.end())
				{
					bsuc = false;
					break;
				}
				msg->mid = atoi( it->c_str() );
			}
			else if(str == "-rid")
			{
				it++;
				if(it == vec_str.end())
				{
					bsuc = false;
					break;
				}
				msg->rid = atoi( it->c_str() );
			}
			else if(str == "--stop")
			{
				msg->stop = true;
			}
			else if(str == "-msgid")
			{
				it++;
				if(it == vec_str.end())
				{
					break;
				}
				msg->msgid = *it;
			}
			else if(str == "-index")
			{
				it++;
				if(it == vec_str.end())
				{
					bsuc = false;
					break;
				}
				msg->index = (int)atoi( it->c_str() );
			}
			else if(str == "-count")
			{
				it++;
				if(it == vec_str.end())
				{
					bsuc = false;
					break;
				}
				msg->count = (int)atoi( it->c_str() );
			}
			else if(str == "--stt")
			{
				msg->stt = true;
			}
			else if(str == "-key")
			{
				it++;
				if(it == vec_str.end())
				{
					bsuc = false;
					break;
				}
				msg->key = *it;
			}
			else if(str == "-value")
			{
				it++;
				if(it == vec_str.end())
				{
					bsuc = false;
					break;
				}
				msg->value = *it;
			}
			else
			{
				bsuc = false;
			}
		}
		if(( msg->cmd == CMD_INVALID || !bsuc ) && !vec_str.empty())
		{
			if(msg->cmd == CMD_INVALID)
			{
				std::cout << "invalid cmd\n";
			}
			else
			{
				std::cout << "invalid agrument!!\n";
			}

			std::cout << "������ help(h) �鿴�����б�\n";
			delete msg;
			msg = nullptr;
		}
		return msg;
	}

	void io_console( MessageQueue *queue )
	{
		version();
		string strline;
		while(1)
		{
			bool bsuc = true;
			std::cout << ">>>";
			getline( cin, strline );
			if(strline.empty() || strline.c_str()[0] == '#')
			{
				continue;
			}
			std::vector<string> vec_str = TokenizeDefault( strline );

			stMSG* msg = SendCmd( vec_str );
			if(!msg)
			{
				continue;
			}
			queue->AddMessage( msg );
			if(msg->cmd == CMD_IO_QUIT)
			{
				return;
			}
		}

	}


	void io_file( MessageQueue*queue, std::string filename )
	{
		//    help();
		std::string strline;
		std::ifstream fin( filename, std::ios::in );
		char buf[1024] = { 0 };
		std::cout << ">>>";
		while(fin.getline( buf, 1024 ))
		{

			if(strlen( buf ) == 0 || buf[0] == '#')
			{
				continue;
			}
			std::cout << buf << "\n";
			bool bsuc = true;
			std::vector<string> vec_str = TokenizeDefault( buf );
			memset( buf, 0, sizeof( buf ) );
			stMSG* msg = SendCmd( vec_str );
			if(!msg)
			{
				continue;
			}
			queue->AddMessage( msg );
			if(msg->cmd == CMD_IO_QUIT)
			{
				return;
			}
			std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
		}
		std::this_thread::sleep_for( std::chrono::seconds( 5 ) );

		std::cout << "�ű����������û���˳����ԣ��Զ��������̨ģʽ\n";
		io_console( queue );

	}

	void set_android_cmd( std::string &cmd )
	{
		std::lock_guard<std::mutex> lg( g_android_cmd_lock );
		android_cmd_list.push_back( std::move( cmd ) );

	}

	std::string get_android_cmd()
	{
		std::lock_guard<std::mutex> lg( g_android_cmd_lock );
		if(android_cmd_list.empty())
		{
			return {};
		}
		else
		{
			auto cmd = android_cmd_list.front();
			android_cmd_list.pop_front();
			return cmd;
		}
		return {};
	}

	void io_android( MessageQueue *queue )
	{
		version();
		string strline;
		while(1)
		{
			bool bsuc = true;

			strline = get_android_cmd();
			if(strline.empty() || strline.c_str()[0] == '#')
			{
				continue;
			}
			logprint( "%s%s", "<<<", strline.c_str() );
			std::vector<string> vec_str = TokenizeDefault( strline );

			stMSG* msg = SendCmd( vec_str );
			if(!msg)
			{
				continue;
			}
			queue->AddMessage( msg );
			if(msg->cmd == CMD_IO_QUIT)
			{
				return;
			}
		}
	}

	void help()
	{
		COUT_REDIRECTION_BEGIN
			std::cout << "Version 0.1.0  "
			<< "Build " __DATE__ " " __TIME__ "\n"
			<< "��ӭʹ�ò��Գ���.\n\n"
			<< "ʹ��˵����\"[]\"��ʾ��ѡ��������-arg value����ʽ�������̿��� ����\"-mid 1\"��ʾ��������Ϊģ��1 --arg ���治��Ҫ����������ע������\n"
			<< "ʹ�����ӣ�\n\tnm #����һ��ģ��\n\tlogin #��½���䣬Ĭ��50�ŷ���\n\tspeak #��ʼ����\n\tlogout #�ǳ�����\n\tdm #ɾ��ģ��\n\tq #�˳�����\n"
			<< "�����б�\n"
			<< "help(h)                                   : ����\n"
			<< "clear(c)                                  : ��տ���̨\n"
			<< "quit(q)                                   : ��ȫ�˳ɽ���\n"
			<< "sleep [%d]                                : IO�߳�˯��һ�����Ĭ��1000ms,���Դ�һ������\n"
			<< "run -file                                 : ִ��һ���ļ��ű����ű�������һЩ����ĺϼ�����ʱ�������������"
			<< "nm [-uid]                                 : ����һ���µ�snail_audio_clientģ�� ����mid -uid��userid����ѡ��ûָ�����������\n"
			<< "dm [-mid]                                 : �ͷ���ģ�� -mid��ʾnm������mid\n" // 
			<< "login [-mid -uid -rid]                    : ��¼���� -rid��ʾ��½�ķ���ţ�Ĭ��Ϊ50\n"
			<< "logout [-mid ]                            : �ǳ�����\n"  // �ǳ�һ��mid
			<< "ul [-mid]                                 : ��ѯ�û��б�\n"
			<< "ls [-mid]                                 : ��ѯ��¼״̬\n"
			<< "rs [-mid]                                 : ��ѯ��ǰ�Ƿ�����¼��������Ϣ\n"
			<< "ps [-mid]                                 : ��ѯ��ǰ�Ƿ����ڲ���������Ϣ\n"
			<< "speak [-mid] --stop                       : �����Ƿ񽲻� --stop��ʾֹͣ����˼����ͬ\n"
			<< "play [-mid]  --stop                       : �����Ƿ񲥷�\n"
			<< "recmsg [-mid --stop --stt     ]           : ¼��������Ϣ --stt��ʾ¼�Ƶ�����ͬʱת���ı�\n"
			<< "plymsg -msgid [ --stop        ]           : ����������Ϣ\n"
			<< "msglist [-mid -index -count   ]           : ��ȡ������Ϣ�б� -index��ʾ��Ϣ�����ŵ���ʼ�㣬-count��ʾ��ȡ���б���\n"
			<< "stt -msgid [ -mid  --stop     ]           : ����ת�ı�����\n"
			<< "ts [ -mid --stop -uid         ]           : ���û�����״̬����һֱ��ӡ,Ĭ��ȫ���û�����\n"
			<< "extend [-mid] -uid -key -value"


			;
		COUT_REDIRECTION_END
	}

	void version()
	{
		COUT_REDIRECTION_BEGIN
			std::cout << "Version 0.1.0  "
			<< "Build " __DATE__ " " __TIME__ "\n"
			<< "��ӭʹ�ò��Գ���.\n\n"
			<< "ʹ��help(h)����鿴��ϸ�������\n"
			;
		COUT_REDIRECTION_END
	}
	std::string lastlog;
}