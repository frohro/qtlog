/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QtGui>
#include "qtlogdelegate.h"

QtlogDelegate::QtlogDelegate(QObject *parent)
    : QItemDelegate(parent) {
}

// create Editor
// --------------------------------------------------------------------
QWidget *QtlogDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem&,
    const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    //create a completer with the strings in the column as model.
    QStringList allStrings;
    for(int i = 1; i<index.model()->rowCount(); i++){
        QString strItem(index.model()->data(index.sibling(i, index.column()), Qt::EditRole).toString());
        if(!allStrings.contains(strItem))
            allStrings.append(strItem);
    }
    QCompleter *autoComplete = new QCompleter(allStrings);
    editor->setCompleter(autoComplete);
    connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return editor;
}

// -------------------------------------------
void QtlogDelegate::commitAndCloseEditor()
{
    QLineEdit *editor = qobject_cast<QLineEdit *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

// copy modeldata into the Editor ------------------------------------------------
void QtlogDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
    if (edit) {
        edit->setText(index.model()->data(index, Qt::EditRole).toString());
    }
}

// when the user has finished editing call setModelData
// ------------------------------------------------------------------------------
void QtlogDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
    const QModelIndex &index) const
{
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
    if(edit) {
        model->setData(index, edit->text());
    }
}
