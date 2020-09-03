#ifndef FUNCTION_SET_H
#define FUNCTION_SET_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include "http_client.h"

namespace Ui {
class function_set;
}

class function_set : public QWidget
{
    Q_OBJECT

public:
    explicit function_set(QWidget *parent = nullptr);
    ~function_set();
    void update_upgrade_progrocess(int percent);

private slots:
    void on_preview_source_stateChanged(int arg1);

    void on_liveness_score_txtbox_editingFinished();

    void on_recognition_score_textbox_editingFinished();

    void on_face_detection_minsize_textbox_editingFinished();

    void on_firmware_upgrade_clicked();

    void on_pushButton_reset_clicked();

    void on_pushButton_restore_clicked();

    void on_multi_face_detection_chkbox_stateChanged(int arg1);

    void on_dms_enable_chkbox_stateChanged(int arg1);

    void on_pushButton_activate_clicked();

    void on_comboBox_liveness_type_currentIndexChanged(int index);

    void on_checkBox_RGB_stateChanged(int arg1);

    void on_checkBox_IR_stateChanged(int arg1);

    void on_checkBox_face_quality_stateChanged(int arg1);

    void on_pushButton_sync_time_clicked();

    void on_btn_password_restore_clicked();

    void on_comboBox_liveness_type_activated(const QString &arg1);

private:
    Ui::function_set *ui;
    uint16_t m_liveness_threshold;
    uint16_t m_recognition_threshold;
    uint16_t m_detection_minsize;
    uint16_t m_liveness_updated_count = 0;
};

#endif // FUNCTION_SET_H
