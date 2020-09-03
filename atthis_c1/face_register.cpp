#include "face_register.h"
#include "ui_face_register.h"
#include "com_face_entering.h"
#include "http_client.h"

face_register::face_register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::face_register)
{
    ui->setupUi(this);
}

face_register::~face_register()
{
    delete ui;
}

/*
*
*/
void face_register::on_btn_go5form_clicked() {

    this->hide();
    Com_face_entering::p_face_entering->show();
    Com_face_entering::p_face_entering->start();
}

// TODO(wangxiang): may still need to check if the current user_id has been registered or not
/*
 * Here is the precedure for registering a new user to ATThis
 * 1)Send new user name , ATThis check database and return her response
 * 2)Based step1 record,if YES, show a quick hint for updating the user name,
 *                      if Not, then send a image with face centered for ATThis to start the process

void face_register::on_lineEdit_editingFinished() {

    std::string new_user_name = ui->lineEdit->text().toStdString();
    HttpClient& http_client = HttpClient::get_instance();
    int ret_code = http_client.user_management_search_user(new_user_name);

    if (ret_code == 0) { //new username has been registered
        m_msgBox.setText("The user has been registered");
    } else {
        m_msgBox.setText("incorrect http response");
    }
    m_msgBox.exec();
}
*/

//
void face_register::get_registeration_new_username(std::string *username) {

    *username = ui->lineEdit->text().toStdString();
}
