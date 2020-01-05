#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QtDebug>
#include <QMessageBox>
#include "polynomial_funcs.hpp"
#include "utils.hpp"

void refresh_database();

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void add1();
    void add2();
    void add3();
    void plus();
    void minus();
    void mult();
    void eql();
    void derivative();
    void value();
    void roots();
    void div();
};

#endif // MAINWINDOW_H
