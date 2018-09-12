#ifndef PLATEAU_H
#define PLATEAU_H

#include <Config.h>


class Plateau : public Config{
public:
    Plateau(int taille);
    Plateau(std::string nomFichier, int depart, int taille);
    Plateau(int taille, int initpla[36]);
    ~Plateau();

    void tourne(int c, int d);
    bool tournebis(int q);
    void affiche() const;
    int test(int c1,int c2,int c3,int c4,int c5) const;
    int test(int l) const;
    bool couleurAligne(Jeton coul) const;
    int valeur() const;
    vector <int> bebes();
    vector <int> enfants();
    void pasPerdant(vector<int>& coup);
    bool canDo(int coup);
    void unDo(int coup);
    bool victoire() const;
    bool victoire(Jeton coul) const;
    bool egalite() const;
    bool fini() const;
    void coupAlea();
    void joue(Jeton coul);
    Jeton trait() const;
    void undoIJ(int i, int j);
    bool victoireEnUn(); //regarde si coul peut gagner en un coup, et si oui, on sauve en m_x et m_y coup gagnant.
    void joueEnUn();
    bool dejaCalcule(int coup) const;
    int evaluate(int k);
    void noteCoup();
    double noteMax() const;
    int posiMax() const;
    int miniMax(int k, int alpha, int beta, Jeton coul, Jeton coulIni, int nbrCalcul);

private:
    int m_condition[32][5];
    vector <double> m_noteCoup; // utilise dans le monte-carlo
    int m_coupGagne;
};


#endif  // PLATEAU_H
