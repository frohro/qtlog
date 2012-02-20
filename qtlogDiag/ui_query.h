/********************************************************************************
** Form generated from reading UI file 'query.ui'
**
** Created: Thu Feb 16 20:57:55 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUERY_H
#define UI_QUERY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_query
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QPushButton *ButtonHilfe;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QTreeWidget *queryList;
    QLabel *label_3;
    QLineEdit *lineEditQname;
    QLabel *label_4;
    QLabel *label_8;
    QLineEdit *lineEditTrenner;
    QLabel *label_9;
    QLineEdit *lineEditEor;
    QLabel *label_10;
    QLineEdit *lineEditOutfile;
    QSpacerItem *horizontalSpacer;
    QLabel *label_7;
    QLabel *label_5;
    QLineEdit *lineEditQuery;
    QLabel *label_6;
    QPushButton *ButtonSave;
    QLabel *label_21;
    QPushButton *ButtonMkText;
    QPushButton *ButtonMkAdif;
    QPushButton *ButtonExit;
    QWidget *Seite;
    QGridLayout *gridLayout_3;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_24;
    QLabel *labelQuery;
    QSpacerItem *horizontalSpacer_3;
    QTreeWidget *logList;
    QTreeWidget *dataList;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_13;
    QDateEdit *dateEditFrom;
    QLabel *label_17;
    QComboBox *comboBoxDform;
    QLabel *label_14;
    QDateEdit *dateEditTo;
    QLabel *label_18;
    QComboBox *comboBoxTform;
    QLabel *label_15;
    QComboBox *comboBoxSep;
    QLabel *label_16;
    QComboBox *comboBoxOrder;
    QLabel *label_22;
    QComboBox *comboBoxEor;
    QLabel *label_25;
    QLabel *label_23;
    QPushButton *ButtonClear;
    QPushButton *ButtonCopy;
    QPushButton *ButtonBack;
    QWidget *tab;
    QGridLayout *gridLayout_4;
    QTextEdit *textEdit;

    void setupUi(QDialog *query)
    {
        if (query->objectName().isEmpty())
            query->setObjectName(QString::fromUtf8("query"));
        query->resize(787, 494);
        gridLayout = new QGridLayout(query);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(query);
        label->setObjectName(QString::fromUtf8("label"));
        QPalette palette;
        QBrush brush(QColor(36, 110, 228, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(146, 145, 144, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label->setPalette(palette);
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(query);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font1;
        font1.setPointSize(9);
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        ButtonHilfe = new QPushButton(query);
        ButtonHilfe->setObjectName(QString::fromUtf8("ButtonHilfe"));
        ButtonHilfe->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(ButtonHilfe, 0, 2, 1, 1);

        tabWidget = new QTabWidget(query);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setFocusPolicy(Qt::NoFocus);
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        queryList = new QTreeWidget(tab_2);
        queryList->setObjectName(QString::fromUtf8("queryList"));
        queryList->setFocusPolicy(Qt::NoFocus);
        queryList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        queryList->setRootIsDecorated(false);
        queryList->setSortingEnabled(false);
        queryList->setAllColumnsShowFocus(true);

        gridLayout_2->addWidget(queryList, 0, 0, 1, 13);

        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        lineEditQname = new QLineEdit(tab_2);
        lineEditQname->setObjectName(QString::fromUtf8("lineEditQname"));
        QPalette palette1;
        QBrush brush2(QColor(229, 248, 220, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        QBrush brush3(QColor(244, 244, 244, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        lineEditQname->setPalette(palette1);

        gridLayout_2->addWidget(lineEditQname, 1, 1, 1, 1);

        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        label_4->setFont(font2);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_4, 1, 2, 1, 2);

        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 1, 4, 1, 1);

        lineEditTrenner = new QLineEdit(tab_2);
        lineEditTrenner->setObjectName(QString::fromUtf8("lineEditTrenner"));
        QPalette palette2;
        QBrush brush4(QColor(231, 231, 231, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        lineEditTrenner->setPalette(palette2);
        lineEditTrenner->setFocusPolicy(Qt::NoFocus);
        lineEditTrenner->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lineEditTrenner, 1, 5, 1, 2);

        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_9, 1, 7, 1, 1);

        lineEditEor = new QLineEdit(tab_2);
        lineEditEor->setObjectName(QString::fromUtf8("lineEditEor"));
        QPalette palette3;
        QBrush brush5(QColor(230, 230, 230, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        lineEditEor->setPalette(palette3);
        lineEditEor->setFocusPolicy(Qt::NoFocus);
        lineEditEor->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(lineEditEor, 1, 8, 1, 2);

        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font2);
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_10, 1, 10, 1, 1);

        lineEditOutfile = new QLineEdit(tab_2);
        lineEditOutfile->setObjectName(QString::fromUtf8("lineEditOutfile"));

        gridLayout_2->addWidget(lineEditOutfile, 1, 11, 1, 1);

        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 12, 1, 1);

        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QPalette palette4;
        QBrush brush6(QColor(40, 36, 153, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_7->setPalette(palette4);
        QFont font3;
        font3.setPointSize(8);
        label_7->setFont(font3);

        gridLayout_2->addWidget(label_7, 2, 0, 1, 13);

        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font3);

        gridLayout_2->addWidget(label_5, 3, 0, 1, 12);

        lineEditQuery = new QLineEdit(tab_2);
        lineEditQuery->setObjectName(QString::fromUtf8("lineEditQuery"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        lineEditQuery->setPalette(palette5);
        lineEditQuery->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(lineEditQuery, 4, 0, 1, 13);

        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font2);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_6, 5, 0, 1, 1);

        ButtonSave = new QPushButton(tab_2);
        ButtonSave->setObjectName(QString::fromUtf8("ButtonSave"));
        QPalette palette6;
        QBrush brush7(QColor(152, 187, 139, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        ButtonSave->setPalette(palette6);
        ButtonSave->setFont(font2);
        ButtonSave->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(ButtonSave, 5, 1, 1, 1);

        label_21 = new QLabel(tab_2);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        QPalette palette7;
        QBrush brush8(QColor(57, 42, 149, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_21->setPalette(palette7);
        label_21->setFont(font2);
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_21, 5, 2, 1, 1);

        ButtonMkText = new QPushButton(tab_2);
        ButtonMkText->setObjectName(QString::fromUtf8("ButtonMkText"));
        QPalette palette8;
        QBrush brush9(QColor(170, 197, 208, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Button, brush9);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        ButtonMkText->setPalette(palette8);
        ButtonMkText->setFont(font2);
        ButtonMkText->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(ButtonMkText, 5, 3, 1, 3);

        ButtonMkAdif = new QPushButton(tab_2);
        ButtonMkAdif->setObjectName(QString::fromUtf8("ButtonMkAdif"));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::Button, brush9);
        palette9.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette9.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        ButtonMkAdif->setPalette(palette9);
        ButtonMkAdif->setFont(font2);
        ButtonMkAdif->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(ButtonMkAdif, 5, 6, 1, 3);

        ButtonExit = new QPushButton(tab_2);
        ButtonExit->setObjectName(QString::fromUtf8("ButtonExit"));
        QPalette palette10;
        QBrush brush10(QColor(126, 144, 139, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette10.setBrush(QPalette::Active, QPalette::Button, brush10);
        palette10.setBrush(QPalette::Inactive, QPalette::Button, brush10);
        palette10.setBrush(QPalette::Disabled, QPalette::Button, brush10);
        ButtonExit->setPalette(palette10);
        ButtonExit->setFont(font2);
        ButtonExit->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(ButtonExit, 5, 9, 1, 4);

        tabWidget->addTab(tab_2, QString());
        Seite = new QWidget();
        Seite->setObjectName(QString::fromUtf8("Seite"));
        gridLayout_3 = new QGridLayout(Seite);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_11 = new QLabel(Seite);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font2);

        gridLayout_3->addWidget(label_11, 0, 0, 1, 1);

        label_12 = new QLabel(Seite);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font2);
        label_12->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_12, 0, 1, 1, 1);

        label_24 = new QLabel(Seite);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setFont(font2);
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_24, 0, 2, 1, 2);

        labelQuery = new QLabel(Seite);
        labelQuery->setObjectName(QString::fromUtf8("labelQuery"));
        QPalette palette11;
        QBrush brush11(QColor(5, 138, 178, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush11);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush11);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        labelQuery->setPalette(palette11);
        labelQuery->setFont(font2);
        labelQuery->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(labelQuery, 0, 4, 1, 2);

        horizontalSpacer_3 = new QSpacerItem(486, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 0, 6, 1, 3);

        logList = new QTreeWidget(Seite);
        logList->setObjectName(QString::fromUtf8("logList"));
        QPalette palette12;
        QBrush brush12(QColor(225, 236, 250, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette12.setBrush(QPalette::Active, QPalette::Base, brush12);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush12);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        logList->setPalette(palette12);
        logList->setFocusPolicy(Qt::NoFocus);
        logList->setRootIsDecorated(false);

        gridLayout_3->addWidget(logList, 1, 0, 9, 1);

        dataList = new QTreeWidget(Seite);
        dataList->setObjectName(QString::fromUtf8("dataList"));
        QPalette palette13;
        QBrush brush13(QColor(239, 255, 228, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette13.setBrush(QPalette::Active, QPalette::Base, brush13);
        palette13.setBrush(QPalette::Inactive, QPalette::Base, brush13);
        palette13.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        dataList->setPalette(palette13);
        dataList->setFocusPolicy(Qt::NoFocus);
        dataList->setRootIsDecorated(false);

        gridLayout_3->addWidget(dataList, 1, 1, 9, 1);

        label_19 = new QLabel(Seite);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setFont(font2);
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_19, 1, 2, 1, 5);

        label_20 = new QLabel(Seite);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setFont(font2);
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_20, 1, 7, 1, 2);

        label_13 = new QLabel(Seite);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_13, 2, 2, 1, 3);

        dateEditFrom = new QDateEdit(Seite);
        dateEditFrom->setObjectName(QString::fromUtf8("dateEditFrom"));
        QPalette palette14;
        QBrush brush14(QColor(238, 248, 227, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette14.setBrush(QPalette::Active, QPalette::Base, brush14);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush14);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        dateEditFrom->setPalette(palette14);
        dateEditFrom->setFocusPolicy(Qt::NoFocus);
        dateEditFrom->setCalendarPopup(true);

        gridLayout_3->addWidget(dateEditFrom, 2, 5, 1, 2);

        label_17 = new QLabel(Seite);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_17, 2, 7, 1, 1);

        comboBoxDform = new QComboBox(Seite);
        comboBoxDform->setObjectName(QString::fromUtf8("comboBoxDform"));
        QPalette palette15;
        QBrush brush15(QColor(238, 248, 230, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette15.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette15.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette15.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        comboBoxDform->setPalette(palette15);
        comboBoxDform->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(comboBoxDform, 2, 8, 1, 1);

        label_14 = new QLabel(Seite);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_14, 3, 2, 1, 3);

        dateEditTo = new QDateEdit(Seite);
        dateEditTo->setObjectName(QString::fromUtf8("dateEditTo"));
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::Base, brush14);
        palette16.setBrush(QPalette::Inactive, QPalette::Base, brush14);
        palette16.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        dateEditTo->setPalette(palette16);
        dateEditTo->setFocusPolicy(Qt::NoFocus);
        dateEditTo->setCalendarPopup(true);

        gridLayout_3->addWidget(dateEditTo, 3, 5, 1, 2);

        label_18 = new QLabel(Seite);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_18, 3, 7, 1, 1);

        comboBoxTform = new QComboBox(Seite);
        comboBoxTform->setObjectName(QString::fromUtf8("comboBoxTform"));
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette17.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette17.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        comboBoxTform->setPalette(palette17);
        comboBoxTform->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(comboBoxTform, 3, 8, 1, 1);

        label_15 = new QLabel(Seite);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_15, 4, 2, 1, 3);

        comboBoxSep = new QComboBox(Seite);
        comboBoxSep->setObjectName(QString::fromUtf8("comboBoxSep"));
        QPalette palette18;
        QBrush brush16(QColor(200, 218, 198, 255));
        brush16.setStyle(Qt::SolidPattern);
        palette18.setBrush(QPalette::Active, QPalette::Button, brush16);
        palette18.setBrush(QPalette::Inactive, QPalette::Button, brush16);
        palette18.setBrush(QPalette::Disabled, QPalette::Button, brush16);
        comboBoxSep->setPalette(palette18);
        comboBoxSep->setFont(font2);
        comboBoxSep->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(comboBoxSep, 4, 5, 1, 2);

        label_16 = new QLabel(Seite);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_16, 5, 2, 1, 3);

        comboBoxOrder = new QComboBox(Seite);
        comboBoxOrder->setObjectName(QString::fromUtf8("comboBoxOrder"));
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::Button, brush16);
        palette19.setBrush(QPalette::Inactive, QPalette::Button, brush16);
        palette19.setBrush(QPalette::Disabled, QPalette::Button, brush16);
        comboBoxOrder->setPalette(palette19);
        comboBoxOrder->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(comboBoxOrder, 5, 5, 1, 2);

        label_22 = new QLabel(Seite);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_22, 6, 2, 1, 3);

        comboBoxEor = new QComboBox(Seite);
        comboBoxEor->setObjectName(QString::fromUtf8("comboBoxEor"));
        QPalette palette20;
        palette20.setBrush(QPalette::Active, QPalette::Button, brush16);
        palette20.setBrush(QPalette::Inactive, QPalette::Button, brush16);
        palette20.setBrush(QPalette::Disabled, QPalette::Button, brush16);
        comboBoxEor->setPalette(palette20);
        comboBoxEor->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(comboBoxEor, 6, 5, 1, 2);

        label_25 = new QLabel(Seite);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_3->addWidget(label_25, 7, 2, 1, 5);

        label_23 = new QLabel(Seite);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_3->addWidget(label_23, 8, 2, 1, 1);

        ButtonClear = new QPushButton(Seite);
        ButtonClear->setObjectName(QString::fromUtf8("ButtonClear"));
        QPalette palette21;
        QBrush brush17(QColor(189, 189, 189, 255));
        brush17.setStyle(Qt::SolidPattern);
        palette21.setBrush(QPalette::Active, QPalette::Button, brush17);
        palette21.setBrush(QPalette::Inactive, QPalette::Button, brush17);
        palette21.setBrush(QPalette::Disabled, QPalette::Button, brush17);
        ButtonClear->setPalette(palette21);
        ButtonClear->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(ButtonClear, 8, 3, 1, 4);

        ButtonCopy = new QPushButton(Seite);
        ButtonCopy->setObjectName(QString::fromUtf8("ButtonCopy"));
        QPalette palette22;
        palette22.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette22.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette22.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        ButtonCopy->setPalette(palette22);
        ButtonCopy->setFont(font2);
        ButtonCopy->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(ButtonCopy, 9, 3, 1, 4);

        ButtonBack = new QPushButton(Seite);
        ButtonBack->setObjectName(QString::fromUtf8("ButtonBack"));
        QPalette palette23;
        QBrush brush18(QColor(154, 177, 172, 255));
        brush18.setStyle(Qt::SolidPattern);
        palette23.setBrush(QPalette::Active, QPalette::Button, brush18);
        palette23.setBrush(QPalette::Inactive, QPalette::Button, brush18);
        palette23.setBrush(QPalette::Disabled, QPalette::Button, brush18);
        ButtonBack->setPalette(palette23);
        ButtonBack->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(ButtonBack, 9, 7, 1, 2);

        tabWidget->addTab(Seite, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_4 = new QGridLayout(tab);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        textEdit = new QTextEdit(tab);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(textEdit, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 3);


        retranslateUi(query);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(query);
    } // setupUi

    void retranslateUi(QDialog *query)
    {
        query->setWindowTitle(QApplication::translate("query", "Log Export", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("query", "         Query Mgr ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("query", "Logdaten Export f\303\274r externe Auswertungen                                             ", 0, QApplication::UnicodeUTF8));
        ButtonHilfe->setText(QApplication::translate("query", "Hilfe", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = queryList->headerItem();
        ___qtreewidgetitem->setText(4, QApplication::translate("query", "No", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(3, QApplication::translate("query", "Query", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(2, QApplication::translate("query", "eor", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("query", "Trenner", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("query", "Name", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("query", "   Query ", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("query", "      Output :", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("query", "Trenner", 0, QApplication::UnicodeUTF8));
        lineEditTrenner->setText(QString());
        label_9->setText(QApplication::translate("query", "EOR", 0, QApplication::UnicodeUTF8));
        lineEditEor->setText(QString());
        label_10->setText(QApplication::translate("query", " Datei", 0, QApplication::UnicodeUTF8));
        lineEditOutfile->setText(QString());
        label_7->setText(QApplication::translate("query", "Doppelklick in die Tabelle um ein Query f\303\274r die Datenausgabe und zur Bearbeitung zu selektieren.", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("query", "Alle Feldnamen des Logbuchs sind vef\303\274gbar.", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("query", " Query  ", 0, QApplication::UnicodeUTF8));
        ButtonSave->setText(QApplication::translate("query", "Speichern", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("query", "Erzeuge:", 0, QApplication::UnicodeUTF8));
        ButtonMkText->setText(QApplication::translate("query", "CSV Datei", 0, QApplication::UnicodeUTF8));
        ButtonMkAdif->setText(QApplication::translate("query", "ADIF Datei", 0, QApplication::UnicodeUTF8));
        ButtonExit->setText(QApplication::translate("query", "Ende", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("query", "  Query      ", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("query", "     Logbook        ", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("query", "Output", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("query", "Query", 0, QApplication::UnicodeUTF8));
        labelQuery->setText(QApplication::translate("query", "?", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = logList->headerItem();
        ___qtreewidgetitem1->setText(1, QApplication::translate("query", "Name", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(0, QApplication::translate("query", "sel", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = dataList->headerItem();
        ___qtreewidgetitem2->setText(0, QApplication::translate("query", "List Report", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("query", "                  Selektion      ", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("query", "         Datum und Zeitformat", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("query", "Vom", 0, QApplication::UnicodeUTF8));
        dateEditFrom->setDisplayFormat(QApplication::translate("query", "dd.MM.yyyy", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("query", "Datum", 0, QApplication::UnicodeUTF8));
        comboBoxDform->clear();
        comboBoxDform->insertItems(0, QStringList()
         << QApplication::translate("query", "yyyy-MM-dd", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("query", "yyyyMMdd", 0, QApplication::UnicodeUTF8)
        );
        label_14->setText(QApplication::translate("query", "Bis", 0, QApplication::UnicodeUTF8));
        dateEditTo->setDisplayFormat(QApplication::translate("query", "dd.MM.yyyy", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("query", "Zeit", 0, QApplication::UnicodeUTF8));
        comboBoxTform->clear();
        comboBoxTform->insertItems(0, QStringList()
         << QApplication::translate("query", "hh:MM:ss", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("query", "hhMMss", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("query", "hhMM", 0, QApplication::UnicodeUTF8)
        );
        label_15->setText(QApplication::translate("query", "Separator", 0, QApplication::UnicodeUTF8));
        comboBoxSep->clear();
        comboBoxSep->insertItems(0, QStringList()
         << QApplication::translate("query", "      ;", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("query", "      ,", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("query", "     +", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("query", "      :", 0, QApplication::UnicodeUTF8)
        );
        label_16->setText(QApplication::translate("query", "Sortierung nach", 0, QApplication::UnicodeUTF8));
        comboBoxOrder->clear();
        comboBoxOrder->insertItems(0, QStringList()
         << QApplication::translate("query", "Call", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("query", "Band", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("query", "Dxcc", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("query", "Datum", 0, QApplication::UnicodeUTF8)
        );
        label_22->setText(QApplication::translate("query", "EOR", 0, QApplication::UnicodeUTF8));
        comboBoxEor->clear();
        comboBoxEor->insertItems(0, QStringList()
         << QApplication::translate("query", "Linux", 0, QApplication::UnicodeUTF8)
        );
        label_25->setText(QString());
        label_23->setText(QString());
        ButtonClear->setText(QApplication::translate("query", "L\303\266sche Selektion", 0, QApplication::UnicodeUTF8));
        ButtonCopy->setText(QApplication::translate("query", "\303\234bernehmen", 0, QApplication::UnicodeUTF8));
        ButtonBack->setText(QApplication::translate("query", "Zur\303\274ck", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Seite), QApplication::translate("query", " Query Editor      ", 0, QApplication::UnicodeUTF8));
        textEdit->setHtml(QApplication::translate("query", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Kurzinfo und Bedeutung der Felder:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\">Spaltennamen sind die Namen desTabellenheaders und werden durch Kommata getrennt. Zwischen Feldnamen und <span style=\" text-decoration: underline;\">range</span> darf kein Komma stehen.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\">Zur Verf\303\274"
                        "gung stehen alle Namen die im Report Layout aufgef\303\274hrt sind.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"><span style=\" font-weight:600;\">range </span> Hiermit wird der Zeitraum eingeleitet. Das Datum beginnt mit<span style=\" font-weight:600;\"> &lt; </span> und endet mit<span style=\" font-weight:600;\"> &gt; </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\">Beispiel:<span style=\" font-weight:600;\"> range</span> <span style=\" font-weight:600;\">&lt;2008-01-01&gt; to  &lt;2008-12-31&gt;</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"><span style=\" font-weight:600;\">AWD </span> Ist ein Synonym f\303\274r alle Award Typen. DLD steht z.B. f\303\274r DOK. Der AWD Type wird gem\303\244\303\237 der Eins"
                        "tellung im Reportdialog eingef\303\274gt.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"><span style=\" font-weight:600;\">ORDER BY</span> <span style=\" font-weight:600;\">dxcc</span>. Hiermit wird die Sortierung der Tabelle eingeleitet. Die Variable <span style=\" font-weight:600;\"> dxcc </span>bestimmt die Sortierung.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"><span style=\" font-weight:600;\">Query    :</span>  Name des erzeugten Queries.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"><span style=\" font-weight:600;\">Trenner :</span>  Spaltentrenne"
                        "r in einer *.csv Textdatei.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"><span style=\" font-weight:600;\">EOR       :</span>  End of Row. ( Zeilenumbruch )</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"><span style=\" font-weight:600;\">Datei      :  </span>Name der Outputdatei, ist mit dem Querynamen identisch.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\">Die Dateien werden im HOME abgelegt. Je nach Format wird die Endung <span style=\" font-weight:600;\">*.csv</span> oder <span style=\" font-weight:600;\">*.adi</span> angeh\303\244ngt.</p>\n"
"<p styl"
                        "e=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\">Wird der Dateiname entfernt, wird eine Tabellenansicht im Report erzeugt.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("query", "     Info           ", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class query: public Ui_query {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUERY_H
