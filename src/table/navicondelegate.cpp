/*****************************************************************************
* Copyright 2015-2016 Alexander Barthel albar965@mailbox.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "navicondelegate.h"
#include "column.h"
#include "columnlist.h"
#include "sqlmodel.h"
#include "sqlproxymodel.h"
#include "logging/loggingdefs.h"
#include "mapgui/mapquery.h"
#include "mapgui/symbolpainter.h"
#include "common/mapcolors.h"

#include <QPainter>
#include <QSqlQueryModel>
#include <QApplication>

NavIconDelegate::NavIconDelegate(const ColumnList *columns)
  : cols(columns)
{
  symbolPainter = new SymbolPainter();
}

NavIconDelegate::~NavIconDelegate()
{
  delete symbolPainter;
}

void NavIconDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option,
                            const QModelIndex& index) const
{
  QModelIndex idx(index);
  const SqlModel *sqlModel = dynamic_cast<const SqlModel *>(index.model());
  if(sqlModel == nullptr)
  {
    const SqlProxyModel *sqlProxyModel = dynamic_cast<const SqlProxyModel *>(index.model());
    Q_ASSERT(sqlProxyModel != nullptr);
    sqlModel = dynamic_cast<const SqlModel *>(sqlProxyModel->sourceModel());
    idx = sqlProxyModel->mapToSource(index);
  }
  Q_ASSERT(sqlModel != nullptr);

  // Create a style copy
  QStyleOptionViewItem opt(option);
  opt.displayAlignment = Qt::AlignRight;
  opt.font.setBold(true);
  // Draw the text
  QStyledItemDelegate::paint(painter, opt, index);

  int symSize = option.rect.height() - 4;
  int x = option.rect.x() + symSize;
  int y = option.rect.y() + symSize / 2 + 2;

  painter->setRenderHint(QPainter::Antialiasing);
  QString navtype = value(sqlModel, idx.row(), "nav_type").toString();
  if(navtype == "WAYPOINT")
    symbolPainter->drawWaypointSymbol(painter, maptypes::MapWaypoint(), x, y, symSize, false, false);
  else if(navtype == "NDB")
    symbolPainter->drawNdbSymbol(painter, maptypes::MapNdb(), x, y, symSize, false, false);
  else if(navtype == "VOR" || navtype == "VORDME" || navtype == "DME")
  {
    maptypes::MapVor vor;
    vor.dmeOnly = navtype == "DME";
    vor.hasDme = navtype == "VORDME" || navtype == "DME";
    symbolPainter->drawVorSymbol(painter, vor, x, y, symSize, false, false, 0);
  }
}

QVariant NavIconDelegate::value(const SqlModel *sqlModel, int row, const QString& name) const
{
  const Column *col = cols->getColumn(name);
  Q_ASSERT(col != nullptr);

  QVariant data = sqlModel->getRawData(row, col->getIndex());

  if(data.isValid())
    return data;
  else
    qCritical() << "sibling for column" << name << "not found";

  return QVariant();
}