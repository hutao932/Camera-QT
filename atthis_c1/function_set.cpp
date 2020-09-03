#include "version.h"
#include "function_set.h"
#include "ui_function_set.h"
#include "com_main_window.h"
#include "com_face_management.h"
#include "camera.h"
#include "base.h"
#include "http.h"
#include <QTime>
#include <QMessageBox>

function_set::function_set(QWidget *parent) :
    QWidget(parent), ui(new Ui::function_set) {
    ui->setupUi(this);

    QFont font("Microsoft YaHei", 12, 75);
    ui->label->setFont(font);
    ui->label_liveness_enable->setFont(font);
    ui->label__recognition_threshold->setFont(font);
    ui->label_face_detection_min_size->setFont(font);
    ui->label_dms_enable->setFont(font);
    ui->label_liveness_threshold->setFont(font);
    ui->label_face_quality->setFont(font);
    ui->label_preview_source->setFont(font);

    QStringList liveness_type;
    liveness_type << "NONE" << "RGB" << "NIR" << "RGB+NIR";
    ui->comboBox_liveness_type->addItems(liveness_type);

    qDebug("APP version: %s", ATTHIS_VERSION);
    ui->label_15->setText(ATTHIS_VERSION);

    // Get ATThis_C1 firm version
    std::string atthis_c1_firmware_string;
    HttpClient::get_instance().device_management_get_atthis_c1_firmware_rev(atthis_c1_firmware_string);
    ui->label_atthis_c1_firmware_rev->setText(QString::fromStdString(atthis_c1_firmware_string));

    std::string device_sn;
    HttpClient::get_instance().device_management_get_device_sn(device_sn);
    ui->label_device_sn->setText(QString::fromStdString(device_sn));
}

function_set::~function_set() {
    delete ui;
}

void function_set::on_preview_source_stateChanged(int arg1) {
    uint16_t preview_source = 0; //RGB
    if (ui->preview_source->isChecked()) {
        preview_source = 1;
    } else {
        preview_source = 0;
    }

    //HttpClient::get_instance().device_management_switch_preview_source(preview_source);
    OpenCV_Camera::get_instance().set_brightness();
}

void function_set::on_liveness_score_txtbox_editingFinished() {

    uint16_t livethreshold = ui->liveness_score_txtbox->text().toUInt();
    qDebug("live threshold: %d", livethreshold);

    if (livethreshold > 100) {
        QMessageBox msgBox;
        msgBox.setText("illegal parameter");
        msgBox.exec();
        return;
    }
    m_liveness_threshold = livethreshold;
}

void function_set::on_recognition_score_textbox_editingFinished() {

    uint16_t recognition_threshold = ui->recognition_score_textbox->text().toUInt();
    qDebug("recog threshold: %d", recognition_threshold);
    if (recognition_threshold > 100) {
        QMessageBox msgBox;
        msgBox.setText("illegal parameter");
        msgBox.exec();
        return;
    }
    m_recognition_threshold = recognition_threshold;
}

void function_set::on_face_detection_minsize_textbox_editingFinished() {
    uint16_t detection_minsize = ui->face_detection_minsize_textbox->text().toUInt();
    qDebug("detect min face: %d", detection_minsize);
    if (detection_minsize < 30 || detection_minsize > 720) {
        QMessageBox msgBox;
        msgBox.setText("illegal parameter");
        msgBox.exec();
        return;
    }
    m_detection_minsize = detection_minsize;
}

void function_set::on_firmware_upgrade_clicked()
{
    std::cout << "firmware updating" << std::endl;
    QString fileName = QFileDialog::getOpenFileName(this, tr("firmware upgrade"));

    HttpClient& http_client = HttpClient::get_instance();

    http_client.upgrade_management_firmware_upload_task(fileName);
    http_client.start();
}

void function_set::on_pushButton_reset_clicked()
{
    HttpClient::get_instance().device_management_reset();
}

void function_set::on_pushButton_restore_clicked()
{
    HttpClient::get_instance().device_management_restore();
}

void function_set::on_multi_face_detection_chkbox_stateChanged(int arg1)
{
    uint16_t multi_faces_detection_enable = 0; //RGB
    if (ui->multi_face_detection_chkbox->isChecked()) {
        multi_faces_detection_enable = 1;
    } else {
        multi_faces_detection_enable = 0;
    }
    qDebug("multi face: %d", multi_faces_detection_enable);
    HttpClient::get_instance().recognition_management_set_multi_face_detection_enable(multi_faces_detection_enable);
}

void function_set::on_dms_enable_chkbox_stateChanged(int arg1)
{
    uint16_t dms_enable = 0; //RGB
    if (ui->dms_enable_chkbox->isChecked()) {
        dms_enable = 1;
    } else {
        dms_enable = 0;
    }
    qDebug("dms enable: %d", dms_enable);
    HttpClient::get_instance().recognition_management_set_dms_enable(dms_enable);
}

void function_set::on_pushButton_activate_clicked()
{
    uint16_t livethreshold = ui->liveness_score_txtbox->text().toUInt();
    qDebug("live threshold: %d", livethreshold);

    if (livethreshold > 100) {
        QMessageBox msgBox;
        msgBox.setText("illegal parameter");
        msgBox.exec();
        return;
    }
    m_liveness_threshold = livethreshold;

    uint16_t recognition_threshold = ui->recognition_score_textbox->text().toUInt();
    qDebug("recog threshold: %d", recognition_threshold);
    if (recognition_threshold > 100) {
        QMessageBox msgBox;
        msgBox.setText("illegal parameter");
        msgBox.exec();
        return;
    }
    m_recognition_threshold = recognition_threshold;

    uint16_t detection_minsize = ui->face_detection_minsize_textbox->text().toUInt();
    qDebug("detect min face: %d", detection_minsize);
    if (detection_minsize < 30 || detection_minsize > 720) {
        QMessageBox msgBox;
        msgBox.setText("illegal parameter");
        msgBox.exec();
        return;
    }
    m_detection_minsize = detection_minsize;

    HttpClient::get_instance().recognition_management_set_liveness_threshold(m_liveness_threshold);
    HttpClient::get_instance().recognition_management_set_recogniton_threshold(m_recognition_threshold);
    HttpClient::get_instance().recognition_management_set_face_recognition_minsize(m_detection_minsize);
    //on_pushButton_settime_clicked();
    //on_pushButton_set_ip_clicked();
}

void function_set::on_comboBox_liveness_type_currentIndexChanged(int index)
{
    uint16_t liveness_type = ui->comboBox_liveness_type->currentIndex();

    if (m_liveness_updated_count != 0) {
        qDebug("liveness type 0: %d", liveness_type);
        HttpClient::get_instance().recognition_management_set_liveness_type(liveness_type);
    }
    m_liveness_updated_count++;
}

void function_set::update_upgrade_progrocess(int percent) {
    ui->upgrade_progress_bar->setValue(percent);
}


void function_set::on_checkBox_RGB_stateChanged(int arg1)
{
    uint8_t brightness = 0;
    if (ui->checkBox_RGB->isChecked()) {
        brightness = 1;
    }
    HttpClient::get_instance().device_management_set_rgb_led(brightness);
}

void function_set::on_checkBox_IR_stateChanged(int arg1)
{
    uint8_t brightness = 0;
    if (ui->checkBox_IR->isChecked()) {
        brightness = 1;
    }
    HttpClient::get_instance().device_management_set_ir_led(brightness);
}

void function_set::on_checkBox_face_quality_stateChanged(int arg1)
{
    uint16_t quality_work_field = 0; //registeration + recognition

    // face quality level
    uint16_t quality_level = 0;

    if (ui->checkBox_face_quality->isChecked()) {
        quality_level = 2; // level "NORMAL"
    } else {
        quality_level = 0; // no quality control
    }
    qDebug("quality work field: %d quality level: %d", quality_work_field, quality_level);

    HttpClient::get_instance().recognition_management_set_face_quality(quality_work_field, quality_level);
}

void function_set::on_pushButton_sync_time_clicked()
{
    QDateTime current_time = QDateTime::currentDateTime();
    uint64_t time = current_time.toTime_t();
    HttpClient::get_instance().device_management_set_device_time(time);
}

void function_set::on_btn_password_restore_clicked()
{
    HttpClient::get_instance().device_management_restore_password();
}
