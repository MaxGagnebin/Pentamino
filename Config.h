#ifndef CONFIG_H
#define CONFIG_H



using namespace std;
#include <vector>

enum Jeton{Vide = 0, Noir = -1, Blanc = 1};

class Config{
public:
    Config(int taille);
    Config(std::string Fichier, int depart, int taille);
    Config(int taille, int c0[6*6]);
    ~Config();
    int taille() const;
    int nbrPierre() const;
    int nbrPierre(Jeton coul) const;
    bool pose(Jeton coul, int ca);
    void remplace(Jeton coul, int ca);
    Jeton jeton(int ca) const;
    void rotation(int c1, int c2, int c3, int c4);
    void rotation();
    void reflexion();
    bool egal(Config const& a) const;
    void become(Config const& a);
    bool couleurPresente(Jeton coul, int c1, int c2, int c3, int c4, int c5) const;

protected:
    int const m_taille;
    Jeton m_config[6*6];
};

bool operator==(Config const& a, Config const& b);
Jeton operator!(Jeton const& a);

#endif // CONFIG_H
