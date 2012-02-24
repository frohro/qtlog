/********************************************************************************
** Form generated from reading UI file 'configdialog.ui'
**
** Created: Fri Feb 24 14:20:25 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGDIALOG_H
#define UI_CONFIGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_confDialog
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    QLabel *label;
    QPushButton *cButtonHilfe;
    QPushButton *ButtonExit;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QGridLayout *gridLayout1;
    QLabel *label_39;
    QLabel *label_37;
    QLabel *label_40;
    QTreeWidget *dblogsList;
    QSpacerItem *spacerItem1;
    QLabel *label_38;
    QLabel *LogLabel;
    QLabel *label_41;
    QFrame *line;
    QSpacerItem *spacerItem2;
    QLabel *label_13;
    QLineEdit *workLog;
    QSpacerItem *spacerItem3;
    QSpacerItem *spacerItem4;
    QPushButton *NewLogButton;
    QPushButton *DelLogButton;
    QSpacerItem *spacerItem5;
    QWidget *tab_2;
    QGridLayout *gridLayout2;
    QLabel *label_20;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QLineEdit *wId;
    QSpacerItem *spacerItem6;
    QComboBox *comboBoxHome;
    QLabel *label_3;
    QLineEdit *wCall;
    QLabel *label_4;
    QLineEdit *wName;
    QLabel *label_5;
    QLineEdit *wQth;
    QSpacerItem *spacerItem7;
    QLabel *label_6;
    QLineEdit *wDxcc;
    QSpacerItem *spacerItem8;
    QLabel *label_7;
    QLineEdit *wLocator;
    QSpacerItem *spacerItem9;
    QLabel *label_8;
    QLineEdit *wUtdif;
    QSpacerItem *spacerItem10;
    QLabel *label_35;
    QLineEdit *wLaenge;
    QLabel *label_36;
    QLineEdit *wBreite;
    QLabel *label_12;
    QTableWidget *locationTable;
    QLabel *label_9;
    QLineEdit *wMail;
    QLabel *label_10;
    QLineEdit *wWww;
    QWidget *tab_3;
    QGridLayout *gridLayout3;
    QSpacerItem *spacerItem11;
    QLabel *label_27;
    QSpacerItem *spacerItem12;
    QSpacerItem *spacerItem13;
    QSpacerItem *spacerItem14;
    QLabel *label_26;
    QLabel *label_25;
    QLabel *label_24;
    QLineEdit *wQslMgr;
    QLineEdit *wAwd;
    QLineEdit *wKenner;
    QSpacerItem *spacerItem15;
    QLabel *label_21;
    QSpacerItem *spacerItem16;
    QPushButton *ClubButton;
    QSpacerItem *spacerItem17;
    QSpacerItem *spacerItem18;
    QLineEdit *wKennerType;
    QLineEdit *wClub;
    QLabel *label_22;
    QLabel *label_23;
    QSpacerItem *spacerItem19;
    QSpacerItem *spacerItem20;
    QSpacerItem *spacerItem21;
    QWidget *tab_4;
    QLabel *label_11;
    QLabel *label_15;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_14;
    QLabel *label_16;
    QLabel *label_19;
    QLabel *label_28;
    QLabel *label_29;
    QLabel *label_30;
    QLineEdit *lotwUser;
    QLineEdit *lotwPasswd;
    QLineEdit *eqslUser;
    QLineEdit *eqslPasswd;
    QPushButton *ButtonInternet;

    void setupUi(QDialog *confDialog)
    {
        if (confDialog->objectName().isEmpty())
            confDialog->setObjectName(QString::fromUtf8("confDialog"));
        confDialog->resize(468, 610);
        gridLayout = new QGridLayout(confDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 0, 1, 1, 1);

        label = new QLabel(confDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QPalette palette;
        QBrush brush(QColor(33, 85, 226, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(119, 119, 119, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label->setPalette(palette);
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        cButtonHilfe = new QPushButton(confDialog);
        cButtonHilfe->setObjectName(QString::fromUtf8("cButtonHilfe"));
        cButtonHilfe->setFocusPolicy(Qt::NoFocus);
        cButtonHilfe->setFlat(true);

        gridLayout->addWidget(cButtonHilfe, 0, 2, 1, 1);

        ButtonExit = new QPushButton(confDialog);
        ButtonExit->setObjectName(QString::fromUtf8("ButtonExit"));
        QPalette palette1;
        QBrush brush2(QColor(175, 215, 217, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush2);
        ButtonExit->setPalette(palette1);
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        ButtonExit->setFont(font1);
        ButtonExit->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(ButtonExit, 2, 0, 1, 3);

        tabWidget = new QTabWidget(confDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setFont(font1);
        tabWidget->setFocusPolicy(Qt::NoFocus);
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        gridLayout1 = new QGridLayout(tab_1);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        label_39 = new QLabel(tab_1);
        label_39->setObjectName(QString::fromUtf8("label_39"));

        gridLayout1->addWidget(label_39, 0, 0, 1, 2);

        label_37 = new QLabel(tab_1);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        gridLayout1->addWidget(label_37, 1, 0, 1, 1);

        label_40 = new QLabel(tab_1);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        QFont font2;
        font2.setPointSize(8);
        font2.setBold(false);
        font2.setWeight(50);
        label_40->setFont(font2);

        gridLayout1->addWidget(label_40, 1, 1, 1, 6);

        dblogsList = new QTreeWidget(tab_1);
        dblogsList->setObjectName(QString::fromUtf8("dblogsList"));
        QPalette palette2;
        QBrush brush3(QColor(255, 255, 239, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush3);
        QBrush brush4(QColor(255, 255, 255, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        dblogsList->setPalette(palette2);
        QFont font3;
        font3.setBold(false);
        font3.setWeight(50);
        dblogsList->setFont(font3);
        dblogsList->setFocusPolicy(Qt::NoFocus);
        dblogsList->setRootIsDecorated(false);

        gridLayout1->addWidget(dblogsList, 2, 0, 1, 6);

        spacerItem1 = new QSpacerItem(17, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem1, 2, 6, 3, 1);

        label_38 = new QLabel(tab_1);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout1->addWidget(label_38, 3, 0, 1, 3);

        LogLabel = new QLabel(tab_1);
        LogLabel->setObjectName(QString::fromUtf8("LogLabel"));
        QPalette palette3;
        QBrush brush5(QColor(34, 85, 226, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        LogLabel->setPalette(palette3);
        QFont font4;
        font4.setPointSize(11);
        LogLabel->setFont(font4);

        gridLayout1->addWidget(LogLabel, 3, 3, 1, 2);

        label_41 = new QLabel(tab_1);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        label_41->setFont(font2);

        gridLayout1->addWidget(label_41, 4, 0, 1, 6);

        line = new QFrame(tab_1);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout1->addWidget(line, 5, 0, 1, 7);

        spacerItem2 = new QSpacerItem(119, 12, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout1->addItem(spacerItem2, 6, 3, 1, 2);

        label_13 = new QLabel(tab_1);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        QPalette palette4;
        QBrush brush6(QColor(35, 101, 208, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_13->setPalette(palette4);
        label_13->setFont(font1);
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout1->addWidget(label_13, 7, 0, 1, 2);

        workLog = new QLineEdit(tab_1);
        workLog->setObjectName(QString::fromUtf8("workLog"));
        workLog->setFont(font3);

        gridLayout1->addWidget(workLog, 7, 2, 1, 3);

        spacerItem3 = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem3, 7, 5, 1, 1);

        spacerItem4 = new QSpacerItem(81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem4, 8, 0, 1, 2);

        NewLogButton = new QPushButton(tab_1);
        NewLogButton->setObjectName(QString::fromUtf8("NewLogButton"));
        QPalette palette5;
        QBrush brush7(QColor(176, 211, 194, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        NewLogButton->setPalette(palette5);
        NewLogButton->setFont(font3);
        NewLogButton->setFocusPolicy(Qt::NoFocus);

        gridLayout1->addWidget(NewLogButton, 8, 2, 1, 2);

        DelLogButton = new QPushButton(tab_1);
        DelLogButton->setObjectName(QString::fromUtf8("DelLogButton"));
        QPalette palette6;
        QBrush brush8(QColor(220, 220, 220, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        DelLogButton->setPalette(palette6);
        DelLogButton->setFont(font3);
        DelLogButton->setFocusPolicy(Qt::NoFocus);

        gridLayout1->addWidget(DelLogButton, 8, 4, 1, 1);

        spacerItem5 = new QSpacerItem(33, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem5, 8, 5, 1, 1);

        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout2 = new QGridLayout(tab_2);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        label_20 = new QLabel(tab_2);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout2->addWidget(label_20, 0, 0, 1, 2);

        frame = new QFrame(tab_2);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        wId = new QLineEdit(frame);
        wId->setObjectName(QString::fromUtf8("wId"));
        QPalette palette7;
        QBrush brush9(QColor(250, 249, 227, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        wId->setPalette(palette7);
        wId->setFont(font3);
        wId->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(wId, 0, 1, 1, 1);

        spacerItem6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(spacerItem6, 0, 2, 1, 1);

        comboBoxHome = new QComboBox(frame);
        comboBoxHome->setObjectName(QString::fromUtf8("comboBoxHome"));
        comboBoxHome->setFont(font3);
        comboBoxHome->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(comboBoxHome, 1, 1, 1, 1);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        wCall = new QLineEdit(frame);
        wCall->setObjectName(QString::fromUtf8("wCall"));
        wCall->setFont(font3);

        gridLayout_2->addWidget(wCall, 2, 1, 1, 1);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font3);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        wName = new QLineEdit(frame);
        wName->setObjectName(QString::fromUtf8("wName"));
        wName->setFont(font3);

        gridLayout_2->addWidget(wName, 3, 1, 1, 1);

        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font3);
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_5, 4, 0, 1, 1);

        wQth = new QLineEdit(frame);
        wQth->setObjectName(QString::fromUtf8("wQth"));
        wQth->setFont(font3);

        gridLayout_2->addWidget(wQth, 4, 1, 1, 1);

        spacerItem7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(spacerItem7, 4, 2, 1, 1);

        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font3);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_6, 5, 0, 1, 1);

        wDxcc = new QLineEdit(frame);
        wDxcc->setObjectName(QString::fromUtf8("wDxcc"));
        wDxcc->setFont(font3);

        gridLayout_2->addWidget(wDxcc, 5, 1, 1, 1);

        spacerItem8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(spacerItem8, 5, 2, 1, 1);

        label_7 = new QLabel(frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font3);
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_7, 6, 0, 1, 1);

        wLocator = new QLineEdit(frame);
        wLocator->setObjectName(QString::fromUtf8("wLocator"));
        wLocator->setFont(font3);

        gridLayout_2->addWidget(wLocator, 6, 1, 1, 1);

        spacerItem9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(spacerItem9, 6, 2, 1, 1);

        label_8 = new QLabel(frame);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font3);
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 7, 0, 1, 1);

        wUtdif = new QLineEdit(frame);
        wUtdif->setObjectName(QString::fromUtf8("wUtdif"));
        wUtdif->setFont(font3);

        gridLayout_2->addWidget(wUtdif, 7, 1, 1, 1);

        spacerItem10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(spacerItem10, 7, 2, 1, 1);

        label_35 = new QLabel(frame);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setFont(font3);
        label_35->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_35, 8, 0, 1, 1);

        wLaenge = new QLineEdit(frame);
        wLaenge->setObjectName(QString::fromUtf8("wLaenge"));
        wLaenge->setFont(font3);

        gridLayout_2->addWidget(wLaenge, 8, 1, 1, 1);

        label_36 = new QLabel(frame);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setFont(font3);
        label_36->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_36, 9, 0, 1, 1);

        wBreite = new QLineEdit(frame);
        wBreite->setObjectName(QString::fromUtf8("wBreite"));
        wBreite->setFont(font3);

        gridLayout_2->addWidget(wBreite, 9, 1, 1, 1);


        gridLayout2->addWidget(frame, 1, 0, 2, 2);

        label_12 = new QLabel(tab_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout2->addWidget(label_12, 1, 2, 1, 1);

        locationTable = new QTableWidget(tab_2);
        if (locationTable->columnCount() < 2)
            locationTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        locationTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        locationTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        locationTable->setObjectName(QString::fromUtf8("locationTable"));
        locationTable->setFont(font3);
        locationTable->setFocusPolicy(Qt::NoFocus);

        gridLayout2->addWidget(locationTable, 2, 2, 1, 1);

        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font3);
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout2->addWidget(label_9, 3, 0, 1, 1);

        wMail = new QLineEdit(tab_2);
        wMail->setObjectName(QString::fromUtf8("wMail"));
        wMail->setFont(font3);

        gridLayout2->addWidget(wMail, 3, 1, 1, 2);

        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font3);
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout2->addWidget(label_10, 4, 0, 1, 1);

        wWww = new QLineEdit(tab_2);
        wWww->setObjectName(QString::fromUtf8("wWww"));
        wWww->setFont(font3);

        gridLayout2->addWidget(wWww, 4, 1, 1, 2);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout3 = new QGridLayout(tab_3);
#ifndef Q_OS_MAC
        gridLayout3->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout3->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        spacerItem11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem11, 7, 2, 1, 1);

        label_27 = new QLabel(tab_3);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setFont(font3);

        gridLayout3->addWidget(label_27, 6, 2, 1, 1);

        spacerItem12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem12, 5, 2, 1, 1);

        spacerItem13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem13, 4, 2, 1, 1);

        spacerItem14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem14, 3, 2, 1, 1);

        label_26 = new QLabel(tab_3);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setFont(font3);
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout3->addWidget(label_26, 7, 0, 1, 1);

        label_25 = new QLabel(tab_3);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setFont(font3);
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout3->addWidget(label_25, 6, 0, 1, 1);

        label_24 = new QLabel(tab_3);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout3->addWidget(label_24, 5, 0, 1, 1);

        wQslMgr = new QLineEdit(tab_3);
        wQslMgr->setObjectName(QString::fromUtf8("wQslMgr"));
        wQslMgr->setFont(font3);

        gridLayout3->addWidget(wQslMgr, 7, 1, 1, 1);

        wAwd = new QLineEdit(tab_3);
        wAwd->setObjectName(QString::fromUtf8("wAwd"));
        wAwd->setFont(font3);

        gridLayout3->addWidget(wAwd, 6, 1, 1, 1);

        wKenner = new QLineEdit(tab_3);
        wKenner->setObjectName(QString::fromUtf8("wKenner"));
        wKenner->setFont(font3);

        gridLayout3->addWidget(wKenner, 5, 1, 1, 1);

        spacerItem15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem15, 2, 1, 1, 1);

        label_21 = new QLabel(tab_3);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Sans Serif"));
        font5.setPointSize(10);
        font5.setBold(true);
        font5.setWeight(75);
        label_21->setFont(font5);

        gridLayout3->addWidget(label_21, 1, 1, 1, 1);

        spacerItem16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem16, 9, 0, 1, 1);

        ClubButton = new QPushButton(tab_3);
        ClubButton->setObjectName(QString::fromUtf8("ClubButton"));
        ClubButton->setFocusPolicy(Qt::NoFocus);

        gridLayout3->addWidget(ClubButton, 9, 1, 1, 1);

        spacerItem17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem17, 9, 2, 1, 1);

        spacerItem18 = new QSpacerItem(20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout3->addItem(spacerItem18, 8, 1, 1, 1);

        wKennerType = new QLineEdit(tab_3);
        wKennerType->setObjectName(QString::fromUtf8("wKennerType"));
        wKennerType->setFont(font3);

        gridLayout3->addWidget(wKennerType, 4, 1, 1, 1);

        wClub = new QLineEdit(tab_3);
        wClub->setObjectName(QString::fromUtf8("wClub"));
        wClub->setFont(font3);

        gridLayout3->addWidget(wClub, 3, 1, 1, 1);

        label_22 = new QLabel(tab_3);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout3->addWidget(label_22, 3, 0, 1, 1);

        label_23 = new QLabel(tab_3);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setFont(font3);
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout3->addWidget(label_23, 4, 0, 1, 1);

        spacerItem19 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout3->addItem(spacerItem19, 0, 1, 1, 1);

        spacerItem20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem20, 1, 2, 1, 1);

        spacerItem21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem21, 1, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        label_11 = new QLabel(tab_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(112, 40, 217, 23));
        label_11->setFont(font4);
        label_15 = new QLabel(tab_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(64, 84, 277, 23));
        QFont font6;
        font6.setPointSize(9);
        label_15->setFont(font6);
        label_17 = new QLabel(tab_4);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(96, 120, 271, 23));
        QFont font7;
        font7.setPointSize(9);
        font7.setBold(false);
        font7.setWeight(50);
        label_17->setFont(font7);
        label_18 = new QLabel(tab_4);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(96, 144, 246, 23));
        label_18->setFont(font7);
        label_14 = new QLabel(tab_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(70, 195, 61, 23));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_16 = new QLabel(tab_4);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(147, 195, 46, 23));
        label_16->setFont(font3);
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_19 = new QLabel(tab_4);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(116, 230, 77, 25));
        label_19->setFont(font3);
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_28 = new QLabel(tab_4);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(30, 300, 97, 23));
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_29 = new QLabel(tab_4);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(142, 300, 51, 23));
        label_29->setFont(font3);
        label_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_30 = new QLabel(tab_4);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(116, 330, 77, 23));
        label_30->setFont(font3);
        label_30->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lotwUser = new QLineEdit(tab_4);
        lotwUser->setObjectName(QString::fromUtf8("lotwUser"));
        lotwUser->setGeometry(QRect(200, 190, 113, 33));
        QFont font8;
        font8.setPointSize(8);
        lotwUser->setFont(font8);
        lotwPasswd = new QLineEdit(tab_4);
        lotwPasswd->setObjectName(QString::fromUtf8("lotwPasswd"));
        lotwPasswd->setGeometry(QRect(200, 225, 141, 33));
        lotwPasswd->setFont(font8);
        lotwPasswd->setEchoMode(QLineEdit::Password);
        eqslUser = new QLineEdit(tab_4);
        eqslUser->setObjectName(QString::fromUtf8("eqslUser"));
        eqslUser->setGeometry(QRect(200, 290, 113, 33));
        eqslUser->setFont(font8);
        eqslPasswd = new QLineEdit(tab_4);
        eqslPasswd->setObjectName(QString::fromUtf8("eqslPasswd"));
        eqslPasswd->setGeometry(QRect(200, 325, 141, 33));
        eqslPasswd->setFont(font8);
        eqslPasswd->setEchoMode(QLineEdit::Password);
        ButtonInternet = new QPushButton(tab_4);
        ButtonInternet->setObjectName(QString::fromUtf8("ButtonInternet"));
        ButtonInternet->setGeometry(QRect(100, 415, 246, 32));
        tabWidget->addTab(tab_4, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 3);

        QWidget::setTabOrder(wCall, wName);
        QWidget::setTabOrder(wName, wQth);
        QWidget::setTabOrder(wQth, wDxcc);
        QWidget::setTabOrder(wDxcc, wLocator);
        QWidget::setTabOrder(wLocator, wUtdif);
        QWidget::setTabOrder(wUtdif, wLaenge);
        QWidget::setTabOrder(wLaenge, wBreite);
        QWidget::setTabOrder(wBreite, wMail);
        QWidget::setTabOrder(wMail, wWww);
        QWidget::setTabOrder(wWww, wClub);
        QWidget::setTabOrder(wClub, wKennerType);
        QWidget::setTabOrder(wKennerType, wKenner);
        QWidget::setTabOrder(wKenner, wAwd);
        QWidget::setTabOrder(wAwd, wQslMgr);
        QWidget::setTabOrder(wQslMgr, workLog);

        retranslateUi(confDialog);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(confDialog);
    } // setupUi

    void retranslateUi(QDialog *confDialog)
    {
        confDialog->setWindowTitle(QApplication::translate("confDialog", "ConfigDialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("confDialog", "  QtLog Config", 0, QApplication::UnicodeUTF8));
        cButtonHilfe->setText(QApplication::translate("confDialog", "Hilfe", 0, QApplication::UnicodeUTF8));
        ButtonExit->setText(QApplication::translate("confDialog", "F10   -   Beenden", 0, QApplication::UnicodeUTF8));
        label_39->setText(QString());
        label_37->setText(QApplication::translate("confDialog", "  LogbookOrdner ", 0, QApplication::UnicodeUTF8));
        label_40->setText(QString());
        QTreeWidgetItem *___qtreewidgetitem = dblogsList->headerItem();
        ___qtreewidgetitem->setText(3, QApplication::translate("confDialog", "Gesichert", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(2, QApplication::translate("confDialog", "db.Rel", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("confDialog", "Qsos", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("confDialog", "LogName", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("confDialog", " Gesetzes ArbeitsLog :", 0, QApplication::UnicodeUTF8));
        LogLabel->setText(QApplication::translate("confDialog", "fun", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("confDialog", "( Klicke in den Logordner um das Arbeitslog zu \303\244ndern )", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("confDialog", "Logbook Name", 0, QApplication::UnicodeUTF8));
        NewLogButton->setText(QApplication::translate("confDialog", "NEU", 0, QApplication::UnicodeUTF8));
        DelLogButton->setText(QApplication::translate("confDialog", "L\303\266schen", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QApplication::translate("confDialog", "Logbook    ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("confDialog", "Id", 0, QApplication::UnicodeUTF8));
        wId->setText(QString());
        label_3->setText(QApplication::translate("confDialog", "Call", 0, QApplication::UnicodeUTF8));
        wCall->setText(QString());
        label_4->setText(QApplication::translate("confDialog", "Name", 0, QApplication::UnicodeUTF8));
        wName->setText(QString());
        label_5->setText(QApplication::translate("confDialog", "Standort", 0, QApplication::UnicodeUTF8));
        wQth->setText(QString());
        label_6->setText(QApplication::translate("confDialog", "DXCC", 0, QApplication::UnicodeUTF8));
        wDxcc->setText(QString());
        label_7->setText(QApplication::translate("confDialog", "Locator", 0, QApplication::UnicodeUTF8));
        wLocator->setText(QString());
        label_8->setText(QApplication::translate("confDialog", "Ut-dif.", 0, QApplication::UnicodeUTF8));
        wUtdif->setText(QString());
        label_35->setText(QApplication::translate("confDialog", "G.L\303\244nge", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("confDialog", "G.Breite", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("confDialog", "     Standorte", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = locationTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("confDialog", "Logging", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = locationTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("confDialog", "Standort", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("confDialog", "eMail:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("confDialog", "WWW:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("confDialog", " Standort     ", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("confDialog", "( DOK - Sammlung )", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("confDialog", "Qsl.Manager", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("confDialog", "Club-Award", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("confDialog", "Kenner", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("confDialog", "Default Club", 0, QApplication::UnicodeUTF8));
        ClubButton->setText(QApplication::translate("confDialog", "Speichern", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("confDialog", "Club", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("confDialog", "KennerName", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("confDialog", "Club     ", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("confDialog", "Internet Zugang", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("confDialog", "M\303\266gliche Server Verbindungen:", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("confDialog", "https://p1k.arrl.org/lotwuser/...", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("confDialog", "http://www.eqsl.cc/qslcard/...", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("confDialog", "LotW", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("confDialog", "User", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("confDialog", "Passwd", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("confDialog", "eQSL.cc", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("confDialog", "User", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("confDialog", "Passwd", 0, QApplication::UnicodeUTF8));
        ButtonInternet->setText(QApplication::translate("confDialog", "Speichern", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("confDialog", "Internet     ", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class confDialog: public Ui_confDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGDIALOG_H
