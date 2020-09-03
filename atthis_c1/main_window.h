#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include<QWidget>
#include<QImage>
#include<QDebug>
#include<QTimer>
#include<QCameraInfo>
#include <iostream>

typedef long LONG;
typedef unsigned long DWORD;
typedef unsigned short WORD;

namespace Ui {
class Widget;
}

QT_BEGIN_NAMESPACE
namespace Ui { class main_window; }
QT_END_NAMESPACE

class main_window : public QWidget
{
    Q_OBJECT

public:
    main_window(QWidget *parent = nullptr);
    ~main_window();

    QTimer* stream_timer;

    QImage get_mainwindow_image(){
        return m_latest_camera_frame;
    }


    static main_window& get_instance() {
        static main_window main_window_inst;
        return main_window_inst;
    }

MAIN_WINDOW_H
private slots:
    void on_btn_go2form_clicked();
    void on_btn_go3form_clicked();
    void show_frame();

    void on_snapshot_clicked();

private:
MAIN_WINDOW_H
    Ui::main_window *ui; MAIN_WINDOW_H
    uint64_t m_total_frames_rendered = 0;
    QImage m_latest_camera_frame;
};
#endif // MAIN_WINDOW_H
