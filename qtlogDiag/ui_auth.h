/********************************************************************************
** Form generated from reading UI file 'auth.ui'
**
** Created: Thu Feb 16 20:57:55 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTH_H
#define UI_AUTH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_auth
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout1;
    QTextEdit *authtextEdit;
    QWidget *page_2;
    QPushButton *ButtonEnde;

    void setupUi(QDialog *auth)
    {
        if (auth->objectName().isEmpty())
            auth->setObjectName(QString::fromUtf8("auth"));
        auth->resize(545, 333);
        gridLayout = new QGridLayout(auth);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(auth);
        label->setObjectName(QString::fromUtf8("label"));
        QPalette palette;
        QBrush brush(QColor(125, 113, 2, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(119, 119, 119, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label->setPalette(palette);
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        stackedWidget = new QStackedWidget(auth);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 537, 255));
        gridLayout1 = new QGridLayout(page);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        authtextEdit = new QTextEdit(page);
        authtextEdit->setObjectName(QString::fromUtf8("authtextEdit"));
        QPalette palette1;
        QBrush brush2(QColor(255, 254, 239, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        QBrush brush3(QColor(255, 255, 255, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        authtextEdit->setPalette(palette1);
        authtextEdit->setFocusPolicy(Qt::NoFocus);

        gridLayout1->addWidget(authtextEdit, 0, 0, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 100, 30));
        stackedWidget->addWidget(page_2);

        gridLayout->addWidget(stackedWidget, 1, 0, 1, 1);

        ButtonEnde = new QPushButton(auth);
        ButtonEnde->setObjectName(QString::fromUtf8("ButtonEnde"));
        QPalette palette2;
        QBrush brush4(QColor(178, 220, 224, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush4);
        ButtonEnde->setPalette(palette2);
        ButtonEnde->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(ButtonEnde, 2, 0, 1, 1);


        retranslateUi(auth);

        QMetaObject::connectSlotsByName(auth);
    } // setupUi

    void retranslateUi(QDialog *auth)
    {
        auth->setWindowTitle(QApplication::translate("auth", "QtLog  Info", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("auth", "     QtLog", 0, QApplication::UnicodeUTF8));
        ButtonEnde->setText(QApplication::translate("auth", "ENDE", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class auth: public Ui_auth {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTH_H
