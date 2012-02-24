/********************************************************************************
** Form generated from reading UI file 'mnote.ui'
**
** Created: Fri Feb 24 14:20:25 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MNOTE_H
#define UI_MNOTE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_mNote
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QTableWidget *NoteTable;
    QPushButton *ButtonDelete;
    QSpacerItem *spacerItem;
    QPushButton *ButtonEnde;

    void setupUi(QDialog *mNote)
    {
        if (mNote->objectName().isEmpty())
            mNote->setObjectName(QString::fromUtf8("mNote"));
        mNote->resize(956, 383);
        gridLayout = new QGridLayout(mNote);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(mNote);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        label_2 = new QLabel(mNote);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font1;
        font1.setPointSize(8);
        label_2->setFont(font1);

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        NoteTable = new QTableWidget(mNote);
        if (NoteTable->columnCount() < 2)
            NoteTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        NoteTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        NoteTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        NoteTable->setObjectName(QString::fromUtf8("NoteTable"));

        gridLayout->addWidget(NoteTable, 1, 0, 1, 3);

        ButtonDelete = new QPushButton(mNote);
        ButtonDelete->setObjectName(QString::fromUtf8("ButtonDelete"));
        ButtonDelete->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(ButtonDelete, 2, 0, 1, 1);

        spacerItem = new QSpacerItem(19, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem, 2, 1, 1, 1);

        ButtonEnde = new QPushButton(mNote);
        ButtonEnde->setObjectName(QString::fromUtf8("ButtonEnde"));
        QPalette palette;
        QBrush brush(QColor(175, 215, 217, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        ButtonEnde->setPalette(palette);
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        ButtonEnde->setFont(font2);
        ButtonEnde->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(ButtonEnde, 2, 2, 1, 1);


        retranslateUi(mNote);

        QMetaObject::connectSlotsByName(mNote);
    } // setupUi

    void retranslateUi(QDialog *mNote)
    {
        mNote->setWindowTitle(QApplication::translate("mNote", "Notizblatt", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("mNote", "  Notizblatt", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("mNote", "( Notiz - Zeilenl\303\244nge max. 255 Buchstaben )", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = NoteTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("mNote", "Index             ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = NoteTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("mNote", "Notiz                                                                                                           ", 0, QApplication::UnicodeUTF8));
        ButtonDelete->setText(QApplication::translate("mNote", "L\303\266schen", 0, QApplication::UnicodeUTF8));
        ButtonEnde->setText(QApplication::translate("mNote", "                        F10   -  Ende                   ", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mNote: public Ui_mNote {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MNOTE_H
