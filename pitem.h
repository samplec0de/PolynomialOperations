#ifndef PITEM_H
#define PITEM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include "Polynomial.hpp"
#include "DataBase.hpp"

class PItem : public QWidget
{
    Q_OBJECT
public:
    explicit PItem(QWidget *parent = nullptr, Polynomial * pol = nullptr, DataBase * datab = nullptr, int num = 0);
    ~PItem();


signals:

public slots:
    void deleteClicked();
};



#endif // PITEM_H
