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

#include "pwdgenerator.h"
#include "wordinputdialog.h"
#include "alphabetconstants.h"

#include <QtWidgets>


pwdGenerator::pwdGenerator(QWidget *parent)
    : QWidget(parent)
{
    m_len = 8;

    /* --- initialisation du tableau m_alphabet --- */
    m_alphabet[0] = {alpha::UPPERCASE,                            26, true  };
    m_alphabet[1] = {alpha::LOWERCASE,                            26, true  };
    m_alphabet[2] = {alpha::NUMBERS,                              10, true  };
    m_alphabet[3] = {alpha::DEFAULT_SPECHAR,  alpha::D_SPECHAR_COUNT, true  };
    m_alphabet[4] = {QString(),                                    0, false };

    m_wordList = QStringList();

    m_wordFirstChBxState = Qt::Checked;

    /* --- initialisation d'un layout global --- */
    QHBoxLayout *globalLayout = new QHBoxLayout;
    globalLayout->setContentsMargins(14, 14, 14, 14);
    globalLayout->setSpacing(5);    

    /* --------------------------------------------------------------------
     *  Partie à gauche : Settings
     * --------------------------------------------------------------------*/

    /* --- Création d'un gridLayout à 9 lignes et 2 colonnes
     *
     * 0: champ de sélection de la longueur de mot de passe (s'étend sur 1 ligne et 2 colonnes)
     * 1: spacer
     * 2: chBox "Capital letters"
     * 3: chBox "Small letters"
     * 4: chBox "Numbers"
     * 5: chBox "Spacial characters"      bouton 1: btnCustomSpeChar
     * 6: chBox "Words"                   bouton 2: btnCustomWords
     * 7. spacer
     * 8: bouton "Restore default settings" (s'étend sur 1 ligne et 2 colonnes)
     */

    QGridLayout *gridLayout = createGridForSettings();

    /* --- Champ de choix de longueur du mot de passe --- */

    QLabel *lblPassLen = new QLabel("Password length:");
    spBxPassLen = new QSpinBox;
    spBxPassLen->setRange(3,16);
    spBxPassLen->setValue(m_len);
    connect(spBxPassLen, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int newVal) {m_len = newVal;}
            );
    QHBoxLayout *hBxLayout_len = new QHBoxLayout;
    hBxLayout_len->setContentsMargins(QMargins());
    hBxLayout_len->addWidget(lblPassLen);
    hBxLayout_len->addWidget(spBxPassLen);
    hBxLayout_len->addStretch(3);

    gridLayout->addLayout(hBxLayout_len, 0, 0, 1, 2);

    /* --- Création de boutons 1,2 permettant de personnaliser les caracters spéciaux et les mots
     * susceptibles d'être inclus dans le mot de passe : --- */

    btnCustomSpeChar = new QPushButton;
    btnCustomSpeChar->setEnabled(true); //pour être cohérent avec l'état initial du checkbox
    connect(btnCustomSpeChar, &QPushButton::clicked, this, &pwdGenerator::customSpeCharSet);    

    btnCustomWords = new QPushButton;
    btnCustomWords->setEnabled(false); //pour être cohérent avec l'état initial du checkbox    
    connect(btnCustomWords, &QPushButton::clicked, this, &pwdGenerator::customWordSet);   

    //ajout d'une icone à ces deux boutons
    QIcon customIcon(":/icons/icon/parametres.png");
    //btnCustomSpeChar->setFlat(true);
    //btnCustomWords->setFlat(true);
    btnCustomSpeChar->setIcon(customIcon);
    //btnCustomSpeChar->setIconSize(0.75*btnSize);
    btnCustomWords->setIcon(customIcon);

    QSize btnSize(25,25);
    btnCustomWords->setMinimumSize(btnSize);
    btnCustomSpeChar->setMinimumSize(btnSize);;

    gridLayout->addWidget(btnCustomSpeChar, 5, 1);
    gridLayout->addWidget(btnCustomWords, 6, 1);


    /* --- Création en boucle de 5 QCheckBoxes (connectées) et leur mise dans gridLayout --- */

    for (int i=0; i < alpha::N_EXISTANTS; ++i)
         gridLayout->addWidget(createChBox(i), chBox::gridRowPos[i], 0);


    /* --- Bouton "Restore default settings" qui remet les checkboxes et alphabets à leurs états initiaux par défaut --- */
    btnReset = new QPushButton("Reset");
    connect(btnReset, &QPushButton::clicked, this, &pwdGenerator::reset);

    gridLayout->addWidget(btnReset, 8, 0, 1, 2);

    globalLayout->addLayout(gridLayout);


    /* --- Création d'une ligne verticale séparant la fenêtre en 2 parties --- */

    QFrame *vLine = new QFrame;
    vLine->setFrameShape(QFrame::VLine);
    vLine->setFrameShadow(QFrame::Sunken);
    vLine->setLineWidth(1);
    vLine->setMidLineWidth(0);

    globalLayout->addWidget(vLine, 1);

    /* --------------------------------------------------------------------
     *  Partie à droite : affichage + boutons de gestion
     * --------------------------------------------------------------------*/

    QVBoxLayout *vBxLayout_result = new QVBoxLayout;
    vBxLayout_result->setContentsMargins(11,0,0,0);
    vBxLayout_result->setSpacing(7);

    /* --- Champ avec le mot de passe généré et un bouton "copier dans le presse-papiers" --- */
    QLabel *lblGeneratedPwd = new QLabel("Generated password:");
    btnCopy = new QPushButton;
    btnCopy->setMinimumHeight(30);
    btnCopy->setToolTip("Copy to clipboard");

    QIcon copyIcon(":/icons/icon/copie.png");
    btnCopy->setFlat(false);
    btnCopy->setIcon(copyIcon);

    m_lnEdtPwd = new QLineEdit;
    m_lnEdtPwd->setMinimumHeight(30);    
    //QFont font = m_lnEdtPwd->font();
    //font.setPointSize(13);
    QFont font("Courier", 13, QFont::Normal, false); //Lucida Console Courier Courrier New Consolas NSimSun    
    m_lnEdtPwd->setFont(font);

    connect(btnCopy, &QPushButton::clicked, m_lnEdtPwd,
            [this] ( ) { m_lnEdtPwd->selectAll();
                         m_lnEdtPwd->copy();
                         m_lnEdtPwd->deselect();}
            );

    QHBoxLayout *hBxLayout_pwd = new QHBoxLayout;
    hBxLayout_pwd->setContentsMargins(0,0,0,0);
    hBxLayout_pwd->setSpacing(0);
    hBxLayout_pwd->addWidget(m_lnEdtPwd,22);
    hBxLayout_pwd->addStretch(1);
    hBxLayout_pwd->addWidget(btnCopy,3);

    vBxLayout_result->addWidget(lblGeneratedPwd);
    vBxLayout_result->addLayout(hBxLayout_pwd);

    /* --- Champ 'Historique' --- */
    QLabel *lblHist = new QLabel("History");
    txtEdtHist = new QTextEdit;
    QFont fontTxtEdt("Courier New", 9, QFont::Medium, false);
    txtEdtHist->setFont(fontTxtEdt);
    connect(m_lnEdtPwd, &QLineEdit::textChanged, txtEdtHist, &QTextEdit::append);
    /* remarque: d'un appel à l'autre, l'algorithme genAllowCrop peut produire un résultat identique à l'appel précédent
     * (cela est d'autant plus probable que l'ensemble m_wordList est petit).
     * Le signal "textChanged" n'est alors pas envoyé et rien ne change au niveau du champ Historique:
     * on a l'impression d'un bug car le programme ne répond pas visuellement à un clic sur le bouton "generate".
     * On voudrait que le nouveau mot de passe (même si identique au précédent) apparaisse tjs sur une nouvelle ligne dans "Historique"
     * Solution: Introduire un compteur (static): nombre de fois cliquées sur le bouton "generate" (remettre à 0 après "clear history" ?)
     * on n'est pas obligé d'afficher ce nombre, juste l'utiliser en interne.
     * Notons que l'algorithme genAllowCrop() est supposé être appelé rarement
     */


    vBxLayout_result->addWidget(lblHist);
    vBxLayout_result->addWidget(txtEdtHist);

    /* --- Champ avec boutons Clear history, Generate, Exit --- */

    btnClearHist = new QPushButton("Clear history");
    connect(btnClearHist, &QPushButton::clicked, txtEdtHist, &QTextEdit::clear);

    btnGen = new QPushButton("Generate");
    connect(btnGen, &QPushButton::clicked, this, &pwdGenerator::generate);

    btnExit = new QPushButton("Exit");
    connect(btnExit, &QPushButton::clicked, qApp, &QApplication::exit);

    QHBoxLayout *hBxLayout_3btns = new QHBoxLayout;
    hBxLayout_3btns->setContentsMargins(QMargins());
    hBxLayout_3btns->setSpacing(7);
    hBxLayout_3btns->addWidget(btnClearHist);
    hBxLayout_3btns->addWidget(btnGen);
    hBxLayout_3btns->addWidget(btnExit);

    vBxLayout_result->addStretch(1);
    vBxLayout_result->addLayout(hBxLayout_3btns);

    int vStretch [6] = {5, 6, 5, 23, 5, 5};
    for (int i=0; i<6; ++i)
    vBxLayout_result->setStretch(i, vStretch[i]);


    globalLayout->addLayout(vBxLayout_result, 30);

    this->setLayout(globalLayout);

}

pwdGenerator::~pwdGenerator()
{
}

QGridLayout * pwdGenerator::createGridForSettings()
{
    QGridLayout *grid = new QGridLayout;
    grid->setContentsMargins(0,2,11,0);
    grid->setSpacing(7);
    int rowStretch[9] = {2,1,2,2,2,2,2,1,2};
    for (int i=0; i<9; ++i)
        grid->setRowStretch(i, rowStretch[i]);
    grid->setColumnStretch(0,19);
    grid->setColumnStretch(1,6);

    return grid;
}


QCheckBox* pwdGenerator::createChBox(const int & i)

{
    chBxTab[i] = new QCheckBox(chBox::LABEL[i]);
    chBxTab[i]->setToolTip(chBox::INFOBULLE[i]);
    chBxTab[i]->setCheckState(chBox::InitialState[i]);
    connect(chBxTab[i], &QCheckBox::stateChanged, this,
            [this, i] (int newState) {
        m_alphabet[i].isIncluded = (newState==Qt::Checked);        
        //qDebug() << "m_alphabet[" << i << "].isIncluded :" << m_alphabet[i].isIncluded;
                                     }
            );    

    if (i==atype::speChar)           
        QObject::connect(chBxTab[i], &QCheckBox::stateChanged, btnCustomSpeChar, &QPushButton::setEnabled);

    if (i==atype::words)
        QObject::connect(chBxTab[i], &QCheckBox::stateChanged, btnCustomWords, &QPushButton::setEnabled);    

    return chBxTab[i];

}

void pwdGenerator::customSpeCharSet()
{
    bool ok;
    QString str = QInputDialog::getText(this,
                                        "Special characters set",                                        
                                        "Please, give a list of characters without\n"
                                        "any separator or white space, e.g. &&-@_:",
                                        QLineEdit::Normal,
                                        m_alphabet[atype::speChar].set,
                                        &ok,
                                        Qt::MSWindowsFixedSizeDialogHint);

    if (ok)
    {
        if(!str.isEmpty())
        {
            m_alphabet[atype::speChar].set = str;
            m_alphabet[atype::speChar].size = str.size();           
        }
        else
            chBxTab[atype::speChar]->setCheckState(Qt::Unchecked);
    }
}

void pwdGenerator::customWordSet()
{
    wordInputDialog * wordDlg = new wordInputDialog;
    wordDlg->setText(m_alphabet[atype::words].set);
    wordDlg->setCheckBoxState(m_wordFirstChBxState);
    QString str;
    if (wordDlg->exec() == QDialog::Accepted)
    {
        str = wordDlg->getText();
        m_wordFirstChBxState = wordDlg->getCheckBoxState();
        if (!str.isEmpty())
        {
            m_alphabet[atype::words].set = str;
            m_wordList = str.split(", ");
            m_alphabet[atype::words].size = m_wordList.size();            
        }
        else
            chBxTab[atype::words]->setCheckState(Qt::Unchecked);
    }
    delete wordDlg; wordDlg = nullptr;

}



void pwdGenerator::generate()
{
    QVector<int> index;   /* tableau d'indices des alphabets inclus par l'itilisateur */
    QVector<int> index_l; /* tableau d'indices des alpabets A-Z et a-z éventuellment inclus par l'utilisateur */
    int a_used = 0;       /* nombre d'alphabets à utiliser pour la construction du mot de passe = taille du tableau m_index */
    int l_used = 0;
    for (int i=0; i < alpha::N_EXISTANTS; ++i)
        if (m_alphabet[i].isIncluded)
        {
            index.push_back(i);
            a_used++;
            if (i<=1) { index_l.push_back(i); l_used++; }
            if (i==atype::words and m_alphabet[atype::words].set.isEmpty())
                index.removeLast(); /*  Si checkBox "use words" est coché mais que l'utilisateur n'a jamais appuyé
                                        sur le bouton "customize" correspondant, on a :
                                        m_alphabet[atype::words].isIncluded = true,
                                        mais m_alphabet[atype::words].size = 0 et
                                        m_alphabet[atype::words].set et m_wordList sont vides.
                                        Dans ce cas, on doit exclure l'alphabet "words" de l'index */

        }


    if (!index.isEmpty())
    {
        bool useLetters = static_cast<bool>(l_used);
        bool useWords = m_alphabet[atype::words].isIncluded;
        bool useSpeCharOrNumbers = (m_alphabet[atype::speChar].isIncluded or
                m_alphabet[atype::nb].isIncluded);
        bool onlyWords = (useWords and !useLetters and !useSpeCharOrNumbers);

        QString pwd;

        if (onlyWords)
            pwd = genAllowCrop(m_wordList.size());
        else
        {
            if (useWords)
                pwd = genWordFirst(m_wordList, m_wordList.size(), index, a_used);
            else
            {
                if (useLetters)
                    pwd = genBegin2End1(index, a_used, index_l, l_used);
                else
                    pwd = genAllRandom(index, a_used);
            }
        }

        m_lnEdtPwd->setText(pwd);


    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot generate password from an empty set of characters or words.");
        msgBox.exec();
    }

}


QString pwdGenerator::genAllowCrop(const int & wListSize)
{
    /* Ingrédients pour mdp : ensemble non vide de "mots", i.e.
     * éléments du tableau m_wordList de taille wListSize.
     * Cas minimal: m_wordList est réduit à un seul mot formé par un seul caractère.
     * Cas maximal: m_wordList contient au moins un mot de longueur > m_len.
     * Comme genAllowCrop() est supposé être appelé rarement,
     * on simplifiera l'algorithme en faisant des tirages dans m_wordList avec remise.
     * Le mot de passe résultant de la concaténation d'éléments ainsi tirés
     * sera éventuellement coupé pour correspondre à la longueur m_len demandée. */

    int j; /* indice de mot dans l'alphabet m_wordList */

    QString el; /* élément rajouté au mot de passe pwd */

    QString pwd;     /* mot de passe à retourner */
    int lenC = 0;    /* longueur du mot de passe en cours de construction */

    while(lenC < m_len)
    {
        j = randInt(wListSize);        
        el = m_wordList[j];        
        pwd += el;
        lenC += el.size();
    }
    if (lenC > m_len)
        pwd.chop(lenC-m_len);

    return pwd;
}

QString pwdGenerator::genWordFirst(QStringList wordList, int wListSize,
                             const QVector<int> & index, const int & size)
{
    /* Ingrédients pour mdp : "words" (obligatoire) + au moins un autre alphabet (obligatoire)
     * structure du mot de passe : [*]RANDOM
     * où [*] est un mot choisi aléatoirement dans
     * m_wordList de taille m_alphabet[atype::words].size
     * On va travailler sur la copie du tableau m_wordList, stockée dans wordList.
     * On ne va pas vérifier que les mots de m_wordList sont tous de longueur <= m_len
     * On sait que m_wordList n'est pas vide.
     * Le mdp pourra commencer par un 'mot' si celui tiré aléatoirement
     * pour la première fois aura une longueur <= m_len.
     * Si ce n'est pas le cas, alors tout le mot de passe sera RANDOM, y compris son début. */


    int k; /* indice de l'alphabet */
    int j; /* indice d'une lettre dans l'alphabet */

    QString el; /* élément susceptible d'être rajouté au mot de passe pwd */
    int lenEl;  /* = el.size() */

    QString pwd;     /* mot de passe à retourner */
    int lenC = 0;    /* longueur du mot de passe en cours de construction */

    bool wordFirst = static_cast<bool> (m_wordFirstChBxState);

    while(lenC!=m_len)
    {
        if (wListSize) /* i.e. if ( !wordList.isEmpty() ) */
        {
            k = index[randInt(size)];
            if (wordFirst and lenC==0 and k!=atype::words) continue; // tente de mettre un mot en premier
            if (k==atype::words)
            {
                j = randInt(wListSize);
                el = wordList[j];
                lenEl = el.size();
            }
            else
            {
                j = randInt(m_alphabet[k].size);
                el = m_alphabet[k].set[j];
                lenEl = 1;
            }

            if (lenC + lenEl <= m_len)
            {
                pwd += el;
                lenC += lenEl;
                if (k==atype::words)
                {
                    wordList.removeAt(j);
                    wListSize--;
                }
            }
            else
                continue;
        }
        else  /* i.e. if ( wordList.isEmpty() ) */
        {
            k = index[randInt(size-1)]; /* on exclut la possibilité de tirer l'indice k==atype::words
                                         * (forcément le dernier de la liste index) */
            j = randInt(m_alphabet[k].size);
            pwd += m_alphabet[k].set[j];
            lenC += 1;
        }
    }

    return pwd;
}

QString pwdGenerator::genBegin2End1(const QVector<int> & index, const int & size,
                                    const QVector<int> & index_l, const int & size_l)
{
    /* Ingrédients pour mdp : lettres (majuscules ou minuscules, obligatoire)
     * et éventuellement nombres et/ou caractères spéciaux.
     * Les indices des alphabets à utiliser sont stockés dans index.
     * index et index_l sont non vides.
     * Structure du mot de passe : ** RANDOM *
     * où * désigne une lettre aléatoire (majuscule ou minuscule)
     * Remarque: cette structure est autorisée par la condition m_len >= 3 toujours remplie (cf. spBxPassLen) */

    QString str;
    int k; /* indice de l'alphabet */
    int j; /* indice d'une lettre dans l'alphabet */

    for (int i=0; i<m_len; ++i)
    {
        if (i==0 or i==1 or i==m_len-1)
            k = index_l[randInt(size_l)];
        else
            k = index[randInt(size)];
        j = randInt(m_alphabet[k].size);
        str += m_alphabet[k].set[j];
    }

    return str;
}

QString pwdGenerator::genAllRandom(const QVector<int> & index, const int size) // size = index.size();
{

    /* ingrédients pour mdp : nombres et/ou caractères spéciaux
     * les indices des alphabets à utiliser sont stockés dans index
     * index est non vide */

    QString str;
    int k; /* indice de l'alphabet */
    int j; /* indice d'une lettre dans l'alphabet */

    for (int i=0; i<m_len; ++i)
    {
        k = index[randInt(size)];
        j = randInt(m_alphabet[k].size);
        str += m_alphabet[k].set[j];
    }

    return str;
}


void pwdGenerator::reset()
{
    for(int i=0; i<alpha::N_EXISTANTS; i++)
        if (chBxTab[i]->checkState()!=chBox::InitialState[i])
            chBxTab[i]->setCheckState(chBox::InitialState[i]);

    m_alphabet[atype::speChar].set = alpha::DEFAULT_SPECHAR;
    m_alphabet[atype::speChar].size = 4;

    m_alphabet[atype::words].set = QString();
    m_alphabet[atype::words].size = 0;
    m_wordList = QStringList();
    m_wordFirstChBxState = Qt::Checked;
    spBxPassLen->setValue(8);

}
