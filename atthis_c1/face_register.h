#ifndef FACE_REGISTER_H
#define FACE_REGISTER_H

#include <stdio.h>
#include <QWidget>
#include <QMessageBox>
namespace Ui {
class face_register;
}

class face_register : public QWidget
{
    Q_OBJECT

public:
    explicit face_register(QWidget *parent = nullptr);
    ~face_register();
    void get_registeration_new_username(std::string *username);

private slots:
    void on_btn_go5form_clicked();


private:
    QMessageBox m_msgBox;
    std::string m_new_username;
    Ui::face_register *ui;
};

#endif // FACE_REGISTER_H
