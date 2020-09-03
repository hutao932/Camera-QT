#include "face_deletion.h"
#include "ui_face_deletion.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QMainWindow>
#include <stdio.h>
#include "http_client.h"


const int W_ICONSIZE = 96;
const int H_ICONSIZE = 96;
const int H_ITEMSIZE = 110;

face_deletion::face_deletion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::face_deletion)
{
    ui->setupUi(this);
    ui->listWidget->setIconSize(QSize(W_ICONSIZE, H_ICONSIZE));
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    ui->listWidget->setSpacing(10);

    ui->listWidget->resize(400, 400);

}

face_deletion::~face_deletion()
{
    delete ui;
}

void face_deletion::on_pushButton_search_clicked()
{
    std::cout <<  ui->lineEdit_username->text().toStdString().data() << std::endl;
    m_username = ui->lineEdit_username->text().toStdString().data();

    //Step1: clean the current

    int item_num = ui->listWidget->count();
    for (int i = 0; i < item_num; i++) {
        QListWidgetItem *item = ui->listWidget->takeItem(0);
        delete item;
    }

    // TODO: Here we need query how many records in database, and load all the matched records here(face images)
    int records_num = 5;
    for (int nIndex = 0; nIndex < records_num; ++nIndex) {
        QString strPath = QString("qt_images/%1.jpg").arg(nIndex+1);
        QPixmap objPixmap(strPath);
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(W_ICONSIZE,H_ICONSIZE))), \
                                                     QString("%1.jpg").arg(nIndex+1));
        pItem->setSizeHint(QSize(W_ICONSIZE,H_ITEMSIZE));
        ui->listWidget->insertItem(nIndex, pItem);
    }

    // relative path of binary
    QString absPath = QApplication::applicationDirPath();
    std::cout << absPath.toStdString().data() << std::endl;
    //Step2: Query database with this basename "username*", then load all the face images here


}

void face_deletion::on_pushButton_search_2_clicked() {
    QList<QListWidgetItem*> selectedItems = ui->listWidget->selectedItems();
    if (selectedItems.count() > 0) {
        foreach(QListWidgetItem* var, selectedItems) {
            ui->listWidget->removeItemWidget(var);
            selectedItems.removeOne(var);
            delete var;
        }
    }
}

void face_deletion::on_pushButton_clear_clicked()
{
    HttpClient::get_instance().user_management_delete_dataset();


   //  ui->facerecordsnum_textbox->setText(QString::number(0));

     m_database_records = 0;
}
