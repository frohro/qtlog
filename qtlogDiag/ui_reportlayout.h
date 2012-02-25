/********************************************************************************
** Form generated from reading UI file 'reportlayout.ui'
**
** Created: Sat Feb 25 15:09:43 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPORTLAYOUT_H
#define UI_REPORTLAYOUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_reportLayout
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QTreeWidget *dbList;
    QTreeWidget *layoutList;
    QLabel *label_4;
    QComboBox *BoxLayout;
    QPushButton *pushButtonOK;

    void setupUi(QDialog *reportLayout)
    {
        if (reportLayout->objectName().isEmpty())
            reportLayout->setObjectName(QString::fromUtf8("reportLayout"));
        reportLayout->resize(308, 468);
        reportLayout->setFocusPolicy(Qt::NoFocus);
        gridLayout = new QGridLayout(reportLayout);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(reportLayout);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 4);

        label_2 = new QLabel(reportLayout);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 1, 0, 1, 2);

        label_3 = new QLabel(reportLayout);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        gridLayout->addWidget(label_3, 1, 2, 1, 2);

        dbList = new QTreeWidget(reportLayout);
        dbList->setObjectName(QString::fromUtf8("dbList"));
        dbList->setFocusPolicy(Qt::NoFocus);
        dbList->setRootIsDecorated(false);

        gridLayout->addWidget(dbList, 2, 0, 1, 2);

        layoutList = new QTreeWidget(reportLayout);
        layoutList->setObjectName(QString::fromUtf8("layoutList"));
        layoutList->setEnabled(true);
        QPalette palette;
        QBrush brush(QColor(255, 253, 234, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        layoutList->setPalette(palette);
        layoutList->setMouseTracking(true);
        layoutList->setFocusPolicy(Qt::NoFocus);
        layoutList->setRootIsDecorated(false);

        gridLayout->addWidget(layoutList, 2, 2, 1, 2);

        label_4 = new QLabel(reportLayout);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        BoxLayout = new QComboBox(reportLayout);
        BoxLayout->setObjectName(QString::fromUtf8("BoxLayout"));
        BoxLayout->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(BoxLayout, 3, 1, 1, 1);

        pushButtonOK = new QPushButton(reportLayout);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));
        pushButtonOK->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonOK, 3, 2, 1, 2);


        retranslateUi(reportLayout);

        QMetaObject::connectSlotsByName(reportLayout);
    } // setupUi

    void retranslateUi(QDialog *reportLayout)
    {
        reportLayout->setWindowTitle(QApplication::translate("reportLayout", "Report Layout", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("reportLayout", "W\303\244hle die Spalten die im QsoReport\n"
"angezeigt werden sollen.\n"
"Die Spalten mit '*' sind immer gew\303\244hlt", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("reportLayout", "  Log-Datensatz", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("reportLayout", "  Anzeige ", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = dbList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("reportLayout", "Spalte", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("reportLayout", "sel", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = layoutList->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("reportLayout", "QsoReport", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("reportLayout", "Layout", 0, QApplication::UnicodeUTF8));
        BoxLayout->clear();
        BoxLayout->insertItems(0, QStringList()
         << QApplication::translate("reportLayout", "EU", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("reportLayout", "DX", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("reportLayout", "QSL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("reportLayout", "Contest", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("reportLayout", "Var", 0, QApplication::UnicodeUTF8)
        );
        pushButtonOK->setText(QApplication::translate("reportLayout", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class reportLayout: public Ui_reportLayout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTLAYOUT_H
