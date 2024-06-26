/*

Copyright (c) 2018, John Smith

Permission to use, copy, modify, and/or distribute this software for
any purpose with or without fee is hereby granted, provided that the
above copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

*/


#ifndef FROZENFRAMESMODEL_H
#define FROZENFRAMESMODEL_H

#include <map>

#include <QAbstractTableModel>

#include "WobblyTypes.h"


class FrozenFramesModel : public QAbstractTableModel, public FreezeFrameMap {
    Q_OBJECT

    enum Columns {
        FirstColumn = 0,
        LastColumn,
        ReplacementColumn,
        ColumnCount
    };

public:
    FrozenFramesModel(QObject *parent = Q_NULLPTR);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    using FreezeFrameMap::size;
    using FreezeFrameMap::cbegin;
    using FreezeFrameMap::cend;
    using FreezeFrameMap::upper_bound;

    void insert(const value_type &freeze_frame);

    void erase(int freeze_frame);

    void clear();
};

#endif // FROZENFRAMESMODEL_H
