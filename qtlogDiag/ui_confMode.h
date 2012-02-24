/********************************************************************************
** Form generated from reading UI file 'confMode.ui'
**
** Created: Fri Feb 24 14:20:25 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFMODE_H
#define UI_CONFMODE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_confMode
{
public:
    QGridLayout *gridLayout_4;
    QSpacerItem *spacerItem;
    QLabel *label;
    QPushButton *ButtonHilfe;
    QPushButton *SavePushButton;
    QPushButton *NewPushButton;
    QPushButton *pushButtonExit;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QTreeWidget *bandList;
    QTreeWidget *modeList;
    QTreeWidget *pwrList;
    QSpacerItem *spacerItem1;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QSpacerItem *spacerItem2;
    QLineEdit *BandSetLineEdit;
    QLineEdit *BandMyBandLineEdit;
    QLineEdit *BandFreqLineEdit;
    QLineEdit *BandRigLineEdit;
    QSpacerItem *horizontalSpacer;
    QLineEdit *ModeSetLineEdit;
    QSpacerItem *spacerItem3;
    QLineEdit *PwrWorkLineEdit;
    QLineEdit *PwrWattLineEdit;
    QSpacerItem *spacerItem4;
    QWidget *tab_2;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label_24;
    QTreeWidget *lineList;
    QTreeWidget *awdList;
    QTreeWidget *customsList;
    QSpacerItem *spacerItem5;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *labelUsrName;
    QLineEdit *LineNameEdit;
    QLineEdit *AwdSetLineEdit;
    QSpacerItem *spacerItem6;
    QLineEdit *NameTypeLineEdit;
    QWidget *Seite;
    QGridLayout *gridLayout_3;
    QLabel *label_17;
    QTreeWidget *rigList;
    QWidget *Seite_2;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_22;
    QLineEdit *RigNameLineEdit;
    QLineEdit *RigTransLineEdit;
    QLineEdit *RigAntLineEdit;
    QLineEdit *RigOpLineEdit;
    QLineEdit *RigSpezLineEdit;
    QPushButton *RigClearPushButton;
    QPushButton *RigSavePushButton;
    QLabel *label_23;

    void setupUi(QDialog *confMode)
    {
        if (confMode->objectName().isEmpty())
            confMode->setObjectName(QString::fromUtf8("confMode"));
        confMode->resize(767, 501);
        gridLayout_4 = new QGridLayout(confMode);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(spacerItem, 0, 2, 1, 1);

        label = new QLabel(confMode);
        label->setObjectName(QString::fromUtf8("label"));
        QPalette palette;
        QBrush brush(QColor(88, 117, 209, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(119, 119, 119, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label->setPalette(palette);
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout_4->addWidget(label, 0, 0, 1, 2);

        ButtonHilfe = new QPushButton(confMode);
        ButtonHilfe->setObjectName(QString::fromUtf8("ButtonHilfe"));
        ButtonHilfe->setFocusPolicy(Qt::NoFocus);
        ButtonHilfe->setFlat(true);

        gridLayout_4->addWidget(ButtonHilfe, 0, 3, 1, 1);

        SavePushButton = new QPushButton(confMode);
        SavePushButton->setObjectName(QString::fromUtf8("SavePushButton"));
        QPalette palette1;
        QBrush brush2(QColor(206, 215, 238, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush2);
        SavePushButton->setPalette(palette1);
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        SavePushButton->setFont(font1);
        SavePushButton->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(SavePushButton, 2, 0, 1, 1);

        NewPushButton = new QPushButton(confMode);
        NewPushButton->setObjectName(QString::fromUtf8("NewPushButton"));
        QPalette palette2;
        QBrush brush3(QColor(209, 238, 213, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        NewPushButton->setPalette(palette2);
        NewPushButton->setFont(font1);

        gridLayout_4->addWidget(NewPushButton, 2, 1, 1, 1);

        pushButtonExit = new QPushButton(confMode);
        pushButtonExit->setObjectName(QString::fromUtf8("pushButtonExit"));
        QPalette palette3;
        QBrush brush4(QColor(175, 215, 217, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush4);
        pushButtonExit->setPalette(palette3);
        pushButtonExit->setFont(font1);
        pushButtonExit->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(pushButtonExit, 2, 2, 1, 2);

        tabWidget = new QTabWidget(confMode);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setFont(font1);
        tabWidget->setFocusPolicy(Qt::NoFocus);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_5->setPalette(palette4);

        gridLayout_2->addWidget(label_5, 0, 0, 1, 5);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_6->setPalette(palette5);

        gridLayout_2->addWidget(label_6, 0, 5, 1, 2);

        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_7->setPalette(palette6);

        gridLayout_2->addWidget(label_7, 0, 7, 1, 2);

        bandList = new QTreeWidget(tab);
        bandList->setObjectName(QString::fromUtf8("bandList"));
        QPalette palette7;
        QBrush brush5(QColor(239, 255, 239, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush5);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush5);
        QBrush brush6(QColor(244, 244, 244, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        bandList->setPalette(palette7);
        QFont font2;
        font2.setBold(false);
        font2.setWeight(50);
        bandList->setFont(font2);
        bandList->setFocusPolicy(Qt::NoFocus);
        bandList->setRootIsDecorated(false);

        gridLayout_2->addWidget(bandList, 1, 0, 1, 5);

        modeList = new QTreeWidget(tab);
        modeList->setObjectName(QString::fromUtf8("modeList"));
        QPalette palette8;
        QBrush brush7(QColor(253, 255, 221, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush7);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        modeList->setPalette(palette8);
        modeList->setFont(font2);
        modeList->setFocusPolicy(Qt::NoFocus);
        modeList->setRootIsDecorated(false);

        gridLayout_2->addWidget(modeList, 1, 5, 1, 2);

        pwrList = new QTreeWidget(tab);
        pwrList->setObjectName(QString::fromUtf8("pwrList"));
        pwrList->setFont(font2);
        pwrList->setFocusPolicy(Qt::NoFocus);
        pwrList->setRootIsDecorated(false);

        gridLayout_2->addWidget(pwrList, 1, 7, 1, 2);

        spacerItem1 = new QSpacerItem(25, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(spacerItem1, 1, 9, 1, 1);

        label_11 = new QLabel(tab);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 2, 0, 1, 1);

        label_12 = new QLabel(tab);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font2);

        gridLayout_2->addWidget(label_12, 2, 1, 1, 1);

        label_13 = new QLabel(tab);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font2);

        gridLayout_2->addWidget(label_13, 2, 2, 1, 1);

        label_10 = new QLabel(tab);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font2);

        gridLayout_2->addWidget(label_10, 2, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 2, 4, 1, 1);

        label_14 = new QLabel(tab);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 2, 5, 1, 1);

        label_15 = new QLabel(tab);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_2->addWidget(label_15, 2, 7, 1, 1);

        label_16 = new QLabel(tab);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setFont(font2);

        gridLayout_2->addWidget(label_16, 2, 8, 1, 1);

        spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(spacerItem2, 2, 9, 1, 1);

        BandSetLineEdit = new QLineEdit(tab);
        BandSetLineEdit->setObjectName(QString::fromUtf8("BandSetLineEdit"));
        QPalette palette9;
        QBrush brush8(QColor(233, 255, 239, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette9.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette9.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette9.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        BandSetLineEdit->setPalette(palette9);
        BandSetLineEdit->setFont(font2);
        BandSetLineEdit->setFocusPolicy(Qt::StrongFocus);

        gridLayout_2->addWidget(BandSetLineEdit, 3, 0, 1, 1);

        BandMyBandLineEdit = new QLineEdit(tab);
        BandMyBandLineEdit->setObjectName(QString::fromUtf8("BandMyBandLineEdit"));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette10.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette10.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        BandMyBandLineEdit->setPalette(palette10);
        BandMyBandLineEdit->setFont(font2);
        BandMyBandLineEdit->setFocusPolicy(Qt::StrongFocus);

        gridLayout_2->addWidget(BandMyBandLineEdit, 3, 1, 1, 1);

        BandFreqLineEdit = new QLineEdit(tab);
        BandFreqLineEdit->setObjectName(QString::fromUtf8("BandFreqLineEdit"));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        BandFreqLineEdit->setPalette(palette11);
        BandFreqLineEdit->setFont(font2);
        BandFreqLineEdit->setFocusPolicy(Qt::StrongFocus);

        gridLayout_2->addWidget(BandFreqLineEdit, 3, 2, 1, 1);

        BandRigLineEdit = new QLineEdit(tab);
        BandRigLineEdit->setObjectName(QString::fromUtf8("BandRigLineEdit"));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        BandRigLineEdit->setPalette(palette12);
        BandRigLineEdit->setFont(font2);

        gridLayout_2->addWidget(BandRigLineEdit, 3, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 3, 4, 1, 1);

        ModeSetLineEdit = new QLineEdit(tab);
        ModeSetLineEdit->setObjectName(QString::fromUtf8("ModeSetLineEdit"));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::Base, brush7);
        palette13.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette13.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        ModeSetLineEdit->setPalette(palette13);
        ModeSetLineEdit->setFont(font2);
        ModeSetLineEdit->setFocusPolicy(Qt::StrongFocus);

        gridLayout_2->addWidget(ModeSetLineEdit, 3, 5, 1, 1);

        spacerItem3 = new QSpacerItem(49, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(spacerItem3, 3, 6, 1, 1);

        PwrWorkLineEdit = new QLineEdit(tab);
        PwrWorkLineEdit->setObjectName(QString::fromUtf8("PwrWorkLineEdit"));
        QPalette palette14;
        QBrush brush9(QColor(255, 248, 238, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette14.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        PwrWorkLineEdit->setPalette(palette14);
        PwrWorkLineEdit->setFont(font2);
        PwrWorkLineEdit->setFocusPolicy(Qt::StrongFocus);

        gridLayout_2->addWidget(PwrWorkLineEdit, 3, 7, 1, 1);

        PwrWattLineEdit = new QLineEdit(tab);
        PwrWattLineEdit->setObjectName(QString::fromUtf8("PwrWattLineEdit"));
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette15.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette15.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        PwrWattLineEdit->setPalette(palette15);
        PwrWattLineEdit->setFont(font2);
        PwrWattLineEdit->setFocusPolicy(Qt::StrongFocus);

        gridLayout_2->addWidget(PwrWattLineEdit, 3, 8, 1, 1);

        spacerItem4 = new QSpacerItem(81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(spacerItem4, 3, 9, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout = new QGridLayout(tab_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 0, 1, 6);

        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_3->setPalette(palette16);

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette17.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_2->setPalette(palette17);

        gridLayout->addWidget(label_2, 1, 1, 1, 2);

        label_24 = new QLabel(tab_2);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush10(QColor(146, 145, 144, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette18.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        label_24->setPalette(palette18);

        gridLayout->addWidget(label_24, 1, 3, 1, 1);

        lineList = new QTreeWidget(tab_2);
        lineList->setObjectName(QString::fromUtf8("lineList"));
        lineList->setFont(font2);
        lineList->setFocusPolicy(Qt::NoFocus);
        lineList->setRootIsDecorated(false);

        gridLayout->addWidget(lineList, 2, 0, 1, 1);

        awdList = new QTreeWidget(tab_2);
        awdList->setObjectName(QString::fromUtf8("awdList"));
        awdList->setFont(font2);
        awdList->setFocusPolicy(Qt::NoFocus);
        awdList->setRootIsDecorated(false);

        gridLayout->addWidget(awdList, 2, 1, 1, 2);

        customsList = new QTreeWidget(tab_2);
        customsList->setObjectName(QString::fromUtf8("customsList"));
        customsList->setFont(font2);
        customsList->setRootIsDecorated(false);

        gridLayout->addWidget(customsList, 2, 3, 1, 1);

        spacerItem5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem5, 2, 5, 1, 1);

        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 3, 0, 1, 1);

        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_9, 3, 1, 1, 1);

        labelUsrName = new QLabel(tab_2);
        labelUsrName->setObjectName(QString::fromUtf8("labelUsrName"));

        gridLayout->addWidget(labelUsrName, 3, 3, 1, 1);

        LineNameEdit = new QLineEdit(tab_2);
        LineNameEdit->setObjectName(QString::fromUtf8("LineNameEdit"));
        LineNameEdit->setFont(font2);

        gridLayout->addWidget(LineNameEdit, 4, 0, 1, 1);

        AwdSetLineEdit = new QLineEdit(tab_2);
        AwdSetLineEdit->setObjectName(QString::fromUtf8("AwdSetLineEdit"));
        AwdSetLineEdit->setFont(font2);

        gridLayout->addWidget(AwdSetLineEdit, 4, 1, 1, 1);

        spacerItem6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem6, 4, 4, 1, 1);

        NameTypeLineEdit = new QLineEdit(tab_2);
        NameTypeLineEdit->setObjectName(QString::fromUtf8("NameTypeLineEdit"));
        NameTypeLineEdit->setEnabled(true);
        NameTypeLineEdit->setFont(font2);

        gridLayout->addWidget(NameTypeLineEdit, 4, 2, 1, 2);

        tabWidget->addTab(tab_2, QString());
        Seite = new QWidget();
        Seite->setObjectName(QString::fromUtf8("Seite"));
        gridLayout_3 = new QGridLayout(Seite);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_17 = new QLabel(Seite);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette19.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_17->setPalette(palette19);

        gridLayout_3->addWidget(label_17, 0, 0, 1, 1);

        rigList = new QTreeWidget(Seite);
        rigList->setObjectName(QString::fromUtf8("rigList"));
        rigList->setFont(font2);
        rigList->setFocusPolicy(Qt::NoFocus);
        rigList->setRootIsDecorated(false);

        gridLayout_3->addWidget(rigList, 1, 0, 1, 1);

        tabWidget->addTab(Seite, QString());
        Seite_2 = new QWidget();
        Seite_2->setObjectName(QString::fromUtf8("Seite_2"));
        label_18 = new QLabel(Seite_2);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 95, 111, 18));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_19 = new QLabel(Seite_2);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(15, 120, 106, 21));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_20 = new QLabel(Seite_2);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(15, 160, 106, 18));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_21 = new QLabel(Seite_2);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(5, 190, 111, 18));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_22 = new QLabel(Seite_2);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(20, 225, 96, 18));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        RigNameLineEdit = new QLineEdit(Seite_2);
        RigNameLineEdit->setObjectName(QString::fromUtf8("RigNameLineEdit"));
        RigNameLineEdit->setGeometry(QRect(128, 88, 113, 31));
        RigNameLineEdit->setFont(font2);
        RigTransLineEdit = new QLineEdit(Seite_2);
        RigTransLineEdit->setObjectName(QString::fromUtf8("RigTransLineEdit"));
        RigTransLineEdit->setGeometry(QRect(128, 120, 241, 31));
        RigTransLineEdit->setFont(font2);
        RigAntLineEdit = new QLineEdit(Seite_2);
        RigAntLineEdit->setObjectName(QString::fromUtf8("RigAntLineEdit"));
        RigAntLineEdit->setGeometry(QRect(128, 152, 241, 31));
        RigAntLineEdit->setFont(font2);
        RigOpLineEdit = new QLineEdit(Seite_2);
        RigOpLineEdit->setObjectName(QString::fromUtf8("RigOpLineEdit"));
        RigOpLineEdit->setGeometry(QRect(128, 184, 241, 31));
        RigOpLineEdit->setFont(font2);
        RigSpezLineEdit = new QLineEdit(Seite_2);
        RigSpezLineEdit->setObjectName(QString::fromUtf8("RigSpezLineEdit"));
        RigSpezLineEdit->setGeometry(QRect(128, 216, 113, 31));
        RigSpezLineEdit->setFont(font2);
        RigClearPushButton = new QPushButton(Seite_2);
        RigClearPushButton->setObjectName(QString::fromUtf8("RigClearPushButton"));
        RigClearPushButton->setGeometry(QRect(128, 264, 91, 41));
        QPalette palette20;
        QBrush brush11(QColor(215, 214, 214, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette20.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette20.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette20.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        RigClearPushButton->setPalette(palette20);
        RigClearPushButton->setFont(font2);
        RigClearPushButton->setFocusPolicy(Qt::NoFocus);
        RigSavePushButton = new QPushButton(Seite_2);
        RigSavePushButton->setObjectName(QString::fromUtf8("RigSavePushButton"));
        RigSavePushButton->setGeometry(QRect(236, 264, 125, 41));
        QPalette palette21;
        QBrush brush12(QColor(190, 214, 181, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette21.setBrush(QPalette::Active, QPalette::Button, brush12);
        palette21.setBrush(QPalette::Inactive, QPalette::Button, brush12);
        palette21.setBrush(QPalette::Disabled, QPalette::Button, brush12);
        RigSavePushButton->setPalette(palette21);
        RigSavePushButton->setFont(font1);
        RigSavePushButton->setFocusPolicy(Qt::NoFocus);
        label_23 = new QLabel(Seite_2);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(88, 40, 345, 18));
        QPalette palette22;
        palette22.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette22.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_23->setPalette(palette22);
        QFont font3;
        font3.setPointSize(10);
        label_23->setFont(font3);
        tabWidget->addTab(Seite_2, QString());

        gridLayout_4->addWidget(tabWidget, 1, 0, 1, 4);

        QWidget::setTabOrder(BandSetLineEdit, BandMyBandLineEdit);
        QWidget::setTabOrder(BandMyBandLineEdit, BandFreqLineEdit);
        QWidget::setTabOrder(BandFreqLineEdit, ModeSetLineEdit);
        QWidget::setTabOrder(ModeSetLineEdit, PwrWorkLineEdit);
        QWidget::setTabOrder(PwrWorkLineEdit, PwrWattLineEdit);
        QWidget::setTabOrder(PwrWattLineEdit, LineNameEdit);
        QWidget::setTabOrder(LineNameEdit, AwdSetLineEdit);
        QWidget::setTabOrder(AwdSetLineEdit, NameTypeLineEdit);
        QWidget::setTabOrder(NameTypeLineEdit, RigNameLineEdit);
        QWidget::setTabOrder(RigNameLineEdit, RigTransLineEdit);
        QWidget::setTabOrder(RigTransLineEdit, RigAntLineEdit);
        QWidget::setTabOrder(RigAntLineEdit, RigOpLineEdit);
        QWidget::setTabOrder(RigOpLineEdit, RigSpezLineEdit);

        retranslateUi(confMode);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(confMode);
    } // setupUi

    void retranslateUi(QDialog *confMode)
    {
        confMode->setWindowTitle(QApplication::translate("confMode", "Betriebsarten", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("confMode", "Betriebsarten Layout", 0, QApplication::UnicodeUTF8));
        ButtonHilfe->setText(QApplication::translate("confMode", "Hilfe", 0, QApplication::UnicodeUTF8));
        SavePushButton->setText(QApplication::translate("confMode", "Speichern", 0, QApplication::UnicodeUTF8));
        NewPushButton->setText(QApplication::translate("confMode", "Neu", 0, QApplication::UnicodeUTF8));
        pushButtonExit->setText(QApplication::translate("confMode", " F10   -  ENDE", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("confMode", "   Band", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("confMode", "  Mode", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("confMode", "  PWR", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = bandList->headerItem();
        ___qtreewidgetitem->setText(4, QApplication::translate("confMode", "Band>Rig", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(3, QApplication::translate("confMode", "Freq", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(2, QApplication::translate("confMode", "MyBand", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("confMode", "Set", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("confMode", "SysBand", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = modeList->headerItem();
        ___qtreewidgetitem1->setText(1, QApplication::translate("confMode", "Mode", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(0, QApplication::translate("confMode", "Set", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = pwrList->headerItem();
        ___qtreewidgetitem2->setText(2, QApplication::translate("confMode", "Watt", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(1, QApplication::translate("confMode", "work", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(0, QApplication::translate("confMode", "id", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("confMode", "    Set", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("confMode", " MyBand", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("confMode", "   Freq", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("confMode", "   Rig", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("confMode", "        Set", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("confMode", "  work", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("confMode", "  Watt", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("confMode", "Band - Mode - PWR  ", 0, QApplication::UnicodeUTF8));
        label_4->setText(QString());
        label_3->setText(QApplication::translate("confMode", "  Verbindungs Arten", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("confMode", "   Award Typen", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("confMode", "Customs-Felder", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem3 = lineList->headerItem();
        ___qtreewidgetitem3->setText(1, QApplication::translate("confMode", "Verbindungs-Name", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem3->setText(0, QApplication::translate("confMode", "set", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem4 = awdList->headerItem();
        ___qtreewidgetitem4->setText(1, QApplication::translate("confMode", "Award Name", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem4->setText(0, QApplication::translate("confMode", "Sort", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem5 = customsList->headerItem();
        ___qtreewidgetitem5->setText(1, QApplication::translate("confMode", "User Feld Name", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem5->setText(0, QApplication::translate("confMode", "DB-Feld", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("confMode", "    Verbindungs Name:", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("confMode", "Sortierung", 0, QApplication::UnicodeUTF8));
        labelUsrName->setText(QApplication::translate("confMode", "-", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("confMode", "Verbindungen -AWD -Custom", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("confMode", "Rigbeschreibung", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem6 = rigList->headerItem();
        ___qtreewidgetitem6->setText(4, QApplication::translate("confMode", "Spezial", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem6->setText(3, QApplication::translate("confMode", "Betriebs-System", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem6->setText(2, QApplication::translate("confMode", "Antenne", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem6->setText(1, QApplication::translate("confMode", "Transceiver", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem6->setText(0, QApplication::translate("confMode", "Name", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Seite), QApplication::translate("confMode", "  Rig         ", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("confMode", "Rig Name", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("confMode", "Transceiver", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("confMode", "Antenne", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("confMode", "Op-System", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("confMode", "Spezial", 0, QApplication::UnicodeUTF8));
        RigClearPushButton->setText(QApplication::translate("confMode", "L\303\266schen", 0, QApplication::UnicodeUTF8));
        RigSavePushButton->setText(QApplication::translate("confMode", "Speichern", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("confMode", "Rig  -  Neu anlegen oder \303\244ndern", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Seite_2), QApplication::translate("confMode", "  Rig - Editor    ", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class confMode: public Ui_confMode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFMODE_H
