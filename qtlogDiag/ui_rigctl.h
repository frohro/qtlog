/********************************************************************************
** Form generated from reading UI file 'rigctl.ui'
**
** Created: Fri Feb 24 14:20:25 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RIGCTL_H
#define UI_RIGCTL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_rigctl
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *editRig;
    QPushButton *ButtonRigList;
    QLineEdit *editPid;
    QLabel *label_5;
    QComboBox *RigDevBox;
    QLabel *label_6;
    QComboBox *BaudBox;
    QLabel *label_7;
    QComboBox *PttBitDevBox;
    QLabel *label_8;
    QLabel *label_10;
    QLineEdit *editHost;
    QLabel *label_11;
    QLineEdit *editPort;
    QLabel *label_12;
    QLineEdit *editRetry;
    QPushButton *ButtonInit;
    QPushButton *ButtonStop;
    QPushButton *ButtonStart;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_9;
    QLineEdit *editRts;
    QLineEdit *editDtr;
    QLabel *label_17;
    QLineEdit *editCts;
    QComboBox *InterfaceBox;
    QLabel *label_18;
    QWidget *page_2;
    QTreeWidget *wrigList;
    QLabel *label_13;
    QPushButton *buttonReturn;
    QLabel *label;
    QLabel *label_2;
    QPushButton *ButtonESC;
    QPushButton *ButtonHilfe;

    void setupUi(QDialog *rigctl)
    {
        if (rigctl->objectName().isEmpty())
            rigctl->setObjectName(QString::fromUtf8("rigctl"));
        rigctl->resize(502, 620);
        stackedWidget = new QStackedWidget(rigctl);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(6, 66, 487, 493));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        label_3 = new QLabel(page);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(36, 30, 101, 22));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(page);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(60, 66, 77, 22));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editRig = new QLineEdit(page);
        editRig->setObjectName(QString::fromUtf8("editRig"));
        editRig->setGeometry(QRect(144, 24, 241, 32));
        QPalette palette;
        QBrush brush(QColor(255, 255, 197, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        editRig->setPalette(palette);
        editRig->setFocusPolicy(Qt::NoFocus);
        ButtonRigList = new QPushButton(page);
        ButtonRigList->setObjectName(QString::fromUtf8("ButtonRigList"));
        ButtonRigList->setGeometry(QRect(384, 24, 79, 31));
        ButtonRigList->setFocusPolicy(Qt::NoFocus);
        editPid = new QLineEdit(page);
        editPid->setObjectName(QString::fromUtf8("editPid"));
        editPid->setGeometry(QRect(144, 60, 55, 32));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        editPid->setPalette(palette1);
        editPid->setFont(font);
        editPid->setFocusPolicy(Qt::NoFocus);
        label_5 = new QLabel(page);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(66, 108, 77, 22));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        RigDevBox = new QComboBox(page);
        RigDevBox->setObjectName(QString::fromUtf8("RigDevBox"));
        RigDevBox->setGeometry(QRect(156, 102, 139, 32));
        RigDevBox->setFocusPolicy(Qt::NoFocus);
        label_6 = new QLabel(page);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(48, 144, 97, 22));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        BaudBox = new QComboBox(page);
        BaudBox->setObjectName(QString::fromUtf8("BaudBox"));
        BaudBox->setGeometry(QRect(156, 138, 139, 32));
        BaudBox->setFocusPolicy(Qt::NoFocus);
        label_7 = new QLabel(page);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(12, 222, 133, 22));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PttBitDevBox = new QComboBox(page);
        PttBitDevBox->setObjectName(QString::fromUtf8("PttBitDevBox"));
        PttBitDevBox->setEnabled(true);
        PttBitDevBox->setGeometry(QRect(156, 216, 163, 32));
        QFont font1;
        font1.setPointSize(8);
        PttBitDevBox->setFont(font1);
        PttBitDevBox->setFocusPolicy(Qt::NoFocus);
        label_8 = new QLabel(page);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(12, 258, 133, 22));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_10 = new QLabel(page);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(66, 372, 77, 22));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editHost = new QLineEdit(page);
        editHost->setObjectName(QString::fromUtf8("editHost"));
        editHost->setGeometry(QRect(156, 366, 113, 32));
        label_11 = new QLabel(page);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(66, 402, 77, 22));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editPort = new QLineEdit(page);
        editPort->setObjectName(QString::fromUtf8("editPort"));
        editPort->setGeometry(QRect(156, 396, 113, 32));
        editPort->setFocusPolicy(Qt::NoFocus);
        label_12 = new QLabel(page);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(0, 342, 145, 22));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editRetry = new QLineEdit(page);
        editRetry->setObjectName(QString::fromUtf8("editRetry"));
        editRetry->setGeometry(QRect(156, 336, 67, 32));
        ButtonInit = new QPushButton(page);
        ButtonInit->setObjectName(QString::fromUtf8("ButtonInit"));
        ButtonInit->setGeometry(QRect(30, 444, 121, 31));
        ButtonInit->setFont(font);
        ButtonInit->setFocusPolicy(Qt::NoFocus);
        ButtonStop = new QPushButton(page);
        ButtonStop->setObjectName(QString::fromUtf8("ButtonStop"));
        ButtonStop->setGeometry(QRect(288, 444, 127, 31));
        ButtonStop->setFont(font);
        ButtonStop->setFocusPolicy(Qt::NoFocus);
        ButtonStart = new QPushButton(page);
        ButtonStart->setObjectName(QString::fromUtf8("ButtonStart"));
        ButtonStart->setGeometry(QRect(156, 444, 127, 31));
        ButtonStart->setFont(font);
        ButtonStart->setFocusPolicy(Qt::NoFocus);
        label_14 = new QLabel(page);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(276, 372, 97, 22));
        label_15 = new QLabel(page);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(276, 402, 97, 22));
        label_16 = new QLabel(page);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(234, 342, 49, 22));
        label_9 = new QLabel(page);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(216, 258, 49, 22));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editRts = new QLineEdit(page);
        editRts->setObjectName(QString::fromUtf8("editRts"));
        editRts->setEnabled(true);
        editRts->setGeometry(QRect(156, 252, 49, 32));
        editRts->setFont(font1);
        editDtr = new QLineEdit(page);
        editDtr->setObjectName(QString::fromUtf8("editDtr"));
        editDtr->setEnabled(true);
        editDtr->setGeometry(QRect(270, 252, 49, 32));
        editDtr->setFont(font1);
        label_17 = new QLabel(page);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(90, 294, 55, 22));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editCts = new QLineEdit(page);
        editCts->setObjectName(QString::fromUtf8("editCts"));
        editCts->setEnabled(true);
        editCts->setGeometry(QRect(156, 288, 49, 32));
        editCts->setFont(font1);
        editCts->setFocusPolicy(Qt::NoFocus);
        InterfaceBox = new QComboBox(page);
        InterfaceBox->setObjectName(QString::fromUtf8("InterfaceBox"));
        InterfaceBox->setGeometry(QRect(156, 186, 163, 29));
        InterfaceBox->setFont(font1);
        InterfaceBox->setFocusPolicy(Qt::NoFocus);
        label_18 = new QLabel(page);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(36, 192, 109, 19));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        wrigList = new QTreeWidget(page_2);
        wrigList->setObjectName(QString::fromUtf8("wrigList"));
        wrigList->setGeometry(QRect(6, 48, 475, 397));
        wrigList->setRootIsDecorated(false);
        label_13 = new QLabel(page_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(138, 12, 235, 22));
        QPalette palette2;
        QBrush brush2(QColor(9, 1, 241, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        QBrush brush3(QColor(126, 125, 124, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        label_13->setPalette(palette2);
        buttonReturn = new QPushButton(page_2);
        buttonReturn->setObjectName(QString::fromUtf8("buttonReturn"));
        buttonReturn->setGeometry(QRect(84, 450, 125, 31));
        stackedWidget->addWidget(page_2);
        label = new QLabel(rigctl);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(114, 36, 81, 22));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        label->setFont(font2);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2 = new QLabel(rigctl);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(204, 36, 121, 22));
        ButtonESC = new QPushButton(rigctl);
        ButtonESC->setObjectName(QString::fromUtf8("ButtonESC"));
        ButtonESC->setGeometry(QRect(36, 570, 421, 31));
        QPalette palette3;
        QBrush brush4(QColor(186, 185, 185, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush4);
        ButtonESC->setPalette(palette3);
        ButtonESC->setFocusPolicy(Qt::NoFocus);
        ButtonHilfe = new QPushButton(rigctl);
        ButtonHilfe->setObjectName(QString::fromUtf8("ButtonHilfe"));
        ButtonHilfe->setGeometry(QRect(390, 30, 79, 31));
        ButtonHilfe->setFocusPolicy(Qt::NoFocus);
        QWidget::setTabOrder(editRts, editDtr);
        QWidget::setTabOrder(editDtr, editRetry);
        QWidget::setTabOrder(editRetry, editHost);
        QWidget::setTabOrder(editHost, editPort);
        QWidget::setTabOrder(editPort, wrigList);
        QWidget::setTabOrder(wrigList, buttonReturn);

        retranslateUi(rigctl);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(rigctl);
    } // setupUi

    void retranslateUi(QDialog *rigctl)
    {
        rigctl->setWindowTitle(QApplication::translate("rigctl", "Dialog", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("rigctl", "Rigmodel", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("rigctl", "Pid", 0, QApplication::UnicodeUTF8));
        ButtonRigList->setText(QApplication::translate("rigctl", "w\303\244hle", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("rigctl", "Device", 0, QApplication::UnicodeUTF8));
        RigDevBox->clear();
        RigDevBox->insertItems(0, QStringList()
         << QApplication::translate("rigctl", "/dev/ttyS0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "/dev/ttyS1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "/dev/ttyS2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "/dev/ttyS3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "/dev/ttyUSB0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "/dev/ttyUSB1", 0, QApplication::UnicodeUTF8)
        );
        label_6->setText(QApplication::translate("rigctl", "Baudrate", 0, QApplication::UnicodeUTF8));
        BaudBox->clear();
        BaudBox->insertItems(0, QStringList()
         << QApplication::translate("rigctl", "4800", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "9600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "19200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "38400", 0, QApplication::UnicodeUTF8)
        );
        label_7->setText(QApplication::translate("rigctl", "Hardware PTT", 0, QApplication::UnicodeUTF8));
        PttBitDevBox->clear();
        PttBitDevBox->insertItems(0, QStringList()
         << QApplication::translate("rigctl", "/dev/ttyS0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "/dev/ttyS1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "/dev/ttyS2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "/dev/ttyS3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "/dev/ttyUSB0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "/dev/ttyUSB1", 0, QApplication::UnicodeUTF8)
        );
        label_8->setText(QApplication::translate("rigctl", "Use state RTS", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("rigctl", "Host", 0, QApplication::UnicodeUTF8));
        editHost->setText(QApplication::translate("rigctl", "localhost", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("rigctl", "Port", 0, QApplication::UnicodeUTF8));
        editPort->setText(QApplication::translate("rigctl", "4532", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("rigctl", "Retry Interval", 0, QApplication::UnicodeUTF8));
        editRetry->setText(QApplication::translate("rigctl", "1000", 0, QApplication::UnicodeUTF8));
        ButtonInit->setText(QApplication::translate("rigctl", "Initialice", 0, QApplication::UnicodeUTF8));
        ButtonStop->setText(QApplication::translate("rigctl", "Stop", 0, QApplication::UnicodeUTF8));
        ButtonStart->setText(QApplication::translate("rigctl", "Start", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("rigctl", "( default )", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("rigctl", "( default )", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("rigctl", "ms", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("rigctl", "DTR", 0, QApplication::UnicodeUTF8));
        editRts->setText(QApplication::translate("rigctl", "OFF", 0, QApplication::UnicodeUTF8));
        editDtr->setText(QApplication::translate("rigctl", "OFF", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("rigctl", "CTS", 0, QApplication::UnicodeUTF8));
        editCts->setText(QApplication::translate("rigctl", "OFF", 0, QApplication::UnicodeUTF8));
        InterfaceBox->clear();
        InterfaceBox->insertItems(0, QStringList()
         << QApplication::translate("rigctl", "NO", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("rigctl", "Funkamateur ONE", 0, QApplication::UnicodeUTF8)
        );
        label_18->setText(QApplication::translate("rigctl", "Interface", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = wrigList->headerItem();
        ___qtreewidgetitem->setText(4, QApplication::translate("rigctl", "Status", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(3, QApplication::translate("rigctl", "Version", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(2, QApplication::translate("rigctl", "Model", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("rigctl", "Mfg", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("rigctl", "RigPid", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("rigctl", "Server RigModel support", 0, QApplication::UnicodeUTF8));
        buttonReturn->setText(QApplication::translate("rigctl", "Return", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("rigctl", "Hamlib", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("rigctl", "  -  Rigcontrol", 0, QApplication::UnicodeUTF8));
        ButtonESC->setText(QApplication::translate("rigctl", "ESC", 0, QApplication::UnicodeUTF8));
        ButtonHilfe->setText(QApplication::translate("rigctl", "Hilfe", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class rigctl: public Ui_rigctl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RIGCTL_H
