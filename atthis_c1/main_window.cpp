#include "main_window.h"
#include "ui_main_window.h"
#include "com_face_management.h"
#include "com_function_set.h"
#include "camera.h"
#include <stdio.h>
#include <iostream>

main_window::main_window(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::main_window)
{
    ui->setupUi(this);

    int preview_width = 1280;
    int preview_height = 720;
    int t_index = 0;
    int camera_index = 0;
    int number_of_atthis_devices = 0;
    bool found_ATThis_C1 = false;

    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras) {
          qDebug("description: %s", cameraInfo.description().toStdString().data());
          if (cameraInfo.description().contains("Multifunction") == true) {
              qDebug("device name: %s", cameraInfo.deviceName().toStdString().data());
              QStringList device_list = cameraInfo.deviceName().split("video");

              QString device_name  = device_list.at(device_list.size() - 1);
              qDebug("device index: %s", device_name.toStdString().data());
              camera_index = device_name.toInt();
              found_ATThis_C1 = true;
              number_of_atthis_devices++;
          }
          t_index++;
    }

    if (found_ATThis_C1) {
        qDebug("found %d ATThis devices in %d webcams", number_of_atthis_devices, t_index);

        // Open the last one only at this moment
        OpenCV_Camera& camera_instance = OpenCV_Camera::get_instance();
        preview_width = camera_instance.get_camera_res_width();
        preview_height = camera_instance.get_camera_res_height();
        camera_instance.start(camera_index);

        stream_timer = new QTimer(this);
        connect(stream_timer, SIGNAL(timeout()), this, SLOT(show_frame()));
        stream_timer->start(40);
    } else {
        qDebug("Did not find any ATThis device");
    }

    // force this main form and Lable to fit video size
    this->resize(preview_width, preview_height);
    ui->label->resize(preview_width, preview_height);
}

main_window::~main_window()
{
    delete ui;
    stream_timer->stop();
    OpenCV_Camera::get_instance().stop();
}

void main_window::on_btn_go2form_clicked() {
    Com_face_management::p_face_management->show();
}

void main_window::on_btn_go3form_clicked() {
    Com_function_set::p_function_set->show();
}

void main_window::show_frame() {
    m_total_frames_rendered++;
    m_latest_camera_frame = OpenCV_Camera::get_instance().camera_get_frame();
    m_latest_camera_frame.scaled(ui->label->size(), Qt::KeepAspectRatio);
    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap::fromImage(m_latest_camera_frame));
}

void main_window::on_snapshot_clicked()
{
    QString filename = QString::number(m_total_frames_rendered, 10);
    filename = filename + ".jpg";
    m_latest_camera_frame.save(filename, "JPG", 100);
}
