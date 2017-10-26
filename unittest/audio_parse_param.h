#pragma once
/*!
 * \file audio_parse_param.h
 * \date 2016/06/01 13:28
 *
 * \author zhangnaigan
 * Contact: zhangng@snailgame.net
 *
 * \brief ����ģ�������������Щ����������������audio_effect�������ȿ��ƣ����еĲ�����Ӧ��ͨ�����������
 *
 * TODO: Ŀǰ���������Ƚ��٣������ܿ죬����ÿ���иı䣬����֪ͨ���й۲��ߣ�
         �۲��߻����±�����������ں��ڻᵼ������cpu�� ���Ժ��滹��Ҫ�Ż���
 *
 * \note �̰߳�ȫ
*/
#include <unordered_map>
#include <string>
#include <stdint.h>
#include <vector>
#include <mutex>
namespace snail {
    namespace audio {
        class IParseParamNotify;
        class AudioParseParameter
        {
        public:
            static AudioParseParameter& GetInstance();
          //  void Enable( bool bEnable );
            void RegisterNotify( IParseParamNotify* notify );
            void UnRegisterNotify( IParseParamNotify* notify );
            void SetParam( std::string strParam );
            bool GetValue( const std::string& type, int8_t ch, int32_t& value );
            bool GetValue( const std::string& type, int8_t ch, double& value );
            bool GetValue( const std::string& type, int8_t ch, int32_t& value, int32_t validStart, int32_t validEnd );//[validStart, validEnd)
            bool GetValue( const std::string& type, int8_t ch, double& value, double validStart, double validEnd );
            bool GetValue( const std::string& type, int8_t ch, std::string& value );
        private:
            AudioParseParameter();
            ~AudioParseParameter();
            int16_t Parse();
            void SkipBlank( std::string& strParam );
            void operator=( const AudioParseParameter& ) = delete;
            AudioParseParameter( const AudioParseParameter& ) = delete;
        private:
            typedef std::unordered_map<std::string, std::unordered_map<int8_t, std::string>> ParamMap;
            ParamMap m_mapParam;
            bool m_bEnable;
            std::string m_strParam;
            std::vector<IParseParamNotify*> m_vecNotify;
            std::mutex m_lock;
            std::mutex m_lockNotify;
        };

        class IParseParamNotify
        {
        public:
            virtual ~IParseParamNotify() { AudioParseParameter::GetInstance().UnRegisterNotify( this ); }
            virtual void ParseParamNotify( const std::string& Param ) = 0;
        };

    }
}