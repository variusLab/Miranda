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
 
// Version 1.0 parue le 01.09.2023

#include "pwdgenerator.h"

#include <QApplication>
#include <cstdlib>
#include <ctime>


int main(int argc, char *argv[])
{

    srand(time(NULL));
    QApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton); // supprime btn "?" ds QDialog
    QApplication::setStyle("Fusion");
    pwdGenerator miranda;

    miranda.show();
    return app.exec();
}
