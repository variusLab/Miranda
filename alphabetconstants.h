#ifndef ALPHABETCONSTANTS_H
#define ALPHABETCONSTANTS_H

#include <QWidget>
class QString;
class QStringList;


namespace alpha
{
    const int N_EXISTANTS = 5;

    const QString UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const QString LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
    const QString NUMBERS = "0123456789";
    const QString DEFAULT_SPECHAR = "&-@_";
    const int D_SPECHAR_COUNT = DEFAULT_SPECHAR.size();
}

namespace chBox {
    const QStringList LABEL = {      "Capital letters",
                                     "Small letters",
                                     "Numbers",
                                     "Special Characters",
                                     "Words"};

    const QStringList INFOBULLE = {  "A-Z",
                                     "a-z",
                                     "0-9",
                                     "Customisable set of special characters (by default, includes &-@_)",
                                     "Customisable groups of characters"};

    const Qt::CheckState InitialState [alpha::N_EXISTANTS] = {
                                              Qt::Checked,
                                              Qt::Checked,
                                              Qt::Checked,
                                              Qt::Checked,
                                              Qt::Unchecked };

    const int gridRowPos [alpha::N_EXISTANTS] = {2, 3, 4, 5, 6};

}

enum atype {cap, small, nb, speChar, words}; // types d'alphabets

#endif // ALPHABETCONSTANTS_H
