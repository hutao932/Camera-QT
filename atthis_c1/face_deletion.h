#ifndef FACE_DELETION_H
#define FACE_DELETION_H

#include <QWidget>
#include <iostream>

namespace Ui {
class face_deletion;
}

class face_deletion : public QWidget
{
    Q_OBJECT

public:
    explicit face_deletion(QWidget *parent = nullptr);
    ~face_deletion();

private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_search_2_clicked();

    void on_pushButton_clear_clicked();

private:

     Ui::face_deletion *ui;
    std::string m_username;
    int m_database_records;
};

#endif // FACE_DELETION_H
