#ifndef FACE_ENTERING_H
#define FACE_ENTERING_H

#include<QWidget>
#include<QImage>
#include<QDebug>
#include<QTimer>
#include <iostream>
#include "http_client.h"

namespace Ui {
class face_entering;
}

class face_entering : public QWidget
{
    Q_OBJECT

public:
    explicit face_entering(QWidget *parent = nullptr);
    ~face_entering();
    QTimer* registeration_preview_timer;
    QTimer* single_http_request_timer;

    void start();
    void show_registeration_face();

private slots:
    void get_registeration_frame_and_show();

private:
    Ui::face_entering *ui;
};

#endif // FACE_ENTERING_H
