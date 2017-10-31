#pragma once
#include <iostream>
#include <string>
#include "config.h"
//#include "code_convert.h"
namespace audio_engine{
	enum  CMD
	{
		CMD_INVALID,                 // ��Чָ��
		CMD_IO_NEW_CLIENT,           // �½�һ������ģ��
		CMD_IO_DELETE_CLIENT,        // ɾ��һ������ģ��
		CMD_IO_LOGIN,                // ��¼����
		CMD_IO_LOGOUT,               // �˳�����
		CMD_EVENT_CALLBACK,          // ģ����¼��ص�
		CMD_ASYNC_CALLBACK,          // ͬ�����߳��õĻص�
		CMD_IO_QUIT,                 // �˳��������Գ���
		CMD_IO_LS,                   // ��ѯ��¼״̬
		CMD_IO_UL,                   // ��ѯ�û��б�   
		CMD_IO_RS,
		CMD_IO_PS,
		CMD_IO_SPEAK,
		CMD_IO_PLAY,
		CMD_IO_REC_MSG,
		CMD_IO_PLAY_MSG,
		CMD_IO_MSG_LIST,
		CMD_IO_STT,                // ����ת�ı�����
		CMD_IO_TS,                 // �򿪸���˵��״̬
		CMD_IO_ECHO,
		CMD_IO_USER_ATTR,           /// �����û�����
		CMD_IO_ROOM_ATTR,          // ���÷�������
		CMD_IO_KICK_OFF,
		CMD_IO_BLOCK,
		CMD_IO_DISABLE_SPEAK,
	};

	struct stMSG
	{
		CMD cmd = CMD_INVALID;
		int mid = 1;
		int rid = 50;
		std::string uid;
		std::string msgid;
		std::string key;
		std::string value;
		bool stop = false;
		int index = 0;
		int count = 0;
		bool stt = false;
		void* id = nullptr;

	};


	extern std::string lastlog;


	extern std::mutex g_mutex;
	extern void android_print_log( std::string log );
	template <typename ... Args>
	void logprint( const char* format, Args const & ... args )
	{
		std::lock_guard<std::mutex> lock( g_mutex );
		char fmt_buf[4096] = { 0 };
		if(sizeof...( args ) == 0)
		{
			lastlog = format;
		}
		else
		{
			sprintf( fmt_buf, format, args... );
			lastlog = fmt_buf;
		}

#ifdef _ANDROID
		if(!lastlog.empty())
		{

			std::string logutf8 = snail::tools::GBKToUTF8( lastlog );

			logE( "before convert logutf8 len=%d,src=%s", logutf8.size(), logutf8.c_str() );

			/*����ת����Ĺ����Ƿ�����*/
			auto w = UTF8ToUnicode( logutf8 );
			UnicodeSimplifiedToTraditional( w );
			auto utf8 = UnicodeToUTF8( w );
			logE( "after convert len=%d,src=%s", utf8.size(), utf8.c_str() );
			/*����ת����Ĺ����Ƿ�����*/
			android_print_log( utf8 );

		}

		return;
#endif
		std::cout << lastlog;
	}

	void set_android_cmd( std::string &cmd );
	std::string get_android_cmd();

}