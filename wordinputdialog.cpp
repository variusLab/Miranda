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

#include "wordinputdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>


wordInputDialog::wordInputDialog(QWidget *parent) : QDialog(parent)
{

    setModal(true);

    QString message = "<p>Please, give a comma separated list of \"words\" (groups of characters),<br />"
                      "with white spaces after the commas, e. g. <em>John, Doe9, stalker, 71:</em></p>";

    QLabel *label = new QLabel;
    label->setTextFormat(Qt::RichText);
    label->setText(message);

    m_lnEdt = new QLineEdit;

    m_chBox = new QCheckBox("Start password with a word (recommended)");
    m_chBox->setToolTip("This option forces the password to start with a word if its length does not exceed\n"
                        "the password's one. The word is randomly chosen from the list provided above.\n"
                        "This semi-random method generally yields better results than the complete randomness.");

    m_btnOk = new QPushButton("Ok");
    m_btnCancel = new QPushButton("Cancel");


    btnBox = new QDialogButtonBox(Qt::Horizontal);
    btnBox->addButton(m_btnOk, QDialogButtonBox::AcceptRole);
    btnBox->addButton(m_btnCancel, QDialogButtonBox::RejectRole);

    connect(btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    //layout
    QVBoxLayout * layout = new QVBoxLayout;
    layout->setContentsMargins(19,19,19,19);
    layout->setSpacing(13);
    layout->addWidget(label, 8);
    layout->addWidget(m_lnEdt, 4);
    layout->addWidget(m_chBox, 4);
    layout->addWidget(btnBox, 4);    

    setLayout(layout);
    setWindowTitle("Custom word list");    

}

QString wordInputDialog::getText()
{
    return m_lnEdt->text();
}

Qt::CheckState wordInputDialog::getCheckBoxState()
{
    return m_chBox->checkState();
}

void wordInputDialog::setText(const QString & content)
{
    m_lnEdt->setText(content);
}

void wordInputDialog::setCheckBoxState(Qt::CheckState state)
{
    m_chBox->setCheckState(state);
}
