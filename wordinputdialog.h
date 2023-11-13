/*
 * This file is part of Miranda.
 * Copyright (C) 2023 Varvara Petrova
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
 */

#ifndef WORDINPUTDIALOG_H
#define WORDINPUTDIALOG_H

#include <QDialog>
class QLineEdit;
class QCheckBox;
class QDialogButtonBox;

class wordInputDialog : public QDialog
{
    Q_OBJECT

public:
    wordInputDialog(QWidget *parent = nullptr);
    QString getText();
    Qt::CheckState getCheckBoxState();
    void setText(const QString & content);
    void setCheckBoxState(Qt::CheckState state);

private:
    QLineEdit * m_lnEdt;
    QCheckBox *m_chBox;
    QPushButton *m_btnOk;
    QPushButton *m_btnCancel;
    QDialogButtonBox *btnBox;

};

#endif // WORDINPUTDIALOG_H

