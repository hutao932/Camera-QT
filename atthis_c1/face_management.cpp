#include "face_management.h"
#include "ui_face_management.h"
#include "com_main_window.h"
#include "com_function_set.h"
#include "com_face_register.h"
#include "com_face_deletion.h"
#include "http_client.h"

face_management::face_management(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::face_management)
{
    ui->setupUi(this);
    int records_num = HttpClient::get_instance().user_management_query_records_num();
    qDebug("records_num: %d", records_num);
    m_database_records = records_num;
    ui->facerecordsnum_textbox->setText(QString::number(records_num));

    records_timer = new QTimer(this);
    connect(records_timer, SIGNAL(timeout()), this, SLOT(update_records()));
    records_timer->start(1000);
}

void face_management::update_records() {
    ui->facerecordsnum_textbox->setText(QString::number(m_database_records));
}
face_management::~face_management()
{
    records_timer->stop();
    delete ui;
}

void face_management::on_btn_go1form_clicked() {
    this->hide();
   // ComOneForm::p_oneform->show();
    Com_face_register::p_face_register->show();

}

void face_management::on_btn_go3form_clicked() {
    Com_face_deletion::p_face_deletion->show();
    /*
    HttpClient::get_instance().user_management_delete_dataset();
    ui->facerecordsnum_textbox->setText(QString::number(0));
    m_database_records = 0;
    */
}
void face_management::update_database_records() {
    m_database_records++;
    //ui->facerecordsnum_textbox->setText(QString::number(m_database_records));
    qDebug("updated records num: %d", m_database_records);
}
