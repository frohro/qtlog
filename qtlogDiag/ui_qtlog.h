/********************************************************************************
** Form generated from reading UI file 'qtlog.ui'
**
** Created: Fri Feb 24 14:36:10 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTLOG_H
#define UI_QTLOG_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtLog
{
public:
    QAction *actionAddQso;
    QAction *actionLogChange;
    QAction *actionDataSave;
    QAction *actionReportLayout;
    QAction *actionDBadmin;
    QAction *actionDxccTable;
    QAction *actionStandort;
    QAction *actionClub;
    QAction *actionDb;
    QAction *actionPrefTable;
    QAction *actionMode;
    QAction *actionVerbindungen;
    QAction *actionRig;
    QAction *actionADIFimport;
    QAction *actionLogconf;
    QAction *actionQslEingang;
    QAction *actionQslDruck;
    QAction *actionQtlogVersion;
    QAction *actionQtLogInfo;
    QAction *actionADIFexport;
    QAction *actionNotiz;
    QAction *actionRuDistrictAwdList;
    QAction *actionQrzCallbook;
    QAction *actionJahresMODE;
    QAction *actionDxStatusCW;
    QAction *actionDxStatusSSB;
    QAction *actionJahresBAND;
    QAction *actionDLDstatus;
    QAction *actionIOTAstatus;
    QAction *actionDokVerwaltung;
    QAction *actionIotaVerwaltung;
    QAction *actionENDE;
    QAction *actionQueryTextOut;
    QAction *actionAddClog;
    QAction *actionInternet_ON;
    QAction *actionRigCtl;
    QAction *actionRigCtlHamlib;
    QAction *actionFldigi_logging;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLabel *Bildlabel;
    QLabel *label_9;
    QWidget *page_2;
    QGridLayout *gridLayout1;
    QLabel *logbooklabel;
    QLabel *LogbookName;
    QSpacerItem *spacerItem;
    QLabel *label_8;
    QLabel *labelQsoBestand;
    QLabel *labelOperator;
    QTableWidget *reportTable;
    QDockWidget *dock;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_2;
    QTableWidget *awdTable;
    QLabel *label_10;
    QLabel *labelDatabase;
    QFrame *frame;
    QGridLayout *gridLayout_4;
    QLabel *label_6;
    QCheckBox *checkBoxDateEpoch;
    QGroupBox *groupBox;
    QGridLayout *gridLayout2;
    QLabel *label_2;
    QComboBox *BoxLimit;
    QCheckBox *checkBoxSort;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout3;
    QComboBox *BoxGroup;
    QLineEdit *groupEditLine;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout4;
    QComboBox *BoxAwd;
    QLineEdit *awdEditLine;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_3;
    QLabel *label_11;
    QPushButton *ButtonRemoveQso;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout5;
    QLabel *label_3;
    QDateEdit *dateEditBis;
    QLabel *label_7;
    QDateEdit *dateEditVon;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout6;
    QLabel *label_5;
    QComboBox *BoxSort;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout7;
    QComboBox *BoxLayout;
    QLabel *label_4;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout8;
    QLabel *label;
    QLabel *labelVersion;
    QLabel *labelRelease;
    QWidget *Seite;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuQrgNotiz;
    QMenu *menuInfo;
    QMenu *menuConfig;
    QMenu *menuQso_Verwaltung;
    QMenu *menuTabellen;
    QMenu *menuBetriebsarten;
    QMenu *menuAward_Info;
    QMenu *menuLogbook;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *QtLog)
    {
        if (QtLog->objectName().isEmpty())
            QtLog->setObjectName(QString::fromUtf8("QtLog"));
        QtLog->resize(1269, 730);
        QFont font;
        font.setPointSize(10);
        QtLog->setFont(font);
        QtLog->setDockNestingEnabled(true);
        QtLog->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        actionAddQso = new QAction(QtLog);
        actionAddQso->setObjectName(QString::fromUtf8("actionAddQso"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../icon/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/new/prefix1/icon/add.png"), QSize(), QIcon::Selected, QIcon::On);
        actionAddQso->setIcon(icon);
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        actionAddQso->setFont(font1);
        actionLogChange = new QAction(QtLog);
        actionLogChange->setObjectName(QString::fromUtf8("actionLogChange"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/icon/umbrello_diagram_component.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLogChange->setIcon(icon1);
        actionDataSave = new QAction(QtLog);
        actionDataSave->setObjectName(QString::fromUtf8("actionDataSave"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/icon/vcs_update.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDataSave->setIcon(icon2);
        actionDataSave->setFont(font1);
        actionReportLayout = new QAction(QtLog);
        actionReportLayout->setObjectName(QString::fromUtf8("actionReportLayout"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/icon/filter.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReportLayout->setIcon(icon3);
        actionDBadmin = new QAction(QtLog);
        actionDBadmin->setObjectName(QString::fromUtf8("actionDBadmin"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/icon/package_utilities.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDBadmin->setIcon(icon4);
        actionDxccTable = new QAction(QtLog);
        actionDxccTable->setObjectName(QString::fromUtf8("actionDxccTable"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/new/prefix1/icon/www.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDxccTable->setIcon(icon5);
        actionStandort = new QAction(QtLog);
        actionStandort->setObjectName(QString::fromUtf8("actionStandort"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/new/prefix1/icon/folder_home.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStandort->setIcon(icon6);
        actionClub = new QAction(QtLog);
        actionClub->setObjectName(QString::fromUtf8("actionClub"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/new/prefix1/icon/presence_online.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClub->setIcon(icon7);
        actionDb = new QAction(QtLog);
        actionDb->setObjectName(QString::fromUtf8("actionDb"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/new/prefix1/icon/vcs_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDb->setIcon(icon8);
        actionPrefTable = new QAction(QtLog);
        actionPrefTable->setObjectName(QString::fromUtf8("actionPrefTable"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/new/prefix1/icon/wp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrefTable->setIcon(icon9);
        actionMode = new QAction(QtLog);
        actionMode->setObjectName(QString::fromUtf8("actionMode"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/new/prefix1/icon/configure_shortcuts.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMode->setIcon(icon10);
        actionVerbindungen = new QAction(QtLog);
        actionVerbindungen->setObjectName(QString::fromUtf8("actionVerbindungen"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/new/prefix1/icon/connect_creating.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionVerbindungen->setIcon(icon11);
        actionRig = new QAction(QtLog);
        actionRig->setObjectName(QString::fromUtf8("actionRig"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/new/prefix1/icon/irkick.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRig->setIcon(icon12);
        actionADIFimport = new QAction(QtLog);
        actionADIFimport->setObjectName(QString::fromUtf8("actionADIFimport"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/new/prefix1/icon/fileimport.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionADIFimport->setIcon(icon13);
        actionLogconf = new QAction(QtLog);
        actionLogconf->setObjectName(QString::fromUtf8("actionLogconf"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/new/prefix1/icon/kontact_todo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLogconf->setIcon(icon14);
        actionQslEingang = new QAction(QtLog);
        actionQslEingang->setObjectName(QString::fromUtf8("actionQslEingang"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/new/prefix1/icon/view_text.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQslEingang->setIcon(icon15);
        actionQslDruck = new QAction(QtLog);
        actionQslDruck->setObjectName(QString::fromUtf8("actionQslDruck"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/new/prefix1/icon/fileprint.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQslDruck->setIcon(icon16);
        actionQtlogVersion = new QAction(QtLog);
        actionQtlogVersion->setObjectName(QString::fromUtf8("actionQtlogVersion"));
        actionQtlogVersion->setIcon(icon14);
        actionQtLogInfo = new QAction(QtLog);
        actionQtLogInfo->setObjectName(QString::fromUtf8("actionQtLogInfo"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/new/prefix1/icon/contexthelp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQtLogInfo->setIcon(icon17);
        actionADIFexport = new QAction(QtLog);
        actionADIFexport->setObjectName(QString::fromUtf8("actionADIFexport"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/new/prefix1/icon/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionADIFexport->setIcon(icon18);
        actionNotiz = new QAction(QtLog);
        actionNotiz->setObjectName(QString::fromUtf8("actionNotiz"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/new/prefix1/icon/color_line.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNotiz->setIcon(icon19);
        actionRuDistrictAwdList = new QAction(QtLog);
        actionRuDistrictAwdList->setObjectName(QString::fromUtf8("actionRuDistrictAwdList"));
        actionRuDistrictAwdList->setIcon(icon15);
        actionQrzCallbook = new QAction(QtLog);
        actionQrzCallbook->setObjectName(QString::fromUtf8("actionQrzCallbook"));
        actionQrzCallbook->setIcon(icon15);
        actionJahresMODE = new QAction(QtLog);
        actionJahresMODE->setObjectName(QString::fromUtf8("actionJahresMODE"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/new/prefix1/icon/view_sidetree.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionJahresMODE->setIcon(icon20);
        actionDxStatusCW = new QAction(QtLog);
        actionDxStatusCW->setObjectName(QString::fromUtf8("actionDxStatusCW"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/new/prefix1/icon/viewmag+.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDxStatusCW->setIcon(icon21);
        actionDxStatusSSB = new QAction(QtLog);
        actionDxStatusSSB->setObjectName(QString::fromUtf8("actionDxStatusSSB"));
        actionDxStatusSSB->setIcon(icon21);
        actionJahresBAND = new QAction(QtLog);
        actionJahresBAND->setObjectName(QString::fromUtf8("actionJahresBAND"));
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/new/prefix1/icon/today.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionJahresBAND->setIcon(icon22);
        actionDLDstatus = new QAction(QtLog);
        actionDLDstatus->setObjectName(QString::fromUtf8("actionDLDstatus"));
        actionDLDstatus->setIcon(icon7);
        actionIOTAstatus = new QAction(QtLog);
        actionIOTAstatus->setObjectName(QString::fromUtf8("actionIOTAstatus"));
        actionIOTAstatus->setIcon(icon9);
        actionDokVerwaltung = new QAction(QtLog);
        actionDokVerwaltung->setObjectName(QString::fromUtf8("actionDokVerwaltung"));
        actionDokVerwaltung->setIcon(icon15);
        actionIotaVerwaltung = new QAction(QtLog);
        actionIotaVerwaltung->setObjectName(QString::fromUtf8("actionIotaVerwaltung"));
        actionIotaVerwaltung->setIcon(icon15);
        actionENDE = new QAction(QtLog);
        actionENDE->setObjectName(QString::fromUtf8("actionENDE"));
        actionQueryTextOut = new QAction(QtLog);
        actionQueryTextOut->setObjectName(QString::fromUtf8("actionQueryTextOut"));
        actionQueryTextOut->setIcon(icon1);
        actionAddClog = new QAction(QtLog);
        actionAddClog->setObjectName(QString::fromUtf8("actionAddClog"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/new/prefix1/icon/fifteenpieces.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAddClog->setIcon(icon23);
        actionInternet_ON = new QAction(QtLog);
        actionInternet_ON->setObjectName(QString::fromUtf8("actionInternet_ON"));
        actionInternet_ON->setIcon(icon5);
        actionRigCtl = new QAction(QtLog);
        actionRigCtl->setObjectName(QString::fromUtf8("actionRigCtl"));
        actionRigCtl->setIcon(icon7);
        actionRigCtlHamlib = new QAction(QtLog);
        actionRigCtlHamlib->setObjectName(QString::fromUtf8("actionRigCtlHamlib"));
        QIcon icon24;
        icon24.addFile(QString::fromUtf8(":/new/prefix1/icon/artscontrol.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRigCtlHamlib->setIcon(icon24);
        actionFldigi_logging = new QAction(QtLog);
        actionFldigi_logging->setObjectName(QString::fromUtf8("actionFldigi_logging"));
        actionFldigi_logging->setIcon(icon22);
        QFont font2;
        actionFldigi_logging->setFont(font2);
        centralwidget = new QWidget(QtLog);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        Bildlabel = new QLabel(page);
        Bildlabel->setObjectName(QString::fromUtf8("Bildlabel"));
        Bildlabel->setGeometry(QRect(9, 9, 600, 20));
        label_9 = new QLabel(page);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(15, -132, 1277, 1024));
        label_9->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/icon/Won-Ton-Soup-3.jpg")));
        label_9->setScaledContents(false);
        label_9->setWordWrap(false);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        gridLayout1 = new QGridLayout(page_2);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        logbooklabel = new QLabel(page_2);
        logbooklabel->setObjectName(QString::fromUtf8("logbooklabel"));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setWeight(75);
        logbooklabel->setFont(font3);

        gridLayout1->addWidget(logbooklabel, 0, 0, 1, 1);

        LogbookName = new QLabel(page_2);
        LogbookName->setObjectName(QString::fromUtf8("LogbookName"));
        QPalette palette;
        QBrush brush(QColor(77, 79, 245, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(119, 119, 119, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        LogbookName->setPalette(palette);
        QFont font4;
        font4.setPointSize(11);
        LogbookName->setFont(font4);

        gridLayout1->addWidget(LogbookName, 0, 1, 1, 1);

        spacerItem = new QSpacerItem(403, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem, 0, 4, 1, 1);

        label_8 = new QLabel(page_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font1);

        gridLayout1->addWidget(label_8, 0, 5, 1, 1);

        labelQsoBestand = new QLabel(page_2);
        labelQsoBestand->setObjectName(QString::fromUtf8("labelQsoBestand"));

        gridLayout1->addWidget(labelQsoBestand, 0, 6, 1, 1);

        labelOperator = new QLabel(page_2);
        labelOperator->setObjectName(QString::fromUtf8("labelOperator"));

        gridLayout1->addWidget(labelOperator, 0, 7, 1, 1);

        reportTable = new QTableWidget(page_2);
        reportTable->setObjectName(QString::fromUtf8("reportTable"));
        QPalette palette1;
        QBrush brush2(QColor(253, 255, 237, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        QBrush brush3(QColor(244, 244, 244, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        reportTable->setPalette(palette1);
        reportTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        reportTable->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
        reportTable->setSelectionMode(QAbstractItemView::ExtendedSelection);

        gridLayout1->addWidget(reportTable, 2, 0, 1, 8);

        dock = new QDockWidget(page_2);
        dock->setObjectName(QString::fromUtf8("dock"));
        QIcon icon25;
        icon25.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        dock->setWindowIcon(icon25);
        dock->setFloating(false);
        dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dock->setAllowedAreas(Qt::NoDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout_2 = new QGridLayout(dockWidgetContents);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        awdTable = new QTableWidget(dockWidgetContents);
        if (awdTable->columnCount() < 2)
            awdTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        awdTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        awdTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        awdTable->setObjectName(QString::fromUtf8("awdTable"));
        awdTable->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
        awdTable->setSelectionMode(QAbstractItemView::SingleSelection);

        gridLayout_2->addWidget(awdTable, 0, 0, 1, 1);

        dock->setWidget(dockWidgetContents);

        gridLayout1->addWidget(dock, 2, 8, 1, 1);

        label_10 = new QLabel(page_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QPalette palette2;
        QBrush brush4(QColor(154, 137, 51, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        QBrush brush5(QColor(146, 145, 144, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        label_10->setPalette(palette2);
        QFont font5;
        font5.setPointSize(8);
        label_10->setFont(font5);

        gridLayout1->addWidget(label_10, 0, 2, 1, 1);

        labelDatabase = new QLabel(page_2);
        labelDatabase->setObjectName(QString::fromUtf8("labelDatabase"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        labelDatabase->setPalette(palette3);
        labelDatabase->setFont(font5);

        gridLayout1->addWidget(labelDatabase, 0, 3, 1, 1);

        frame = new QFrame(page_2);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frame);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QPalette palette4;
        QBrush brush6(QColor(96, 98, 171, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_6->setPalette(palette4);
        label_6->setFont(font3);

        gridLayout_4->addWidget(label_6, 0, 0, 1, 2);

        checkBoxDateEpoch = new QCheckBox(frame);
        checkBoxDateEpoch->setObjectName(QString::fromUtf8("checkBoxDateEpoch"));
        checkBoxDateEpoch->setFocusPolicy(Qt::NoFocus);
        checkBoxDateEpoch->setCheckable(true);

        gridLayout_4->addWidget(checkBoxDateEpoch, 0, 5, 1, 1);

        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font1);
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        gridLayout2 = new QGridLayout(groupBox);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font6;
        font6.setPointSize(8);
        font6.setBold(true);
        font6.setWeight(75);
        label_2->setFont(font6);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout2->addWidget(label_2, 0, 0, 1, 1);

        BoxLimit = new QComboBox(groupBox);
        BoxLimit->setObjectName(QString::fromUtf8("BoxLimit"));
        BoxLimit->setFont(font1);
        BoxLimit->setFocusPolicy(Qt::NoFocus);

        gridLayout2->addWidget(BoxLimit, 2, 0, 1, 1);

        checkBoxSort = new QCheckBox(groupBox);
        checkBoxSort->setObjectName(QString::fromUtf8("checkBoxSort"));
        checkBoxSort->setEnabled(false);
        QFont font7;
        font7.setPointSize(8);
        font7.setBold(false);
        font7.setWeight(50);
        checkBoxSort->setFont(font7);
        checkBoxSort->setFocusPolicy(Qt::NoFocus);
        checkBoxSort->setChecked(false);

        gridLayout2->addWidget(checkBoxSort, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupBox, 1, 0, 1, 1);

        groupBox_6 = new QGroupBox(frame);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setFont(font1);
        gridLayout3 = new QGridLayout(groupBox_6);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        BoxGroup = new QComboBox(groupBox_6);
        BoxGroup->setObjectName(QString::fromUtf8("BoxGroup"));
        QFont font8;
        font8.setPointSize(9);
        font8.setBold(false);
        font8.setWeight(50);
        BoxGroup->setFont(font8);
        BoxGroup->setFocusPolicy(Qt::NoFocus);

        gridLayout3->addWidget(BoxGroup, 0, 0, 1, 1);

        groupEditLine = new QLineEdit(groupBox_6);
        groupEditLine->setObjectName(QString::fromUtf8("groupEditLine"));
        QPalette palette5;
        QBrush brush7(QColor(255, 255, 231, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush7);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        QBrush brush8(QColor(255, 255, 255, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        groupEditLine->setPalette(palette5);
        QFont font9;
        font9.setBold(false);
        font9.setWeight(50);
        groupEditLine->setFont(font9);

        gridLayout3->addWidget(groupEditLine, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_6, 1, 1, 1, 1);

        groupBox_5 = new QGroupBox(frame);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setFont(font1);
        gridLayout4 = new QGridLayout(groupBox_5);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        BoxAwd = new QComboBox(groupBox_5);
        BoxAwd->setObjectName(QString::fromUtf8("BoxAwd"));
        BoxAwd->setFont(font9);
        BoxAwd->setFocusPolicy(Qt::NoFocus);

        gridLayout4->addWidget(BoxAwd, 0, 0, 1, 1);

        awdEditLine = new QLineEdit(groupBox_5);
        awdEditLine->setObjectName(QString::fromUtf8("awdEditLine"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::Base, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        awdEditLine->setPalette(palette6);
        awdEditLine->setFont(font9);

        gridLayout4->addWidget(awdEditLine, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_5, 1, 2, 1, 1);

        groupBox_8 = new QGroupBox(frame);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setFont(font1);
        gridLayout_3 = new QGridLayout(groupBox_8);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_11 = new QLabel(groupBox_8);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font9);

        gridLayout_3->addWidget(label_11, 0, 0, 1, 1);

        ButtonRemoveQso = new QPushButton(groupBox_8);
        ButtonRemoveQso->setObjectName(QString::fromUtf8("ButtonRemoveQso"));
        ButtonRemoveQso->setFont(font9);
        ButtonRemoveQso->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(ButtonRemoveQso, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_8, 1, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(106, 45, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer, 1, 4, 1, 1);

        groupBox_3 = new QGroupBox(frame);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout5 = new QGridLayout(groupBox_3);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout5->addWidget(label_3, 0, 0, 1, 1);

        dateEditBis = new QDateEdit(groupBox_3);
        dateEditBis->setObjectName(QString::fromUtf8("dateEditBis"));
        dateEditBis->setEnabled(false);
        QPalette palette7;
        QBrush brush9(QColor(238, 238, 238, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        dateEditBis->setPalette(palette7);
        dateEditBis->setFocusPolicy(Qt::NoFocus);
        dateEditBis->setDate(QDate(2000, 1, 1));
        dateEditBis->setCalendarPopup(true);

        gridLayout5->addWidget(dateEditBis, 0, 1, 1, 1);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout5->addWidget(label_7, 1, 0, 1, 1);

        dateEditVon = new QDateEdit(groupBox_3);
        dateEditVon->setObjectName(QString::fromUtf8("dateEditVon"));
        dateEditVon->setEnabled(false);
        QPalette palette8;
        QBrush brush10(QColor(228, 255, 207, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush10);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush10);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        dateEditVon->setPalette(palette8);
        dateEditVon->setFocusPolicy(Qt::NoFocus);
        dateEditVon->setCalendarPopup(true);

        gridLayout5->addWidget(dateEditVon, 1, 1, 1, 1);


        gridLayout_4->addWidget(groupBox_3, 1, 5, 1, 1);

        groupBox_7 = new QGroupBox(frame);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setFont(font1);
        gridLayout6 = new QGridLayout(groupBox_7);
        gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
        label_5 = new QLabel(groupBox_7);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font9);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout6->addWidget(label_5, 0, 0, 1, 1);

        BoxSort = new QComboBox(groupBox_7);
        BoxSort->setObjectName(QString::fromUtf8("BoxSort"));
        BoxSort->setFont(font9);
        BoxSort->setFocusPolicy(Qt::NoFocus);

        gridLayout6->addWidget(BoxSort, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_7, 1, 6, 1, 1);

        groupBox_4 = new QGroupBox(frame);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setFont(font1);
        gridLayout7 = new QGridLayout(groupBox_4);
        gridLayout7->setObjectName(QString::fromUtf8("gridLayout7"));
        BoxLayout = new QComboBox(groupBox_4);
        BoxLayout->setObjectName(QString::fromUtf8("BoxLayout"));
        BoxLayout->setFont(font9);

        gridLayout7->addWidget(BoxLayout, 1, 0, 1, 1);

        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font9);
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout7->addWidget(label_4, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_4, 1, 7, 1, 1);

        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout8 = new QGridLayout(groupBox_2);
        gridLayout8->setObjectName(QString::fromUtf8("gridLayout8"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        QPalette palette9;
        QBrush brush11(QColor(159, 143, 53, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush11);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush11);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label->setPalette(palette9);
        QFont font10;
        font10.setPointSize(18);
        label->setFont(font10);

        gridLayout8->addWidget(label, 0, 0, 1, 1);

        labelVersion = new QLabel(groupBox_2);
        labelVersion->setObjectName(QString::fromUtf8("labelVersion"));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        labelVersion->setPalette(palette10);
        labelVersion->setFont(font5);
        labelVersion->setAlignment(Qt::AlignCenter);

        gridLayout8->addWidget(labelVersion, 1, 0, 1, 1);

        labelRelease = new QLabel(groupBox_2);
        labelRelease->setObjectName(QString::fromUtf8("labelRelease"));
        QPalette palette11;
        QBrush brush12(QColor(154, 137, 50, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush12);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush12);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        labelRelease->setPalette(palette11);
        QFont font11;
        font11.setPointSize(7);
        labelRelease->setFont(font11);
        labelRelease->setAlignment(Qt::AlignCenter);

        gridLayout8->addWidget(labelRelease, 2, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_2, 1, 8, 1, 1);


        gridLayout1->addWidget(frame, 1, 0, 1, 9);

        stackedWidget->addWidget(page_2);
        Seite = new QWidget();
        Seite->setObjectName(QString::fromUtf8("Seite"));
        stackedWidget->addWidget(Seite);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);

        QtLog->setCentralWidget(centralwidget);
        menubar = new QMenuBar(QtLog);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1269, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuQrgNotiz = new QMenu(menubar);
        menuQrgNotiz->setObjectName(QString::fromUtf8("menuQrgNotiz"));
        menuInfo = new QMenu(menubar);
        menuInfo->setObjectName(QString::fromUtf8("menuInfo"));
        menuConfig = new QMenu(menubar);
        menuConfig->setObjectName(QString::fromUtf8("menuConfig"));
        menuQso_Verwaltung = new QMenu(menubar);
        menuQso_Verwaltung->setObjectName(QString::fromUtf8("menuQso_Verwaltung"));
        menuTabellen = new QMenu(menubar);
        menuTabellen->setObjectName(QString::fromUtf8("menuTabellen"));
        menuBetriebsarten = new QMenu(menubar);
        menuBetriebsarten->setObjectName(QString::fromUtf8("menuBetriebsarten"));
        menuAward_Info = new QMenu(menubar);
        menuAward_Info->setObjectName(QString::fromUtf8("menuAward_Info"));
        menuLogbook = new QMenu(menubar);
        menuLogbook->setObjectName(QString::fromUtf8("menuLogbook"));
        QtLog->setMenuBar(menubar);
        statusbar = new QStatusBar(QtLog);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        QtLog->setStatusBar(statusbar);
        QWidget::setTabOrder(groupEditLine, awdEditLine);
        QWidget::setTabOrder(awdEditLine, reportTable);
        QWidget::setTabOrder(reportTable, awdTable);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuAward_Info->menuAction());
        menubar->addAction(menuLogbook->menuAction());
        menubar->addAction(menuQso_Verwaltung->menuAction());
        menubar->addAction(menuConfig->menuAction());
        menubar->addAction(menuBetriebsarten->menuAction());
        menubar->addAction(menuTabellen->menuAction());
        menubar->addAction(menuQrgNotiz->menuAction());
        menubar->addAction(menuInfo->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionENDE);
        menuQrgNotiz->addAction(actionNotiz);
        menuInfo->addSeparator();
        menuInfo->addAction(actionQtLogInfo);
        menuInfo->addSeparator();
        menuInfo->addAction(actionRigCtlHamlib);
        menuInfo->addAction(actionQtlogVersion);
        menuConfig->addSeparator();
        menuConfig->addAction(actionLogconf);
        menuConfig->addAction(actionStandort);
        menuConfig->addAction(actionClub);
        menuConfig->addSeparator();
        menuConfig->addSeparator();
        menuConfig->addAction(actionInternet_ON);
        menuConfig->addSeparator();
        menuConfig->addAction(actionRigCtl);
        menuQso_Verwaltung->addSeparator();
        menuQso_Verwaltung->addSeparator();
        menuQso_Verwaltung->addAction(actionQslEingang);
        menuQso_Verwaltung->addAction(actionQslDruck);
        menuQso_Verwaltung->addSeparator();
        menuQso_Verwaltung->addAction(actionReportLayout);
        menuQso_Verwaltung->addSeparator();
        menuQso_Verwaltung->addAction(actionQueryTextOut);
        menuQso_Verwaltung->addSeparator();
        menuQso_Verwaltung->addSeparator();
        menuQso_Verwaltung->addAction(actionADIFimport);
        menuQso_Verwaltung->addAction(actionADIFexport);
        menuQso_Verwaltung->addSeparator();
        menuQso_Verwaltung->addAction(actionDataSave);
        menuTabellen->addSeparator();
        menuTabellen->addAction(actionDxccTable);
        menuTabellen->addAction(actionPrefTable);
        menuTabellen->addSeparator();
        menuTabellen->addSeparator();
        menuTabellen->addAction(actionDokVerwaltung);
        menuTabellen->addAction(actionIotaVerwaltung);
        menuTabellen->addSeparator();
        menuTabellen->addAction(actionQrzCallbook);
        menuTabellen->addSeparator();
        menuTabellen->addAction(actionRuDistrictAwdList);
        menuTabellen->addSeparator();
        menuTabellen->addAction(actionDBadmin);
        menuBetriebsarten->addAction(actionMode);
        menuBetriebsarten->addAction(actionVerbindungen);
        menuBetriebsarten->addAction(actionRig);
        menuAward_Info->addAction(actionDxStatusSSB);
        menuAward_Info->addAction(actionDxStatusCW);
        menuAward_Info->addSeparator();
        menuAward_Info->addAction(actionDLDstatus);
        menuAward_Info->addAction(actionIOTAstatus);
        menuAward_Info->addSeparator();
        menuAward_Info->addAction(actionJahresMODE);
        menuAward_Info->addAction(actionJahresBAND);
        menuAward_Info->addSeparator();
        menuAward_Info->addSeparator();
        menuLogbook->addAction(actionAddQso);
        menuLogbook->addSeparator();
        menuLogbook->addAction(actionAddClog);
        menuLogbook->addSeparator();
        menuLogbook->addAction(actionLogChange);
        menuLogbook->addSeparator();
        menuLogbook->addAction(actionFldigi_logging);

        retranslateUi(QtLog);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(QtLog);
    } // setupUi

    void retranslateUi(QMainWindow *QtLog)
    {
        QtLog->setWindowTitle(QApplication::translate("QtLog", "QtLog       Log-Verwaltung ", 0, QApplication::UnicodeUTF8));
        actionAddQso->setText(QApplication::translate("QtLog", "&QSO Logging", 0, QApplication::UnicodeUTF8));
        actionLogChange->setText(QApplication::translate("QtLog", "Logbook &tauschen", 0, QApplication::UnicodeUTF8));
        actionDataSave->setText(QApplication::translate("QtLog", "Datensicherung", 0, QApplication::UnicodeUTF8));
        actionReportLayout->setText(QApplication::translate("QtLog", "ReportLayout", 0, QApplication::UnicodeUTF8));
        actionDBadmin->setText(QApplication::translate("QtLog", "DB- Administration", 0, QApplication::UnicodeUTF8));
        actionDxccTable->setText(QApplication::translate("QtLog", "Dxcc - Laender", 0, QApplication::UnicodeUTF8));
        actionStandort->setText(QApplication::translate("QtLog", "Standort", 0, QApplication::UnicodeUTF8));
        actionClub->setText(QApplication::translate("QtLog", "Club", 0, QApplication::UnicodeUTF8));
        actionDb->setText(QApplication::translate("QtLog", "Datenbank", 0, QApplication::UnicodeUTF8));
        actionPrefTable->setText(QApplication::translate("QtLog", "Sub-Prefixliste", 0, QApplication::UnicodeUTF8));
        actionMode->setText(QApplication::translate("QtLog", "Band | Mod | Pwr", 0, QApplication::UnicodeUTF8));
        actionVerbindungen->setText(QApplication::translate("QtLog", "Verbindungen | AWD", 0, QApplication::UnicodeUTF8));
        actionRig->setText(QApplication::translate("QtLog", "Rig", 0, QApplication::UnicodeUTF8));
        actionADIFimport->setText(QApplication::translate("QtLog", "ADIF Import", 0, QApplication::UnicodeUTF8));
        actionLogconf->setText(QApplication::translate("QtLog", "Logbook", 0, QApplication::UnicodeUTF8));
        actionQslEingang->setText(QApplication::translate("QtLog", "QSL Bearbeitung", 0, QApplication::UnicodeUTF8));
        actionQslDruck->setText(QApplication::translate("QtLog", "QSL Drucken", 0, QApplication::UnicodeUTF8));
        actionQtlogVersion->setText(QApplication::translate("QtLog", "QtLog Version", 0, QApplication::UnicodeUTF8));
        actionQtLogInfo->setText(QApplication::translate("QtLog", "Qso Verwaltung", 0, QApplication::UnicodeUTF8));
        actionADIFexport->setText(QApplication::translate("QtLog", "ADIF Export", 0, QApplication::UnicodeUTF8));
        actionNotiz->setText(QApplication::translate("QtLog", "Notizblatt", 0, QApplication::UnicodeUTF8));
        actionRuDistrictAwdList->setText(QApplication::translate("QtLog", "RU-DistrictAwdList", 0, QApplication::UnicodeUTF8));
        actionQrzCallbook->setText(QApplication::translate("QtLog", "QRZ - callbook", 0, QApplication::UnicodeUTF8));
        actionJahresMODE->setText(QApplication::translate("QtLog", "Mode   - Aktivit\303\244t", 0, QApplication::UnicodeUTF8));
        actionDxStatusCW->setText(QApplication::translate("QtLog", "DX-Status  -CW", 0, QApplication::UnicodeUTF8));
        actionDxStatusSSB->setText(QApplication::translate("QtLog", "DX-Status  -SSB", 0, QApplication::UnicodeUTF8));
        actionJahresBAND->setText(QApplication::translate("QtLog", "Band   - Aktivit\303\244t", 0, QApplication::UnicodeUTF8));
        actionDLDstatus->setText(QApplication::translate("QtLog", "DLD   -Status", 0, QApplication::UnicodeUTF8));
        actionIOTAstatus->setText(QApplication::translate("QtLog", "IOTA   -Status", 0, QApplication::UnicodeUTF8));
        actionDokVerwaltung->setText(QApplication::translate("QtLog", "DOK  Verwaltung", 0, QApplication::UnicodeUTF8));
        actionIotaVerwaltung->setText(QApplication::translate("QtLog", "IOTA Verwaltung", 0, QApplication::UnicodeUTF8));
        actionENDE->setText(QApplication::translate("QtLog", "ENDE", 0, QApplication::UnicodeUTF8));
        actionQueryTextOut->setText(QApplication::translate("QtLog", "LogDaten Export", 0, QApplication::UnicodeUTF8));
        actionAddClog->setText(QApplication::translate("QtLog", "Contest Logging", 0, QApplication::UnicodeUTF8));
        actionInternet_ON->setText(QApplication::translate("QtLog", "Internet freigeben", 0, QApplication::UnicodeUTF8));
        actionRigCtl->setText(QApplication::translate("QtLog", "Rig Control/Hamlib", 0, QApplication::UnicodeUTF8));
        actionRigCtlHamlib->setText(QApplication::translate("QtLog", "RigControl-Hamlib", 0, QApplication::UnicodeUTF8));
        actionFldigi_logging->setText(QApplication::translate("QtLog", "Fldigi Logging", 0, QApplication::UnicodeUTF8));
        logbooklabel->setText(QApplication::translate("QtLog", "    Logbook :", 0, QApplication::UnicodeUTF8));
        LogbookName->setText(QApplication::translate("QtLog", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("QtLog", "Datenbestand: ", 0, QApplication::UnicodeUTF8));
        labelQsoBestand->setText(QApplication::translate("QtLog", "   Datensaetze", 0, QApplication::UnicodeUTF8));
        labelOperator->setText(QApplication::translate("QtLog", "  Operator", 0, QApplication::UnicodeUTF8));
        dock->setWindowTitle(QApplication::translate("QtLog", "Award Tabelle", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = awdTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("QtLog", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = awdTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("QtLog", "Kenner", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("QtLog", "          [ Database", 0, QApplication::UnicodeUTF8));
        labelDatabase->setText(QApplication::translate("QtLog", "logdb", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("QtLog", " Datenbankfilter", 0, QApplication::UnicodeUTF8));
        checkBoxDateEpoch->setText(QApplication::translate("QtLog", "Ohne Kalender", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("QtLog", " Zeige ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("QtLog", "Monate", 0, QApplication::UnicodeUTF8));
        BoxLimit->clear();
        BoxLimit->insertItems(0, QStringList()
         << QApplication::translate("QtLog", "    3  ", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "    6  ", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "   12  ", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "   24  ", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "   36  ", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "    0  ", 0, QApplication::UnicodeUTF8)
        );
        checkBoxSort->setText(QApplication::translate("QtLog", "Abw.Sort", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("QtLog", "  Suche", 0, QApplication::UnicodeUTF8));
        BoxGroup->clear();
        BoxGroup->insertItems(0, QStringList()
         << QApplication::translate("QtLog", "Call", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Band", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Mode", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Dxcc", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "WAE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Wpx", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Locator", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "ContId", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "QslMgr", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Rig", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Qth", 0, QApplication::UnicodeUTF8)
        );
        groupBox_5->setTitle(QApplication::translate("QtLog", "  Zeige AwdTypen", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("QtLog", "L\303\266sche", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("QtLog", "Select QSO Zeile", 0, QApplication::UnicodeUTF8));
        ButtonRemoveQso->setText(QApplication::translate("QtLog", "Remove QSO", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("QtLog", ".", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("QtLog", "Vom", 0, QApplication::UnicodeUTF8));
        dateEditBis->setDisplayFormat(QApplication::translate("QtLog", "yyyy.MM.dd", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("QtLog", "Bis", 0, QApplication::UnicodeUTF8));
        dateEditVon->setDisplayFormat(QApplication::translate("QtLog", "yyyy.MM.dd", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("QtLog", "Sortierung", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("QtLog", "   Nach", 0, QApplication::UnicodeUTF8));
        BoxSort->clear();
        BoxSort->insertItems(0, QStringList()
         << QApplication::translate("QtLog", "Datum", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Band", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Call", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "DXcc", 0, QApplication::UnicodeUTF8)
        );
        groupBox_4->setTitle(QApplication::translate("QtLog", "Report ", 0, QApplication::UnicodeUTF8));
        BoxLayout->clear();
        BoxLayout->insertItems(0, QStringList()
         << QApplication::translate("QtLog", "EU", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "DX", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "QSL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Contest", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QtLog", "Var", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("QtLog", " Layout", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        label->setText(QApplication::translate("QtLog", "QtLog", 0, QApplication::UnicodeUTF8));
        labelVersion->setText(QApplication::translate("QtLog", "versiom", 0, QApplication::UnicodeUTF8));
        labelRelease->setText(QApplication::translate("QtLog", "Rel", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("QtLog", "File                 ", 0, QApplication::UnicodeUTF8));
        menuQrgNotiz->setTitle(QApplication::translate("QtLog", "Notizen        ", 0, QApplication::UnicodeUTF8));
        menuInfo->setTitle(QApplication::translate("QtLog", "Hilfe        ", 0, QApplication::UnicodeUTF8));
        menuConfig->setTitle(QApplication::translate("QtLog", "Configuriere      ", 0, QApplication::UnicodeUTF8));
        menuQso_Verwaltung->setTitle(QApplication::translate("QtLog", "LogbookVerwaltung    ", 0, QApplication::UnicodeUTF8));
        menuTabellen->setTitle(QApplication::translate("QtLog", "Tabellen         ", 0, QApplication::UnicodeUTF8));
        menuBetriebsarten->setTitle(QApplication::translate("QtLog", "Betriebsarten", 0, QApplication::UnicodeUTF8));
        menuAward_Info->setTitle(QApplication::translate("QtLog", "Award Status      ", 0, QApplication::UnicodeUTF8));
        menuLogbook->setTitle(QApplication::translate("QtLog", "Logging                ", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QtLog: public Ui_QtLog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTLOG_H
