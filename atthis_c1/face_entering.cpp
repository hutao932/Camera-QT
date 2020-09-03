#include<QImage>
#include<QDebug>
#include<QTimer>
#include<QPoint>
#include<QPainter>
#include<QLabel>
#include<QMouseEvent>
#include<QPen>

#include "face_entering.h"
#include "ui_face_entering.h"
#include "main_window.h"
#include "camera.h"
#include "com_face_register.h"
#include "com_face_management.h"

face_entering::face_entering(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::face_entering) {
    ui->setupUi(this);
    ui->label_txt->raise();
}

face_entering::~face_entering()
{
    delete ui;
    registeration_preview_timer->stop();
    //re-enable bbox after new user registeration
    HttpClient& http_client = HttpClient::get_instance();
    http_client.recognition_management_set_face_detection_bbox(1);
}

void face_entering::start() {

    registeration_preview_timer = new QTimer(this);
    connect(registeration_preview_timer, SIGNAL(timeout()), this, SLOT(get_registeration_frame_and_show()));
    registeration_preview_timer->start(40);

    //disable bbox before new user registeration
    HttpClient& http_client = HttpClient::get_instance();
    http_client.recognition_management_set_face_detection_bbox(0);

    qDebug("=======================start register");
    http_client.m_task_type = 1;
    http_client.m_wait_flag = 0;
    http_client.start();
}

void face_entering::get_registeration_frame_and_show() {

    QPainter painter;
    // clear text
    QFont label_font;
    label_font.setPointSize(16);

    // Text color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);

    ui->label_txt->setText("请将人脸置于下方红框内，并保持正视前方");
    ui->label_txt->setFont(label_font);
    ui->label_txt->setPalette(pa);

    QImage image_src = OpenCV_Camera::get_instance().camera_get_frame();
    QImage image = image_src.copy(340, 110, 500, 500);

    painter.begin(&image);
    painter.setPen(QPen(Qt::red, 2));
    painter.drawRect(QRect(100, 100, 300, 300));
    painter.end();

    image.scaled(ui->label->size(), Qt::KeepAspectRatio);
    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void face_entering::show_registeration_face() {

    std::string username;
    Com_face_register::p_face_register->get_registeration_new_username(&username);

    registeration_preview_timer->stop();

    // Make sure the last call for slot route has been made
    QThread::msleep(100);
    //Only show the 300*300 face in the label

    // Text size
    QFont label_font;
    label_font.setPointSize(20);

    // Text color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::green);

    ui->label_txt->setText("注册成功!");
    ui->label_txt->setFont(label_font);
    ui->label_txt->setPalette(pa);

    QImage image_src = OpenCV_Camera::get_instance().camera_get_frame();
    QImage image = image_src.copy(440, 210, 300, 300);

    /*
    //image.scaled(ui->label->size(), Qt::KeepAspectRatio);
    ui->label->setScaledContents(false);
    ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->label->setPixmap(QPixmap::fromImage(image));

    ui->label__username->setText(QString::fromStdString(username));
    */

    std::string register_image_string;
    HttpClient::get_instance().get_register_image(register_image_string);

    QPixmap register_face;
    register_face.loadFromData(\
                QByteArray::fromBase64(QString::fromStdString(register_image_string).toLocal8Bit()), "JPG");

    //image.scaled(ui->label->size(), Qt::KeepAspectRatio);
    ui->label->setScaledContents(false);
    ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->label->setPixmap(register_face);
    ui->label__username->setText(QString::fromStdString(username));

    // update local records
    Com_face_management::p_face_management->update_database_records();
}
