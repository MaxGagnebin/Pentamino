
#include <iostream>
#include <time.h>
#include <algorithm>
#include <vector>
#include "Plateau.h"
using namespace std;

Plateau::Plateau(int taille) : Config(taille) , m_coupGagne(0){
    for(int j=0;j<5;j++)
    {
        for(int i=0;i<m_taille;i++){
            m_condition[i][j]=i*m_taille+j;
            m_condition[m_taille+i][j]=i*m_taille+j+1;
            m_condition[2*m_taille+i][j]=j*m_taille+i;
            m_condition[3*m_taille+i][j]=(j+1)*m_taille+i;
        }
        m_condition[4*m_taille][j]=7*j;
        m_condition[4*m_taille+1][j]=7*(j+1);
        m_condition[4*m_taille+2][j]=6+7*j;
        m_condition[4*m_taille+3][j]=1+7*j;
        m_condition[4*m_taille+4][j]=5+5*j;
        m_condition[4*m_taille+5][j]=10+5*j;
        m_condition[4*m_taille+6][j]=4+5*j;
        m_condition[4*m_taille+7][j]=11+5*j;
    }
    m_noteCoup.resize(sizeof(double),288);
}

Plateau::Plateau(string nomFichier, int depart, int taille) : Config(nomFichier, depart, taille), m_coupGagne(0){
    for(int j=0;j<5;j++)
    {
        for(int i=0;i<m_taille;i++){
            m_condition[i][j]=i*m_taille+j;
            m_condition[m_taille+i][j]=i*m_taille+j+1;
            m_condition[2*m_taille+i][j]=j*m_taille+i;
            m_condition[3*m_taille+i][j]=(j+1)*m_taille+i;
        }
        m_condition[4*m_taille][j]=7*j;
        m_condition[4*m_taille+1][j]=7*(j+1);
        m_condition[4*m_taille+2][j]=6+7*j;
        m_condition[4*m_taille+3][j]=1+7*j;
        m_condition[4*m_taille+4][j]=5+5*j;
        m_condition[4*m_taille+5][j]=10+5*j;
        m_condition[4*m_taille+6][j]=4+5*j;
        m_condition[4*m_taille+7][j]=11+5*j;
    }
    m_noteCoup.resize(sizeof(double),288);
}

Plateau::Plateau(int taille, int initpla[36]) : Config(taille, initpla), m_coupGagne(0){
    for(int j=0;j<5;j++)
    {
        for(int i=0;i<m_taille;i++){
            m_condition[i][j]=i*m_taille+j;
            m_condition[m_taille+i][j]=i*m_taille+j+1;
            m_condition[2*m_taille+i][j]=j*m_taille+i;
            m_condition[3*m_taille+i][j]=(j+1)*m_taille+i;
        }
        m_condition[4*m_taille][j]=7*j;
        m_condition[4*m_taille+1][j]=7*(j+1);
        m_condition[4*m_taille+2][j]=6+7*j;
        m_condition[4*m_taille+3][j]=1+7*j;
        m_condition[4*m_taille+4][j]=5+5*j;
        m_condition[4*m_taille+5][j]=10+5*j;
        m_condition[4*m_taille+6][j]=4+5*j;
        m_condition[4*m_taille+7][j]=11+5*j;
    }
    m_noteCoup.resize(sizeof(double),288);
}

Plateau::~Plateau(){}

void Plateau::tourne(int c, int d)//la case en haut a gauche du quadrant a tourner
{
    rotation(c*m_taille+d, (c+2)*m_taille+d, (c+2)*m_taille+d+2, c*m_taille+d+2);
    rotation(c*m_taille+d+1, (c+1)*m_taille+d, (c+2)*m_taille+d+1, (c+1)*m_taille+d+2);
}

bool Plateau::tournebis(int q){
    if (q==0) {
        tourne(0,0);
        return true;
    }
    else if (q==1){
        tourne(0,0);
        tourne(0,0);
        tourne(0,0);
        return true;
    }
    else if (q==2){
        tourne(3,0);
        return true;
    }
    else if (q==3){
        tourne(3,0);
        tourne(3,0);
        tourne(3,0);
        return true;
    }
    else if(q==4){
        tourne(0,3);
        return true;
    }
    else if (q==5){
        tourne(0,3);
        tourne(0,3);
        tourne(0,3);
        return true;
    }
    else if (q==6){
        tourne(3,3);
        return true;
    }
    else if(q==7){
        tourne(3,3);
        tourne(3,3);
        tourne(3,3);
        return true;
    }
    else return false;
}

void Plateau::affiche() const{
    for(int i=0;i<m_taille;i++)
    {
        for(int j=0;j<m_taille;j++){
            if (jeton(i*m_taille+j)==Vide)
                cout<<"."<<"  ";
            if (jeton(i*m_taille+j)==Blanc)
                cout<< "2  " ;
            if (jeton(i*m_taille+j)==Noir)
                cout<< "1  " ;
            if (j==2)
                cout<<"|  ";
        }
        cout<<endl;
        if (i==2){
            cout<<"--------------------"<<endl;
        }
    }
    cout<<endl;
}

int Plateau::test(int c1,int c2,int c3,int c4,int c5) const{
    if (couleurPresente(Blanc,c1,c2,c3,c4,c5) && couleurPresente(Noir,c1,c2,c3,c4,c5)) return 0;
    else{
        return (jeton(c1)+jeton(c2)+jeton(c3)+jeton(c4)+jeton(c5));
    }
}


int Plateau::test(int l) const{
    return test(m_condition[l][0],m_condition[l][1],m_condition[l][2],m_condition[l][3],m_condition[l][4]);
}

bool Plateau::couleurAligne(Jeton coul) const{
    for (int i=0;i<32;i++){
        if ((test(i)==5) && (coul==Blanc))
            return true;
        if ((test(i)==-5) && (coul==Noir))
            return true;
    }
    return false;
}

int Plateau::valeur() const{
    float val(0);
    bool vicBlanc(false), vicNoir(false);
    for (int i=0;i<32;i++){
        if (test(i) == 5)
            vicBlanc = true;
        else if (test(i) == -5)
            vicNoir = true;
        else if (test(i) == 4)
            val += 10;
        else if (test(i) == 3)
            val += 1;
        else if (test(i)==-4)
            val -= 10;
        else if (test(i)==-3)
            val -= 1;
    }
    if (vicBlanc && vicNoir)
        return 0;
    else if (vicBlanc)
        return 100;
    else if (vicNoir)
        return -100;
    else return val;
}

vector <int> Plateau::bebes(){
    vector <int> bebes;
    for (unsigned coup=0; coup<288; coup++)
        bebes.push_back(coup);

    for (unsigned i=0; i < bebes.size(); i++){
        Plateau plaTemp(*this);
        if(canDo(bebes[i])){

            for (unsigned k=i+1; k < bebes.size(); k++){
                if (plaTemp.canDo(bebes[k])){
                    if (plaTemp==*this){
                        plaTemp.unDo(bebes[k]);
                        bebes.erase(bebes.begin()+k);
                        k--;
                    }
                    else
                        plaTemp.unDo(bebes[k]);
                }
            }
            unDo(bebes[i]);
        }
        else{
            bebes.erase(bebes.begin()+i);
            i--;
        }
    }
    return bebes;
}

vector <int> Plateau::enfants(){
    vector <int> enfants;
    vector <int> notes;

    if (trait() == Blanc){
        notes.push_back(-100);
        for (unsigned coup=0; coup<288; coup++){
            if (canDo(coup)){
                if (!dejaCalcule(coup)){
                    int val(valeur());
                    for (unsigned i=0; i < notes.size(); i++){
                        if (val >= notes.at(i)){
                            enfants.insert(enfants.begin()+i,coup);
                            notes.insert(notes.begin()+i,val);
                            break;
                        }
                    }
                }
                unDo(coup);
            }
        }
        return enfants;
    }
    else{
        notes.push_back(100);
        for (unsigned coup=0; coup<288; coup++){
            if (canDo(coup)){
                if (!dejaCalcule(coup)){
                    int val(valeur());
                    for (unsigned i=0; i < notes.size(); i++){
                        if (val <= notes.at(i)){
                            enfants.insert(enfants.begin()+i,coup);
                            notes.insert(notes.begin()+i,val);
                            break;
                        }
                    }
                }
                unDo(coup);
            }
        }
        return enfants;
    }
}

void Plateau::pasPerdant(vector<int>& coups){
    for (unsigned i=0; i<coups.size();i++){
        if (canDo(coups[i])){
            if (victoireEnUn()){
                unDo(coups[i]);
                coups.erase(coups.begin()+i);
                i--;
            }
            else
                unDo(coups[i]);
        }
    }
}

bool Plateau::victoire() const{
    if (victoire(Blanc) || victoire(Noir)){
            return true;
        }
    return false;
}

bool Plateau::victoire(Jeton coul) const{
    if (couleurAligne(coul) && !couleurAligne(!coul)){
            return true;
        }
    return false;
}

bool Plateau::egalite() const{
    if (victoire(Blanc) && victoire(Noir))
    return true;
    else if (nbrPierre()==36 && !victoire(Blanc) && !victoire(Noir))
    return true;
    else return false;
}

bool Plateau::fini() const{
    if (victoire(Blanc) || victoire(Noir))
        return true;
    else if (nbrPierre()==36)
        return true;
    else return false;
}

void Plateau::coupAlea(){
    int coup(0);
    do{
        coup=(rand()%288);
    }while(!canDo(coup));
}

void Plateau::joue(Jeton coul){
    int c(0);
    if (coul==Blanc){
        cout<<"Ou est-ce que blanc veut jouer?"<<endl;
    }
    else
        cout<<"Ou est-ce que noir veut jouer?"<<endl;
    cin>>c;

    while(!pose(coul,c)){
        cout<<"Cette position n'est pas valable, donnes-en une autre."<<endl;
        cin>>c;
    }

    cout<<"Quel quadrant tourner?"<<endl;
    cin>>c;
    while(!tournebis(c)){
        cout<<"Ce n'est pas valable, donne un quadrant entre 0 et 7."<<endl;
        cin>>c;
    }
}
// Fait jouer l'humain. La methode demande au joueur l'endroit
// ou jouer et quel quadrant a tourner.

Jeton Plateau::trait() const{
    if (nbrPierre()%2) {
        return Noir;
    }
    else return Blanc;
}

void Plateau::undoIJ(int i, int j){
    if (j%2)
        tournebis(j-1);
    else
        tournebis(j+1);
    remplace(Vide, i);
}

bool Plateau::victoireEnUn(){
    Jeton coul(trait());
    if (nbrPierre(coul)<4)
        return false;
    for (unsigned coup=0; coup<288; coup++) {
        if (canDo(coup)){
            if (victoire(coul)){
                m_coupGagne=coup;
                unDo(coup);
                return true;
            }
            unDo(coup);
        }
    }
    return false;
}

void Plateau::joueEnUn() {
    canDo(m_coupGagne);
}

bool Plateau::dejaCalcule(int coup) const{
    Plateau plaTemp(*this);
    plaTemp.unDo(coup);

    for (int i=0; i<coup; i++){
        if (plaTemp.canDo(i)){
            if (plaTemp==*this)
                return true;
            plaTemp.unDo(i);
        }
    }
    return false;
}

bool Plateau::canDo(int coup){
    if (pose(trait(),coup/8)){
        tournebis(coup-8*(coup/8));
        return true;
    }
    return false;
}

void Plateau::unDo(int coup){
    undoIJ(coup/8, coup-8*(coup/8));
}

int Plateau::evaluate(int k){

    if (k==0)
        return 1;

    Jeton coul=trait();
    int best(-1);

    for (int i=0; i<36;i++){
        if (pose(coul, i)){
            for (int j=0;j<8;j++){
                tournebis(j);
                if (victoire(coul)){
                    undoIJ(i,j);
                    return 2;
                }
                else if (victoire(!coul)){
                    if (j%2)
                        tournebis(j-1);
                    else
                        tournebis(j+1);
                    best=max(best,-1);
                }
                else if (egalite()){
                    if (j%2)
                        tournebis(j-1);
                    else
                        tournebis(j+1);
                    best=max(best,0);
                }
                else{
                    if (j%2)
                        tournebis(j-1);
                    else
                        tournebis(j+1);
                    best=1;
                }
            }
            remplace(Vide,i);
        }
    }
    if (best==-1)
        return -1;
    else if (best==0)
        return 0;
    best=-1;

    if (k==1)
    {
        for (int i=0; i<36;i++){
             if (pose(coul,i)){
                 for (int j=0;j<8;j++){
                    if (evaluate(k-1)==-1){
                        undoIJ(i,j);
                        return 2;
                    }
                    else if (evaluate(k-1)==2){
                        best=max(best,-1);
                    }
                    else if (evaluate(k-1)==0){
                        best=max(best,0);
                    }
                    else {
                        best=1;
                    }
                    if (j%2)
                        tournebis(j-1);
                    else
                        tournebis(j+1);
                 }
                 remplace(Vide,i);
             }
        }
        return best;
    }

    noteCoup();
    vector<double> noteTemp(m_noteCoup);
    while (*max_element(noteTemp.begin(), noteTemp.end())>-100000.0){
        int place=posiMax();
        int i(place%36);
        int j(place/36);
        pose(coul,i);
        tournebis(j);
        {
            if (evaluate(k-1)==-1){
                undoIJ(i,j);
                return 2;
            }
            else if (evaluate(k-1)==2){
                best=max(best,-1);
            }
            else if (evaluate(k-1)==0){
                best=max(best,0);
            }
            else {
                best=1;
            }
    }
        noteTemp[place]=-100000.0;
        if (j%2)
            tournebis(j-1);
        else
            tournebis(j+1);
        remplace(Vide,i);
    }
    return best;
}

void Plateau::noteCoup(){
    for (int coup=0;coup<288;coup++)
        m_noteCoup[coup]=-100000.0;

    Jeton coul=trait();

    for (unsigned coup=0; coup<288; coup++){
        if (canDo(coup)){;
            if (!dejaCalcule(coup)){

                if (victoire(coul) && !victoire(trait()))
                    m_noteCoup[coup]=+1000.0;
                else if (egalite())
                    m_noteCoup[coup]=0.0;
                else if (victoireEnUn())
                    m_noteCoup[coup]=-90000.0;
                else{
                    double noteTemp(0.0);
                    Plateau plateauTest(taille());
                    for (int k=0;k<100;k++){
                        plateauTest.become(*this);
                        while (plateauTest.nbrPierre()<36 && !plateauTest.victoire()){
                            if (plateauTest.victoireEnUn())
                                plateauTest.joueEnUn();
                            else
                                plateauTest.coupAlea();
                        }
                        if (plateauTest.victoire(coul))
                            noteTemp+=(1.0/(plateauTest.nbrPierre()-nbrPierre()));
                        else
                            noteTemp-=(1.0/(plateauTest.nbrPierre()-nbrPierre()));
                    }
                    m_noteCoup[coup]=noteTemp;
                }
            }
            unDo(coup);
        }
    }
}

double Plateau::noteMax() const{
    return *max_element(m_noteCoup.begin(), m_noteCoup.end());
}

int Plateau::posiMax() const{
    return (int)(max_element(m_noteCoup.begin(), m_noteCoup.end())-m_noteCoup.begin());
}

int Plateau::miniMax(int k, int alpha, int beta, Jeton coul, Jeton coulIni, int nbrCalcul){

    if ( (k==0) || (fini()) ){
        return valeur();
    }

    vector <int> coups;
    if (k==1)
        coups=bebes();
    else
        coups=enfants();
    if (k==5)
        cout << coups.size()<< endl;

    pasPerdant(coups);

    if(k==5){
        cout << coups.size() <<endl;

    for(int i=0;i<coups.size();i++)
        cout << coups[i] << "  ";
    cout << endl;
      }
    int nbrCoup;
    if ((coul == coulIni) && (nbrCalcul<=coups.size()))
        nbrCoup = nbrCalcul;
    else
        nbrCoup = coups.size();

    if (coul == Blanc){
        for (unsigned i=0; i<nbrCoup; i++){
            if (alpha >= beta)
                break;
            if (canDo(coups[i])){

                    int betaTemp(miniMax(k-1 , alpha, beta, Noir, coulIni, nbrCalcul));
                    if (betaTemp >= alpha){
                        m_coupGagne = coups[i];
                        alpha = betaTemp;
                    }

                unDo(coups[i]);
            }
        }
        return alpha;
    }
    else{
        for (unsigned i=0; i<nbrCoup; i++){
            if (alpha >= beta)
                break;
            if (canDo(coups[i])){
                    int alphaTemp(miniMax(k-1 , alpha, beta, Blanc, coulIni, nbrCalcul));
                    if (alphaTemp <= beta){
                        m_coupGagne = coups[i];
                        beta = alphaTemp;
                    }

                unDo(coups[i]);
            }
        }
        return beta;
    }
}
