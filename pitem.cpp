#include "pitem.h"
#include "mainwindow.h"

QHBoxLayout * mainLay;
QLineEdit * label;
QLabel * number;
QToolButton * copy, * del;
Polynomial * pol_g;
DataBase * db_l;

PItem::PItem(QWidget *parent, Polynomial * pol, DataBase * datab, int num) : QWidget(parent)
{
    db_l = datab;
    mainLay = new QHBoxLayout();
    pol_g = pol;
    this->setLayout(mainLay);
    label = new QLineEdit(QString::fromStdString(pol->str()));
    label->setReadOnly(true);
    number = new QLabel(QString::number(num) + ".");
    number->setFont(QFont("Helvetica", 20));
    label->setFont(QFont("Helvetica", 20));
    number->setStyleSheet("QLabel {"
                          "color: white;"
                          "}");
    label->setStyleSheet("QLabel {"
                          "color: white;"
                          "}");
    mainLay->addWidget(number, Qt::AlignLeft);
    mainLay->addWidget(label, Qt::AlignLeft);
    label->setFixedSize(QSize(250, 40));
    number->setFixedSize(QSize(30, 35));
//    copy = new QToolButton();
    del = new QToolButton();
    QIcon * ico_copy = new QIcon();
    ico_copy->addPixmap(QPixmap(":/images/copy.png"));
//    copy->setIcon(*ico_copy);

    QIcon * ico_del = new QIcon();
    ico_del->addPixmap(QPixmap(":/images/trash.png"));
    del->setIcon(*ico_del);
//    mainLay->addWidget(copy, Qt::AlignRight);
    mainLay->addWidget(del, Qt::AlignRight);
//    copy->setFixedSize(QSize(20, 20));
    del->setFixedSize(QSize(20, 20));
    this->setFixedSize(QSize(400, 45));
    del->setProperty("n", num);
    connect(del, SIGNAL (clicked()),this, SLOT(deleteClicked()));
}

void PItem::deleteClicked() {
    QToolButton * btn = ((QToolButton *)sender());
    db_l->remove(btn->property("n").toInt());
    refresh_database();
}


PItem::~PItem() {

//    delete label;
//    delete  number;
//    delete copy;
//    delete del;
//    delete mainLay;
}
