/*****************************************************************************
* Copyright 2015-2017 Alexander Barthel albar965@mailbox.org
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

#ifndef LITTLENAVMAP_USERWAYPOINTDIALOG_H
#define LITTLENAVMAP_USERWAYPOINTDIALOG_H

#include <QDialog>

class QRegularExpressionValidator;

namespace Ui {
class UserWaypointDialog;
}

class UserWaypointDialog :
  public QDialog
{
  Q_OBJECT

public:
  UserWaypointDialog(QWidget *parent, const QString& name);
  virtual ~UserWaypointDialog();

  QString getName() const;

private:
  QRegularExpressionValidator *validator = nullptr;
  Ui::UserWaypointDialog *ui;
};

#endif // LITTLENAVMAP_USERWAYPOINTDIALOG_H
