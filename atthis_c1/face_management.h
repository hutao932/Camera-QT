#ifndef FACE_MANAGEMENT_H
#define FACE_MANAGEMENT_H

#include <QWidget>

namespace Ui {
class face_management;
}

class face_management : public QWidget
{
    Q_OBJECT

public:
    explicit face_management(QWidget *parent = nullptr);
    ~face_management();
    void update_database_records();

    QTimer* records_timer;

private slots:
    void on_btn_go1form_clicked();

    void on_btn_go3form_clicked();
    void update_records();

private:
    Ui::face_management *ui;
    int m_database_records;
};

#endif // FACE_MANAGEMENT_H
