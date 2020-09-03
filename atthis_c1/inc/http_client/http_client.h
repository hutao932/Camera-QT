#ifndef __HttpClient__
#define __HttpClient__

#include <QThread>
#include <QFileDialog>
#include <QFile>
#include "base.h"
#include "http.h"
#include "json/json.h"
#include "atthis_protocal.h"

class HttpClient : public QThread
{
public:
    void stopThread();

    static HttpClient& get_instance() {
        static HttpClient http_client_inst;
        return http_client_inst;
    }
    enum HttpClientTaskType {
        HTTP_CLIENT_FIRMWARE_UPGRADE,
        HTTP_CLIENT_USER_REGISTERATION
    };

    /*HTTP device management*/
    void device_management_set_device_time(uint64_t time);
    void device_management_set_device_ip(std::string ip);
    void device_management_reset();
    void device_management_restore();
    void device_management_set_rgb_led(uint8_t brightness);
    void device_management_set_ir_led(uint8_t brightness);
    void device_management_set_password(std::string password);
    bool device_management_get_atthis_c1_firmware_rev(std::string& firmware_rev);
    bool device_management_get_device_sn(std::string& device_sn);
    void device_management_switch_preview_source(int source);
    void device_management_restore_password(void);

    /*HTTP Recognition management(Face Algorithm parameters setting)*/
    void recognition_management_set_multi_face_detection_enable(int multi_face_detection_enable);
    void recognition_management_set_face_recognition_minsize(int face_detection_minsize);
    void recognition_management_set_face_detection_bbox(int show_bbox);
    void recognition_management_set_recogniton_threshold(int recognition_threshold);
    void recognition_management_set_liveness_type(int liveness_type);
    void recognition_management_set_liveness_threshold(int liveness_threshold);
    void recognition_management_set_dms_enable(int dms_enable);
    void recognition_management_set_face_quality(int workfield, int quality_level);

    /*HTTP user management*/
    int user_management_add_user(std::string username);
    int user_management_delete_user(std::string username);
    int user_management_search_user(std::string username);
    int user_management_query_records_num();
    int user_management_delete_dataset();
    /*HTTP recognition records management*/

     /*HTTP device upgrade management*/
    int upgrade_management_firmware_upload(std::string firmware_base64_string);
    void upgrade_management_start_upgrade();
    int upgrade_management_get_upgrade_status();

    int upgrade_management_firmware_upload_task(QString fileName);

    void get_register_image(std::string &image) {
        image = m_register_image;
    }

    int m_task_type = 0;
    int m_wait_flag = 1;
    QString m_task_para_str;

protected:
    virtual void run();

private:
    HttpClient();
    volatile bool isStop;
    std::string m_device_ip;
    std::string m_http_client_listen_port;
    std::string m_device_gateway;
    std::string m_device_netmask;
    std::string m_http_client_port_for_firmware_upgrade;
    std::string m_register_image;
signals:
    void request_done();
    void send_request_done_signal();

};

#endif // __HttpClient__
