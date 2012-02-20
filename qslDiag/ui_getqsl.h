/********************************************************************************
** Form generated from reading ui file 'getqsl.ui'
**
** Created: Thu Jun 11 12:22:44 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GETQSL_H
#define UI_GETQSL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_getqsl
{
public:
    QLabel *label;
    QCheckBox *checkBox_7;
    QCheckBox *checkBox_1;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QLabel *label_7;
    QPushButton *ButtonGetRecord;
    QPushButton *ButtonExit;
    QFrame *line;
    QLineEdit *lineEdit_4;
    QDateEdit *dateEdit;

    void setupUi(QDialog *getqsl)
    {
    if (getqsl->objectName().isEmpty())
        getqsl->setObjectName(QString::fromUtf8("getqsl"));
    getqsl->resize(599, 480);
    label = new QLabel(getqsl);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(92, 32, 441, 37));
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    label->setFont(font);
    checkBox_7 = new QCheckBox(getqsl);
    checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));
    checkBox_7->setGeometry(QRect(100, 352, 253, 23));
    checkBox_1 = new QCheckBox(getqsl);
    checkBox_1->setObjectName(QString::fromUtf8("checkBox_1"));
    checkBox_1->setGeometry(QRect(100, 180, 405, 23));
    checkBox_1->setChecked(true);
    checkBox_2 = new QCheckBox(getqsl);
    checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
    checkBox_2->setGeometry(QRect(100, 220, 205, 23));
    checkBox_3 = new QCheckBox(getqsl);
    checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
    checkBox_3->setGeometry(QRect(100, 252, 209, 23));
    checkBox_4 = new QCheckBox(getqsl);
    checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
    checkBox_4->setGeometry(QRect(100, 276, 217, 23));
    checkBox_5 = new QCheckBox(getqsl);
    checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));
    checkBox_5->setGeometry(QRect(100, 300, 217, 23));
    checkBox_6 = new QCheckBox(getqsl);
    checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));
    checkBox_6->setGeometry(QRect(100, 324, 217, 23));
    label_7 = new QLabel(getqsl);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    label_7->setGeometry(QRect(68, 132, 309, 23));
    label_7->setFont(font);
    ButtonGetRecord = new QPushButton(getqsl);
    ButtonGetRecord->setObjectName(QString::fromUtf8("ButtonGetRecord"));
    ButtonGetRecord->setGeometry(QRect(68, 424, 217, 32));
    QPalette palette;
    QBrush brush(QColor(198, 219, 191, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Button, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
    ButtonGetRecord->setPalette(palette);
    ButtonGetRecord->setFont(font);
    ButtonExit = new QPushButton(getqsl);
    ButtonExit->setObjectName(QString::fromUtf8("ButtonExit"));
    ButtonExit->setGeometry(QRect(300, 424, 233, 32));
    QPalette palette1;
    QBrush brush1(QColor(197, 196, 196, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
    palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
    palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
    ButtonExit->setPalette(palette1);
    ButtonExit->setFont(font);
    line = new QFrame(getqsl);
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(60, 80, 465, 16));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    lineEdit_4 = new QLineEdit(getqsl);
    lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
    lineEdit_4->setGeometry(QRect(320, 244, 113, 33));
    dateEdit = new QDateEdit(getqsl);
    dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
    dateEdit->setGeometry(QRect(320, 216, 133, 33));
    QPalette palette2;
    QBrush brush2(QColor(228, 244, 228, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Active, QPalette::Base, brush2);
    palette2.setBrush(QPalette::Inactive, QPalette::Base, brush2);
    QBrush brush3(QColor(244, 244, 244, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Disabled, QPalette::Base, brush3);
    dateEdit->setPalette(palette2);
    dateEdit->setCalendarPopup(true);

    retranslateUi(getqsl);

    QMetaObject::connectSlotsByName(getqsl);
    } // setupUi

    void retranslateUi(QDialog *getqsl)
    {
    getqsl->setWindowTitle(QApplication::translate("getqsl", "Dialog", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("getqsl", "QSL - DownLoad   < Logbook of The World >", 0, QApplication::UnicodeUTF8));
    checkBox_7->setText(QApplication::translate("getqsl", " 7  Alle best\303\244tigten QSOs", 0, QApplication::UnicodeUTF8));
    checkBox_1->setText(QApplication::translate("getqsl", " 1  Best\303\244tigte QSOs ab   [Last Upload - Datum ]", 0, QApplication::UnicodeUTF8));
    checkBox_2->setText(QApplication::translate("getqsl", " 2  QSL  ab [ Datum ]", 0, QApplication::UnicodeUTF8));
    checkBox_3->setText(QApplication::translate("getqsl", " 3  QSL  f\303\274r [ CALL   ]", 0, QApplication::UnicodeUTF8));
    checkBox_4->setText(QApplication::translate("getqsl", " 4  QSL  f\303\274r [ MODE ]", 0, QApplication::UnicodeUTF8));
    checkBox_5->setText(QApplication::translate("getqsl", " 5  QSL  f\303\274r [ BAND  ]", 0, QApplication::UnicodeUTF8));
    checkBox_6->setText(QApplication::translate("getqsl", " 6  QSL  f\303\274r [ DXCC  ]", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("getqsl", "Abfrage Queries  ( QSL records )", 0, QApplication::UnicodeUTF8));
    ButtonGetRecord->setText(QApplication::translate("getqsl", "Download", 0, QApplication::UnicodeUTF8));
    ButtonExit->setText(QApplication::translate("getqsl", "Zur\303\274ck", 0, QApplication::UnicodeUTF8));
    dateEdit->setDisplayFormat(QApplication::translate("getqsl", "dd.MM.yyyy", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(getqsl);
    } // retranslateUi

};

namespace Ui {
    class getqsl: public Ui_getqsl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETQSL_H
