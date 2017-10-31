//#include "media_base_client/imedia_base_client.h"
#include "SnailAudioEngineHelper.h"
#include "audio_typedef.h"
//#include "system/media_error_code.h"

namespace audio_engine{
        ErrorCodeDesc g_error_code_zh_cn[ERROR_CODE_COUNT] =
        {
            { ERR_OK, "�ɹ�" },
            { ERR_GENERIC, "����" },
            { ERR_NOT_INITIALIZE, "δ��ɳ�ʼ��" },
            { ERR_INVALID_ARGUMENT, "��������" },
            { ERR_INVALID_AUTHO_KEY, "��Ч����Ȩ��" },
            { ERR_INVALID_ROOM_KEY, "��Ч�ķ����" },
            { ERR_INVALID_USER_ID, "��Ч���û���" },
            { ERR_NOT_SUPPORTED, "���ܲ�֧��" },
            { ERR_NOT_VERSION_SUPPORTED, "�汾��֧��" },
            { ERR_NOT_LOGINED, "û�е�½" },
            { ERR_SERVER_CONNECT_FAILED, "����������ʧ��" },
            { ERR_TIME_OUT, "��ʱ" },
            { ERR_ALREADY_IN_USE, "�ù�������ʹ���У������ظ�����" },
            { ERR_USER_NOT_FOUND, "�޷��ҵ��û���" },
            { ERR_MSG_NOT_FOUND, "�޷��ҵ���Ϣ" },
            { ERR_ROOM_NOT_FOUND, "�޷��Ҳ�������" },
            { ERR_ADM_NO_FOUND_SPEAKER, "�޷��ҵ�������" },
            { ERR_ADM_NO_FOUND_MICPHONE, "�޷��ҵ���˷�" },
            { ERR_ADM_OPEN_SPEAKER_FAILED, "�޷���������" },
            { ERR_ADM_OPEN_MICPHONE_FAILED, "�޷�����˷�" },
            { ERR_ADM_NO_VALID_DATA, "û������" },
            { ERR_ADM_NO_RECORD_PERMISSION, "û��¼��Ȩ��" },
            { ERR_ADM_TIME_TOO_SHORT, "����ʱ��̫��" },
            { ERR_ADM_TIME_TOO_LONG, "����ʱ��̫��" },
            { ERR_NETWORK_POOL, "������������" },
            { ERR_NETWORK_BROKEN, "���粻ͨ" },
            { ERR_NET_FILE_UPLOAD_FAILED, "�ϴ��ļ�ʧ��" },
            { ERR_NET_FILE_DOWNLOAD_FAILED, "�����ļ�ʧ��" },
            { ERR_FILE_NOT_FOUND, "�޷��ҵ��ļ�" },
            { ERR_FILE_OPEN_FAILED, "�޷����ļ�" },
        };

        ErrorCodeDesc g_error_code_en_us[ERROR_CODE_COUNT] =
        {
            { ERR_OK, "success" },
            { ERR_GENERIC, "generic error" },
            { ERR_NOT_INITIALIZE, "not initialize" },
            { ERR_INVALID_ARGUMENT, "invalid argument" },
            { ERR_INVALID_AUTHO_KEY, "invalid authorization code" },
            { ERR_INVALID_ROOM_KEY, "invalid room key" },
            { ERR_INVALID_USER_ID, "invalid user id" },
            { ERR_NOT_SUPPORTED, " the function is not supported" },
            { ERR_NOT_VERSION_SUPPORTED, "the version of sdk is not supported" },
            { ERR_NOT_LOGINED, "the user is not logined" },
            { ERR_SERVER_CONNECT_FAILED, "connect server failed" },
            { ERR_TIME_OUT, "the operator is time out" },
            { ERR_ALREADY_IN_USE, "the function is already in use" },
            { ERR_USER_NOT_FOUND, "the user id is not found" },
            { ERR_MSG_NOT_FOUND, "the msg id is not found" },
            { ERR_ROOM_NOT_FOUND, "the room not found" },
            { ERR_ADM_NO_FOUND_SPEAKER, "the speaker is not found" },
            { ERR_ADM_NO_FOUND_MICPHONE, "the microphone is not found" },
            { ERR_ADM_OPEN_SPEAKER_FAILED, "can not open speaker" },
            { ERR_ADM_OPEN_MICPHONE_FAILED, "can not open microphone" },
            { ERR_ADM_NO_VALID_DATA, "have not a valid data" },
            { ERR_ADM_NO_RECORD_PERMISSION, "have not record permission" },
            { ERR_ADM_TIME_TOO_SHORT, "time is too short" },
            { ERR_ADM_TIME_TOO_LONG, "time is too long" },
            { ERR_NETWORK_POOL, "the network is not good" },
            { ERR_NETWORK_BROKEN, "the network is broken" },
            { ERR_NET_FILE_UPLOAD_FAILED, "upload file failed" },
            { ERR_NET_FILE_DOWNLOAD_FAILED, "download file failed" },
            { ERR_FILE_NOT_FOUND, "can not found file" },
            { ERR_FILE_OPEN_FAILED, "can not open file" },
        };

        int TransformErrorCode( int ec )
        {
			return ec;
            //using namespace snail::media;
            //int api_ec = ERR_OK;
            //switch ( ec )
            //{
            //case err_succeed:
            //    api_ec = ERR_OK; break;
            //case err_connect_mcu_failed:
            //    api_ec = ERR_SERVER_CONNECT_FAILED; break;
            //case err_request_mcu_timeout:
            //    api_ec = ERR_TIME_OUT; break;
            //case err_connect_http_timeout:
            //    api_ec = ERR_TIME_OUT; break;
            //case err_config:
            //    api_ec = -1; break;
            //case err_connect_audio_server:
            //    api_ec = ERR_SERVER_CONNECT_FAILED; break;
            //case err_connect_http_failed:
            //    api_ec = ERR_SERVER_CONNECT_FAILED; break;
            //case err_network_pool:
            //    api_ec = ERR_NETWORK_POOL; break;
            //case err_net_broken:
            //    api_ec = ERR_NETWORK_BROKEN; break;
            //case err_not_logined:
            //    api_ec = ERR_NOT_LOGINED; break;
            //case err_notfound_user:
            //    api_ec = ERR_USER_NOT_FOUND; break;
            //case err_not_init:
            //    api_ec = ERR_NOT_INITIALIZE; break;
            //case err_file_notfound:
            //    api_ec = ERR_FILE_NOT_FOUND; break;
            //case err_file_badformat:
            //    api_ec = ERR_FILE_OPEN_FAILED; break;
            //case err_file_openfailed:
            //    api_ec = ERR_FILE_OPEN_FAILED; break;
            //case err_task_busy:
            //    api_ec = ERR_ALREADY_IN_USE; break;
            //case err_not_enough_space:
            //    api_ec = ERR_FILE_OPEN_FAILED; break;
            //case err_file_recordfiled:
            //    api_ec = ERR_ADM_NO_VALID_DATA; break;
            //case err_file_recorddataless:
            //    api_ec = ERR_ADM_TIME_TOO_SHORT; break;
            //case err_time_too_long:
            //    api_ec = ERR_ADM_TIME_TOO_LONG; break;
            //case err_connect_ftp_failed:
            //    api_ec = ERR_SERVER_CONNECT_FAILED; break;
            //case err_download_file_notfound:
            //    api_ec = ERR_MSG_NOT_FOUND; break;
            //case err_download_url_notsupport:
            //    api_ec = ERR_MSG_NOT_FOUND; break;
            //case err_request_qntoken:
            //    api_ec = ERR_NET_FILE_UPLOAD_FAILED; break;
            //case err_request_down_url:
            //    api_ec = ERR_NET_FILE_DOWNLOAD_FAILED; break;
            //case err_upload_file_failed:
            //    api_ec = ERR_NET_FILE_UPLOAD_FAILED; break;
            //case err_upload_file_cancel:
            //    api_ec = ERR_NET_FILE_UPLOAD_FAILED; break;
            //case err_download_file_failed:
            //    api_ec = ERR_NET_FILE_DOWNLOAD_FAILED; break;
            //case err_download_cancel:
            //    api_ec = ERR_NET_FILE_DOWNLOAD_FAILED; break;
            //case err_stt_svr_failed:
            //    api_ec = ERR_SERVER_CONNECT_FAILED; break;
            //case err_mcu_not_find_room:
            //    api_ec = ERR_ROOM_NOT_FOUND; break;
            //case err_mcu_not_find_user:
            //    api_ec = ERR_USER_NOT_FOUND; break;
            //case err_mcu_version_support:
            //    api_ec = ERR_NOT_VERSION_SUPPORTED; break;
            //case err_mcu_not_get_msgs:
            //    api_ec = ERR_MSG_NOT_FOUND; break;
            //case err_mcu_msg_notsupport:
            //    api_ec = ERR_MSG_NOT_FOUND; break;
            //case err_db_roomkey_autho:
            //    api_ec = ERR_INVALID_ROOM_KEY; break;
            //case err_db_version_support:
            //    api_ec = ERR_NOT_VERSION_SUPPORTED; break;
            //case err_db_room_not_exist:
            //    api_ec = ERR_ROOM_NOT_FOUND; break;
            //case err_db_request_timeout:
            //    api_ec = ERR_TIME_OUT; break;
            //case err_db_autho_check_failed:
            //    api_ec = ERR_INVALID_AUTHO_KEY; break;
            //case err_qiniu_reqtokenfailed:
            //    api_ec = ERR_NET_FILE_UPLOAD_FAILED; break;
            //case err_audio_no_record_access:
            //    api_ec = ERR_ADM_NO_RECORD_PERMISSION; break;
            //case err_not_found_record_device:
            //    api_ec = ERR_ADM_NO_FOUND_MICPHONE; break;
            //case err_not_found_playout_device:
            //    api_ec = ERR_ADM_NO_FOUND_SPEAKER; break;
            //case err_open_record_device_failed:
            //    api_ec = ERR_ADM_OPEN_MICPHONE_FAILED; break;
            //case err_open_playout_device_failed:
            //    api_ec = ERR_ADM_OPEN_SPEAKER_FAILED; break;
            //case EC_HAS_RUN:
            //    api_ec = ERR_ALREADY_IN_USE; break;
            //case EC_PARAMS_ERROR:
            //    api_ec = ERR_INVALID_ARGUMENT; break;
            //case EC_NOT_LOGINED:
            //    api_ec = ERR_NOT_LOGINED; break;
            //case EC_FUNCTION_DISABLEED:
            //    api_ec = ERR_NOT_SUPPORTED; break;
            //case EC_TASK_COMPLATED:
            //    api_ec = ERR_OK; break;
            //case EC_TASK_NOTCOMPLATE:
            //    api_ec = ERR_ALREADY_IN_USE; break;
            //default:
            //    api_ec = ERR_GENERIC;
            //    break;
            //}
            //return api_ec;
        }

}