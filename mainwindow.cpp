#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DataBase.hpp"
#include "Polynomial.hpp"
#include "pitem.h"

Ui::MainWindow * mw;

QHBoxLayout * mainHBox;
QVBoxLayout * leftVBox, * rightVBox;
QListWidget * databaseListWidget;
QToolButton * btn_minus, * btn_plus, * btn_multiply, * btn_div, * btn_equal;
QLineEdit * aLine, * bLine, * cLine;
QToolButton * btn_2_add, * btn_1_add;
QLineEdit * res_2;
QToolButton * btn_nth_derivative, * btn_value, * btn_roots;
QLineEdit * nLine, * xLine, * res_1;
QToolButton * add_3;

DataBase * db;

Polynomial * p_res_2 = nullptr, * oper_1_res = nullptr;

void refresh_database() {
    while (databaseListWidget->count() > 0) {
        auto itc = databaseListWidget->takeItem(0);
        databaseListWidget->removeItemWidget(itc);
        delete itc;
    }

    int num = 1;
    for (auto cur : db->getAll()) {
//        qDebug() << QString::fromStdString(cur->str());
        QListWidgetItem *item = new QListWidgetItem(databaseListWidget);
        PItem * t = new PItem(databaseListWidget, cur, db, num);
        databaseListWidget->addItem(item);
        item->setSizeHint(t->size());
        databaseListWidget->setItemWidget(item, t);
        num += 1;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mw = ui;
    mainHBox = new QHBoxLayout(ui->centralWidget);
    leftVBox = new QVBoxLayout();
    leftVBox->setAlignment(Qt::AlignTop);
    rightVBox = new QVBoxLayout();
    mainHBox->addLayout(leftVBox);
    mainHBox->addLayout(rightVBox);
    databaseListWidget = new QListWidget();
    rightVBox->addItem(new QSpacerItem(0, 95));
    rightVBox->addWidget(databaseListWidget, 1, Qt::AlignTop);
    leftVBox->addItem(new QSpacerItem(0, 95));
//    QLabel * twoArgsFuncs = new QLabel("Two-argument functions");
//    twoArgsFuncs->setStyleSheet("QLabel {"
//                                "color: #B2BEB5;"
//                                "}");
//    twoArgsFuncs->setFont(QFont("Helvetica", 25));
//    twoArgsFuncs->setFixedHeight(30);
//    leftVBox->addWidget(twoArgsFuncs, 0, Qt::AlignTop);
    aLine = new QLineEdit();
    bLine = new QLineEdit();
    aLine->setPlaceholderText("Polynomial A");
    aLine->setAlignment(Qt::AlignCenter);
    aLine->setFixedWidth(226);
    bLine->setFixedWidth(226);
    bLine->setPlaceholderText("Polynomial B");
    bLine->setAlignment(Qt::AlignCenter);
    QHBoxLayout * linesHBox = new QHBoxLayout();
    leftVBox->addLayout(linesHBox);
    leftVBox->setAlignment(linesHBox, Qt::AlignTop);
    linesHBox->addWidget(aLine);
    linesHBox->addWidget(bLine);
    aLine->setFixedHeight(40);
    bLine->setFixedHeight(40);
    aLine->setFont(QFont("Helvetica", 25));
    bLine->setFont(QFont("Helvetica", 25));

    QHBoxLayout * btnsOpsHBox = new QHBoxLayout();
    leftVBox->addLayout(btnsOpsHBox);
    leftVBox->setAlignment(btnsOpsHBox, Qt::AlignTop);

    btn_plus = new QToolButton();
    btn_plus->setText("+");
    btn_plus->setFont(QFont("Helvetica", 32));
    btn_plus->setStyleSheet("QToolButton {"
                       "background: rgb(255, 149, 0);"
                       "border-radius: 5px;"
                       "color: white;"
                       "}"
                       "QToolButton:pressed {"
                       "background: rgb(255, 160, 0);"
                       "color: white;"
                       "}");
    btn_plus->setFixedSize(QSize(86, 64));
    btnsOpsHBox->addWidget(btn_plus);

    btn_minus = new QToolButton();
    btn_minus->setText("-");
    btn_minus->setFont(QFont("Helvetica", 32));
    btn_minus->setStyleSheet("QToolButton {"
                       "background: rgb(255, 149, 0);"
                       "border-radius: 5px;"
                       "color: white;"
                       "}"
                       "QToolButton:pressed {"
                       "background: rgb(255, 160, 0);"
                       "color: white;"
                       "}");
    btn_minus->setFixedSize(QSize(86, 64));
    btnsOpsHBox->addWidget(btn_minus);

    btn_multiply = new QToolButton();
    btn_multiply->setText("*");
    btn_multiply->setFont(QFont("Helvetica", 32));
    btn_multiply->setStyleSheet("QToolButton {"
                       "background: rgb(255, 149, 0);"
                       "border-radius: 5px;"
                       "color: white;"
                       "}"
                       "QToolButton:pressed {"
                       "background: rgb(255, 160, 0);"
                       "color: white;"
                       "}");
    btn_multiply->setFixedSize(QSize(86, 64));
    btnsOpsHBox->addWidget(btn_multiply);

    btn_div = new QToolButton();
    btn_div->setText("÷");
    btn_div->setFont(QFont("Helvetica", 32));
    btn_div->setStyleSheet("QToolButton {"
                       "background: rgb(255, 149, 0);"
                       "border-radius: 5px;"
                       "color: white;"
                       "}"
                       "QToolButton:pressed {"
                       "background: rgb(255, 160, 0);"
                       "color: white;"
                       "}");
    btn_div->setFixedSize(QSize(86, 64));
    btnsOpsHBox->addWidget(btn_div);

    btn_equal = new QToolButton();
    btn_equal->setText("=");
    btn_equal->setFont(QFont("Helvetica", 32));
    btn_equal->setStyleSheet("QToolButton {"
                       "background: rgb(255, 149, 0);"
                       "border-radius: 5px;"
                       "color: white;"
                       "}"
                       "QToolButton:pressed {"
                       "background: rgb(255, 160, 0);"
                       "color: white;"
                       "}");
    btn_equal->setFixedSize(QSize(86, 64));
    btnsOpsHBox->addWidget(btn_equal);


    res_2 = new QLineEdit();
    res_2->setFixedHeight(40);
    res_2->setStyleSheet("QLabel {"
                         "background-color: white;"
                         "}");
    res_2->setFont(QFont("Helvetica", 20));
    res_2->setAlignment(Qt::AlignCenter);
    res_2->setFixedSize(QSize(370, 40));
    res_2->setReadOnly(true);
    QHBoxLayout * resHBox = new QHBoxLayout();
    leftVBox->addLayout(resHBox, 0);
    leftVBox->setAlignment(resHBox, Qt::AlignTop);
    resHBox->addWidget(res_2, 0, Qt::AlignLeft);
    btn_2_add = new QToolButton();
    btn_2_add->setText("Add");
    btn_2_add->setFont(QFont("Helvetica", 25));
    btn_2_add->setStyleSheet("QToolButton {"
                             "color: white;"
                             "background-color: #555555;"
                             "border-radius: 5px;"
                             "}"
                             "QToolButton:pressed {"
                             "color: white;"
                             "background-color: #1C1E1D;"
                             "border-radius: 5px;"
                             "}");
    btn_2_add->setFixedSize(QSize(85, 40));
    resHBox->addWidget(btn_2_add, 0, Qt::AlignLeft);
    resHBox->setSpacing(8);


    leftVBox->addItem(new QSpacerItem(10, 64));

    QHBoxLayout * lineVal = new QHBoxLayout();
    leftVBox->addLayout(lineVal, 0);
    leftVBox->setAlignment(lineVal, Qt::AlignTop);
    cLine = new QLineEdit();
    cLine->setPlaceholderText("Polynomial");
    cLine->setFixedSize(QSize(370, 40));
    cLine->setFont(QFont("Helvetica", 25));
    cLine->setAlignment(Qt::AlignCenter);
    btn_1_add = new QToolButton();
    btn_1_add->setText("Add");
    btn_1_add->setFont(QFont("Helvetica", 25));
    btn_1_add->setStyleSheet("QToolButton {"
                             "color: white;"
                             "background-color: #555555;"
                             "border-radius: 5px;"
                             "}"
                             "QToolButton:pressed {"
                             "color: white;"
                             "background-color: #1C1E1D;"
                             "border-radius: 5px;"
                             "}"
                             );
    btn_1_add->setFixedSize(QSize(85, 40));
//    btn_1_add->setAlignment(Qt::AlignCenter);
    lineVal->addWidget(cLine, 4);
    lineVal->addWidget(btn_1_add, 1);
    leftVBox->setAlignment(lineVal, Qt::AlignTop);
    QHBoxLayout * btns_1_op = new QHBoxLayout();
    leftVBox->addLayout(btns_1_op, 0);
    leftVBox->setAlignment(btns_1_op, Qt::AlignTop);

    btn_nth_derivative = new QToolButton();
    btn_nth_derivative->setText("n-th d/dx");
    btn_nth_derivative->setFont(QFont("Helvetica", 32));
    btn_nth_derivative->setStyleSheet("QToolButton {"
                                      "background: rgb(255, 149, 0);"
                                      "border-radius: 5px;"
                                      "color: white;"
                                      "}"
                                      "QToolButton:pressed {"
                                      "background: rgb(255, 160, 0);"
                                      "color: white;"
                                      "}");
    btn_nth_derivative->setFixedSize(QSize(155, 40));
    btns_1_op->addWidget(btn_nth_derivative);

    btn_roots = new QToolButton();
    btn_roots->setText("all x, F(x)=0");
    btn_roots->setFont(QFont("Helvetica", 32));
    btn_roots->setStyleSheet("QToolButton {"
                             "background: rgb(255, 149, 0);"
                             "border-radius: 5px;"
                             "color: white;"
                             "}"
                             "QToolButton:pressed {"
                             "background: rgb(255, 160, 0);"
                             "color: white;"
                             "}");
    btn_roots->setFixedSize(QSize(205, 40));
    btns_1_op->addWidget(btn_roots);

    btn_value = new QToolButton();
    btn_value->setText("F(x)");
    btn_value->setFont(QFont("Helvetica", 32));
    btn_value->setStyleSheet("QToolButton {"
                             "background: rgb(255, 149, 0);"
                             "border-radius: 5px;"
                             "color: white;"
                             "}"
                             "QToolButton:pressed {"
                             "background: rgb(255, 160, 0);"
                             "color: white;"
                             "}");
    btn_value->setFixedSize(QSize(92, 40));
    btns_1_op->addWidget(btn_value);

    databaseListWidget->setFixedSize(QSize(450, 425));
    db = new DataBase;
    db->add(new Polynomial("x"));
    db->add(new Polynomial("48x^6+7x^3+x"));
    refresh_database();
    connect(btn_1_add, SIGNAL(clicked()), this, SLOT(add1()));
    connect(btn_2_add, SIGNAL(clicked()), this, SLOT(add2()));
    connect(btn_plus, SIGNAL(clicked()), this, SLOT(plus()));
    connect(btn_minus, SIGNAL (clicked()), this, SLOT(minus()));
    connect(btn_multiply, SIGNAL (clicked()), this, SLOT(mult()));
    connect(btn_equal, SIGNAL (clicked()), this, SLOT(eql()));
    connect(btn_nth_derivative, SIGNAL (clicked()), this, SLOT(derivative()));
    connect(btn_value, SIGNAL (clicked()), this, SLOT(value()));
    connect(btn_roots, SIGNAL (clicked()), this, SLOT(roots()));
    connect(btn_div, SIGNAL (clicked()), this, SLOT(div()));
    nLine = new QLineEdit();
    xLine = new QLineEdit();
    QHBoxLayout * xnHBox = new QHBoxLayout();
    leftVBox->addLayout(xnHBox);
    xnHBox->addWidget(nLine);
    xnHBox->addWidget(xLine);
    nLine->setFixedSize(QSize(155, 40));
    xLine->setFixedSize(QSize(95, 40));
    xnHBox->setAlignment(nLine, Qt::AlignLeft);
    xnHBox->setAlignment(xLine, Qt::AlignRight);
    nLine->setPlaceholderText("n");
    xLine->setPlaceholderText("x");
    nLine->setFont(QFont("Helvetica", 30));
    xLine->setFont(QFont("Helvetica", 30));
    nLine->setAlignment(Qt::AlignCenter);
    xLine->setAlignment(Qt::AlignCenter);
    res_1 = new QLineEdit();
    QHBoxLayout * res1_add = new QHBoxLayout();
    leftVBox->addLayout(res1_add);
    res1_add->addWidget(res_1);
    add_3 = new QToolButton();
    res1_add->addWidget(add_3);
    add_3->setFixedSize(QSize(85, 40));
    add_3->setText("Add");
    add_3->setFont(QFont("Helvetica", 25));
    add_3->setStyleSheet("QToolButton {"
                         "color: white;"
                         "background-color: #555555;"
                         "border-radius: 5px;"
                         "}"
                         "QToolButton:pressed {"
                         "color: white;"
                         "background-color: #1C1E1D;"
                         "border-radius: 5px;"
                         "}");
    connect(add_3, SIGNAL (clicked()), this, SLOT(add3()));
    res_1->setFixedSize(QSize(370, 40));
    res_1->setFont(QFont("Helvetica", 25));
    res_1->setAlignment(Qt::AlignCenter);
    res_1->setReadOnly(true);
    show();
}

void throw_error(QString reason) {
    QMessageBox messageBox;
    messageBox.critical(0, "Error", reason);
    messageBox.setFixedSize(500,200);
}

bool checkInput(QLineEdit * lineEdit) {
    if (lineEdit->text().size() <= 0) {
        return false;
    }
    QString s = lineEdit->text();
    for (int i = 0; i < s.size(); ++i) {
        char x = s[i].cell();
        if (x == ' ') {
            throw_error("Spaces are not allowed. Position: " + QString::number(i));
            return false;
        }
        else if (x == '+' && i + 1 < s.size() && s[i + 1] == '+') {
            throw_error("++ at position " + QString::number(i));
            return false;
        }
        else if (x == '-' && i + 1 < s.size() && s[i + 1] == '-') {
            throw_error("-- at position " + QString::number(i));
            return false;
        }
        else if (x == '-' && i + 1 < s.size() && s[i + 1] == '+') {
            throw_error("-+ at position " + QString::number(i));
            return false;
        }
        else if (x == '+' && i + 1 < s.size() && s[i + 1] == '-') {
            throw_error("+- at position " + QString::number(i));
            return false;
        }
    }
    return true;
}


bool fromLine(QLineEdit * line, Polynomial &a) {
    bool db_bool = false;
    if (line->text().startsWith("db")) {
        db_bool = true;
        std::string res = line->text().toStdString();
        res = remove_spaces(res);
        res = res.substr(2, res.size() - 2);
        if (res.size() == 0) {
            throw_error("Number in db not specified.");
            return false;
        }
        for (char x : res) {
            if (!(x >= '0' && x <= '9')) {
                QMessageBox messageBox;
                messageBox.critical(0,"Parsing error", "DataBase index must be integer");
                messageBox.setFixedSize(500, 200);
                return false;
            }
        }
        try {
            if (stoi(res) < 0 || stoi(res) > db->count) {
                throw_error("This number is not in database.");
                return false;
            }
            a = db->get(stoi(res));
            return true;
        }
        catch (DataBaseException e) {
            QMessageBox messageBox;
            messageBox.critical(0, "Error", QString::fromStdString(e.reason()));
            messageBox.setFixedSize(500,200);
            return false;
        }
        return true;
    }
    if (!checkInput(line)) {
        return false;
    }
    for (auto x : line->text()) {
        if (x == ' ') {
            QMessageBox messageBox;
            messageBox.critical(0, "Error", "Spaces are not available");
            messageBox.setFixedSize(500,200);
            return false;
        }
    }
    try {
        std::string sn = "";
        sn = line->text().toStdString();
        for (int i = 0; i < sn.size(); ++i) {
            if ((sn[i] >= '0' && sn[i] <= '9') || sn[i] == '+' || sn[i] == '-' || sn[i] == '^') {
                continue;
            }
            sn[i] = 'x';
        }
        a = Polynomial(sn);
    }
    catch (InputValidationException e) {
        QMessageBox messageBox;
        messageBox.critical(0, "Parse error", QString::fromStdString(e.reason()));
        messageBox.setFixedSize(500,200);
        return false;
    }
    return true;
}

void MainWindow::derivative() {
    Polynomial cur;
    if (!fromLine(cLine, cur) || nLine->text().size() == 0) {
        return;
    }
    std::string n_str = nLine->text().toStdString();
    for (char x : n_str) {
        if (!(x >= '0' && x <= '9')) {
            throw_error("Bad value in n, only integer expected");
            return;
        }
    }
    int n = stoi(n_str);
//    Polynomial cur = *p_res_1_Cline;
    Polynomial der = cur.n_derivative(n);
    if (oper_1_res) {
        delete oper_1_res;
    }
    oper_1_res = new Polynomial;
    *oper_1_res = der;
    res_1->setText(QString::fromStdString(der.str()));
}

void MainWindow::roots() {
    Polynomial cur;
    if (!fromLine(cLine, cur)) {
        return;
    }
    QString roots = "";
    std::vector<int> rs = cur.roots();
    for (int i = 0; i < rs.size(); ++i) {
        roots += QString::number(rs[i]);
        if (i != rs.size() - 1) {
            roots += ", ";
        }
    }
    oper_1_res = nullptr;
    res_1->setText(roots);
}

void MainWindow::value() {
    Polynomial cur;
    if (!fromLine(cLine, cur) || xLine->text().size() == 0) {
        return;
    }
//    if (!checkInput(cLine) || xLine->text().size() == 0) {
//        return;
//    }
    std::string x_str = xLine->text().toStdString();
    for (char x : x_str) {
        if (!((x >= '0' && x <= '9') || x == '.' || x == '-')) {
            throw_error("Bad value in x, only integer expected");
            return;
        }
    }
    double x = stod(x_str);
//    Polynomial cur = Polynomial(cLine->text().toStdString());
    if (oper_1_res) {
        delete oper_1_res;
    }
    oper_1_res = new Polynomial("1");
    oper_1_res->pol_head->cf = cur.value(x);
    res_1->setText(QString::fromStdString(dtos(oper_1_res->pol_head->cf)));
}

void MainWindow::add1() {
    qDebug() << 1;
    Polynomial c;
    if (!fromLine(cLine, c)) {
        return;
    }
    try {
        Polynomial * r1 = new Polynomial;
        *r1 = c;
        db->add(r1);
        refresh_database();
    }
    catch (DataBaseException e) {
        QMessageBox messageBox;
        messageBox.critical(0,"DataBase error", QString::fromStdString(e.reason()));
        messageBox.setFixedSize(500,200);
    }
}

void MainWindow::add2() {
    qDebug() << 2;
    if (p_res_2 == nullptr || res_2->text() == "True" || res_2->text() == "False") {
        return;
    }
    try {
        Polynomial * r2 = new Polynomial;
        *r2 = *p_res_2;
        db->add(r2);
        refresh_database();
    }
    catch (DataBaseException e) {
        QMessageBox messageBox;
        messageBox.critical(0,"DataBase error", QString::fromStdString(e.reason()));
        messageBox.setFixedSize(500,200);
    }
}

void MainWindow::add3() {
    qDebug() << 3;
    if (!oper_1_res) {
        return;
    }
    try {
//        Polynomial * t = new Polynomial;
//        *t = cur;
        db->add(oper_1_res);
        refresh_database();
    }
    catch (InputValidationException e) {
        QMessageBox messageBox;
        messageBox.critical(0,"Parsing error", QString::fromStdString(e.reason()));
        messageBox.setFixedSize(500,200);
    }
}

void MainWindow::plus() {
    Polynomial a, b;
    try {
        if (!fromLine(aLine, a) || !fromLine(bLine, b)) {
            return;
        }
        p_res_2 = new Polynomial();
        *p_res_2 = sum(a, b).str();
        res_2->setText(QString::fromStdString(p_res_2->str()));
    }
    catch (InputValidationException e) {
        QMessageBox messageBox;
        messageBox.critical(0,"Parsing error", QString::fromStdString(e.reason()));
        messageBox.setFixedSize(500,200);
    }
}

void MainWindow::minus() {
    Polynomial a, b;
    try {
        if (!fromLine(aLine, a) || !fromLine(bLine, b)) {
            return;
        }
        p_res_2 = new Polynomial("0");
        Polynomial res = sum(a, b, true).str();
        *p_res_2 = res;
        res_2->setText(QString::fromStdString(p_res_2->str()));
    }
    catch (InputValidationException e) {
        QMessageBox messageBox;
        messageBox.critical(0,"Parsing error", QString::fromStdString(e.reason()));
        messageBox.setFixedSize(500,200);
    }
}

void MainWindow::mult() {
    Polynomial a, b;
    try {
        if (!fromLine(aLine, a) || !fromLine(bLine, b)) {
            return;
        }
        if (p_res_2) {
            delete p_res_2;
        }
        p_res_2 = new Polynomial();
        *p_res_2 = multiply(a, b);
        res_2->setText(QString::fromStdString(p_res_2->str()));
    }
    catch (InputValidationException e) {
        QMessageBox messageBox;
        messageBox.critical(0,"Parsing error", QString::fromStdString(e.reason()));
        messageBox.setFixedSize(500,200);
    }
}

void MainWindow::div() {
    Polynomial a, b;
    try {
        if (!fromLine(aLine, a) || !fromLine(bLine, b)) {
            return;
        }
        if (p_res_2) {
            delete p_res_2;
        }
        p_res_2 = new Polynomial();
        try {
            std::pair<Polynomial, Polynomial> res_c = divide(a, b);
            *p_res_2 = res_c.first;
            QString ost = "";
            if (res_c.second.len > 1 || (res_c.second.len == 1 && res_c.second.pol_head->cf != 0)) {
                ost = ", r=" + QString::fromStdString(res_c.second.str());
            }
            res_2->setText(QString::fromStdString(p_res_2->str()) + ost);
        }
        catch (OpearionException e) {
            QMessageBox messageBox;
            messageBox.critical(0,"Division error", QString::fromStdString(e.reason()));
            messageBox.setFixedSize(500,200);
        }
    }
    catch (InputValidationException e) {
        QMessageBox messageBox;
        messageBox.critical(0,"Parsing error", QString::fromStdString(e.reason()));
        messageBox.setFixedSize(500,200);
    }
}

void MainWindow::eql() {
    Polynomial a, b;
    try {
        if (!fromLine(aLine, a) || !fromLine(bLine, b)) {
            return;
        }
        res_2->setText(equals(a, b) ? "True" : "False");
        p_res_2 = nullptr;
    }
    catch (InputValidationException e) {
        QMessageBox messageBox;
        messageBox.critical(0,"Parsing error", QString::fromStdString(e.reason()));
        messageBox.setFixedSize(500,200);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
