#include "user_manager.h"

#include "server_module.h"
#include "socket_manager.h"
#include "server_config.h"

#include "user.h"

UserManager::UserManager()
    :_packet(nullptr)
{
    _socket_mgr = ServerModule::GetInstance()->GetSocketManager();
    _task = ServerModule::GetInstance()->GetAsyncTask();
}

UserManager::~UserManager()
{
    
}

void UserManager::Start()
{
    std::string ip;
    int16_t port;
    if ( ServerModule::GetInstance()->GetServerCnfig()->GetServer(1, ip, port) )
    {
        _socket_mgr->AsyncAccept( ip, port, std::bind( &UserManager::HandleAccept, this, std::placeholders::_1, std::placeholders::_2 ) );
    }
}

void UserManager::Stop()
{
    _stop = true;
    _lock.lock();
    for (auto & u: _users )
    {
        u->DettachTcp();
    }
    _lock.unlock();
}

void UserManager::HandleLogin( std::shared_ptr<User> user)
{
    _lock.lock();
    _users.push_back( user );
    _lock.unlock();
    auto pb = std::make_shared<audio_engine::RAUserMessage>();
    auto login_ntf = pb->mutable_login_notify();
    login_ntf->set_status( 1 );
    login_ntf->set_userid( user->userid() );
    login_ntf->set_username( user->username() );
    login_ntf->set_extend( user->extend() );
    login_ntf->set_devtype( audio_engine::DEVICE_LINUX );
    BufferPtr buf = _packet.Build( pb );
    if ( buf )
    {
        _lock.lock();
        for ( auto u : _users )
        {
            if ( u != user )
            {
                u->Send( 1, buf );
            }
        }
        _lock.unlock();
    }
}

void UserManager::HandleLogout( std::shared_ptr<User> user )
{
    _lock.lock();
    if (std::find(_users.begin(),_users.end(),user) == _users.end() )
    {
        _lock.unlock();
        return;
    }
    _users.remove( user );
    _lock.unlock();
    auto pb2 = std::make_shared<audio_engine::RAUserMessage>();
    auto login_ntf = pb2->mutable_login_notify();
    login_ntf->set_status( 0 );
    login_ntf->set_userid( user->userid() );
    login_ntf->set_username( user->username() );
    login_ntf->set_extend( user->extend() );
    BufferPtr buf = _packet.Build( pb2 );
    if ( buf )
    {
        _lock.lock();
        for ( auto u : _users )
        {
            if ( u != user )
            {
                u->Send( 1, buf );
            }
        }
        _lock.unlock();
    }
}

bool UserManager::HandleAccept( std::error_code ec, socket_t fd )
{
    if (ec == asio::error::basic_errors::bad_descriptor )
    {
        printf("accept error:%s\n",ec.message().c_str());
    }
    if ( !ec)
    {
        std::string ip;
        int16_t port;
        auto sm = ServerModule::GetInstance()->GetSocketManager();
        if ( sm->QuerySocketInfo( fd, ip, port ) )
        {
            printf( "�յ��ͻ��������ӣ�%s:%u\n",ip.c_str(), (uint16_t)port );
        }
        auto user = std::make_shared<User>(this);
        user->AttachTcp( fd );
    }
    if ( _stop )
    {
        return false;
    }
    return true;
}