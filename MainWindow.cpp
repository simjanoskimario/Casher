#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

int findRowByKey(QTableWidget *table, const QString &searchKey) {
    QList<QTableWidgetItem *> matches = table->findItems(searchKey, Qt::MatchExactly);
    for (QTableWidgetItem *item : matches) {
        if (item->column() == 0) {
            return item->row();
        }
    }
    return -1;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dataBase(Casher::KeyValStore(1024)){
    ui->setupUi(this);
    ui->Table->setColumnWidth(0, 180);

    connect(ui->setButton, &QPushButton::clicked, this, [this]() {
        QString key = ui->keyBox->text().trimmed();
        QString value = ui->valueBox->text().trimmed();

        bool s = dataBase.set(key.toStdString(),value.toStdString());
        if (s) {
            int newRow = ui->Table->rowCount();

            ui->Table->insertRow(newRow);
            ui->Table->setItem(newRow,0, new QTableWidgetItem(key));
            ui->Table->setItem(newRow,1, new QTableWidgetItem(value));
        }
    });

    connect(ui->delButton, &QPushButton::clicked, this, [this](){
        QString key = ui->keyBox->text().trimmed();
        QString value = ui->valueBox->text().trimmed();

        bool s = dataBase.del(key.toStdString());
        if (s) {
            int row = findRowByKey(ui->Table, key);
            if (row != -1) {
                ui->Table->removeRow(row);
            }
            //ui->keyBox->clear();
        }
    });
}

MainWindow::~MainWindow() {
    delete ui;
}
