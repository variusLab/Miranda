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


#ifndef PWDGENERATOR_H
#define PWDGENERATOR_H

#include <QWidget>
#include <QLayout>

class QPushButton;
class QCheckBox;
class QSpinBox;
class QLineEdit;
class QTextEdit;
class QLabel;


struct node{
    QString set;
    int size;
    bool isIncluded;
};


class pwdGenerator : public QWidget
{
    Q_OBJECT

public:
    pwdGenerator(QWidget *parent = nullptr);
    ~pwdGenerator();

   QCheckBox* createChBox(const int & i);

    int randInt(int max) // tirange aléatoire d'un nombre entier entre 0 et max-1 bornes incluses.
    {
        return static_cast<int>( (max-1)*static_cast<float>(rand())/RAND_MAX + 0.5 );
    }

    /* Quatre algorithmes de génération de mot de passe : */
    QString genAllowCrop(const int & wListSize);

    QString genWordFirst(QStringList wordList, int wListSize,
                   const QVector<int> & index, const int & size);

    QString genBegin2End1(const QVector<int> & index, const int & size,
                          const QVector<int> & index_l, const int & size_l);

    QString genAllRandom(const QVector<int> & index, const int size);


public slots:
    void customSpeCharSet(); /* réponse au clic sur le bouton "custom SpeChar set":
                              * ouvre une fenêtre de dialogue via QInputDialog::getText
                              * met à jour l'alphabet de caractères spéciaux */

    void customWordSet(); /* réponse au clic sur le bouton "custom the set of words":
                           * ouvre une fenêtre de dialogue sur mesure (instance de classe wordInputDialog héritée de QDialog)
                           * met à jour l'alphabet de mots ainsi que l'attribut m_wordFirstChBxState */

    void generate(); /* réponse au clic sur le bouton "Générer le mot de passe" */
    void reset(); /* restaure les paramètres par défaut */


private:
    int m_len;              /* longueur du mot de passe */
    node m_alphabet [5];    /* tableau de 5 'nodes' pour 5 alphabets différents */
    QStringList m_wordList; /* cette liste est considéré comme un alphabet
                             * dont les éléments sont des "mots" donnés par l'utilisateur */

    QLineEdit * m_lnEdtPwd; /* contient le mot de pass généré */
    Qt::CheckState m_wordFirstChBxState; /* enregistre l'état du checkBox
                                          * "Start password with a word (recommended)" dans wordInputDialog */

    QGridLayout * createGridForSettings(); /* crée un layout à 9 lignes x 2 colonnes pour la partie Settings */


    // Eléments de la fenêtre principale

    // Partie à gauche (Settings)

    QSpinBox * spBxPassLen;         // contrôle de la longueur du mot de passe
    QCheckBox * chBxTab [5];        // Tableau de 5 checkboxes pour choisir les alphabets à inclure
    QPushButton * btnCustomSpeChar; // bouton "Custom speChar set"
    QPushButton * btnCustomWords;   // bouton "Custom the set of words"
    QPushButton *btnReset;          // pour restaurer les réglages par défaut


    /* Partie à droite (Affichage + boutons de gestion) */

    QLineEdit * lnEdtPwd; // affiche le mot de passe généré
    QPushButton *btnCopy; // pour copier le mot de passe dans le presse-papiers
    QTextEdit *txtEdtHist; // champ "History" : affiche les mots de passes générés

    QPushButton *btnClearHist; // efface le champ "History"
    QPushButton *btnGen; // bouton "Générer le mot de passe"
    QPushButton *btnExit;

};
#endif // PWDGENERATOR_H
