/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */
#ifndef QTLOGDELEGATE_H
#define QTLOGDELEGATE_H
#include <QtGui>
class QtlogDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    QtlogDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
    const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
    const QModelIndex &index) const;
private slots:
    void commitAndCloseEditor();
};
#endif //QTLOGDELEGATE_H
