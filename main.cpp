
#include <iostream>
#include <time.h>
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include "Plateau.h"
#include <iterator>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int const taille(6);

float MiniMax(Plateau& pla, int k, float alpha, float beta, Jeton coul);


void coupReflechi(Plateau& pla, int nombPartie);
// Cette fonction joue un coup sur le plateau pla et retourne la note qu'elle donne a ce coup.
// Une note de 1000 annonce un coup gagnant,
// -90'000 un coup pour lequel l'adversaire gagne en 1 coup.
// La méthode utilise un nombre de partie aléatoire donne par nombPartie.

int evaluate(Plateau& pla);
// Le but de cette fonction est de prendre une configuration et de dire
// si la personne qui va jouer peut forcer la victoire (return 1)
// peut arracher la nulle (return 0) ou va forcement perdre (return -1)

int evaluate(Plateau& pla, int k);
// On va chercher a evaluer une position en limitant la profondeur de recherche a K coup.
// Les resultats sont:
// 2 - position gagnantes,
// 1 - certaines branches sont non-conclusives,
// 0 - on peut arracher la nulle,
// -1 - on va perdre.

int evaluateReflechi(Plateau& pla);

void coupPeuReflechi(Plateau& pla);
// Cette fontion joue un coup au hasard sur le plateau, sauf s'il existe
// une victoire un 1 coup.

Jeton joueUnePartie(const int m[36][288]);
// Cette fonction joue une partie et retourne le vainqueur.


class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index];
        }
        std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream& str)
        {
            std::string         line;
            std::getline(str, line);

            std::stringstream   lineStream(line);
            std::string         cell;

            m_data.clear();
            while(std::getline(lineStream, cell, ','))
            {
                m_data.push_back(cell);
            }
            // This checks for a trailing comma with no data after it.
            if (!lineStream && cell.empty())
            {
                // If there was a trailing comma then add an empty element.
                m_data.push_back("");
            }
        }
    private:
        std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}


int main (int argc, const char * argv[]){
    srand((unsigned)time(0));


    Plateau plateau(taille);
    while (plateau.nbrPierre()<36 && !plateau.victoire()){

        if (plateau.trait()==Blanc){
            plateau.joue(plateau.trait());
        }
        else{
            coupReflechi(plateau, 20);
        }

        plateau.affiche();

    }

    if (plateau.victoire(Blanc) && plateau.victoire(Noir))
        cout<< "Les deux couleurs sont alignees, match nul !"<<endl;
    else if (plateau.victoire(Noir))
        cout<<"Les noirs ont gagnes !"<<endl;
    else if (plateau.victoire(Blanc))
        cout<< "Les blancs ont gagnes !"<<endl;
    else cout<< "La partie n'a pas de vainqueur, match nul !"<<endl;

    plateau.~Plateau();
    return 0;
}



float MiniMax(Plateau& pla, int k, float alpha, float beta, Jeton coul){

	if ((k==0) || (pla.fini())){
		return ((float) pla.valeur()/100);
	}

	if (coul == Blanc){
		for (int coup=0; coup<288; coup++){
			if (alpha >= beta)
			    break;
			if (pla.canDo(coup)){
		        if (!pla.dejaCalcule(coup)){

			        alpha = max ( alpha , MiniMax(pla, k-1 , alpha, beta, Noir) );
			    }
			    pla.unDo(coup);
			}
		}
		return alpha;
	}
	else{
		for (int coup=0; coup<288; coup++){
			if (alpha >= beta)
    		    break;
			if (pla.canDo(coup)){
		        if (!pla.dejaCalcule(coup)){

    		        beta = min ( beta , MiniMax(pla, k-1 , alpha, beta, Blanc));
    		    }
    		    pla.unDo(coup);
    		}
		}
		return beta;
	}
}

void coupReflechi(Plateau& pla, int nombPartie){
    time_t start=time(0);
    double noteMax(-100000.0);
    Jeton coul=pla.trait();
    Plateau plateauTest(pla.taille());
    vector <int> ve(pla.enfants());
    int preference(ve[ve.size()]);

    for (unsigned i=0; i<ve.size(); i++){
        double noteTemp(0.0);
        if (pla.canDo(ve[i])){
            if (pla.victoire(coul) && !pla.victoire(plateauTest.trait()))
                noteTemp=+1000.0;
            else{
                for (int k=0;k<nombPartie;k++){
                    plateauTest.become(pla);
                    while (plateauTest.nbrPierre()<36 && !plateauTest.victoire()){
                        if (plateauTest.victoireEnUn()){
                            plateauTest.joueEnUn();
                        }
                        else{
                            plateauTest.coupAlea();
                        }
                    }
                    if (plateauTest.victoire(coul)){
                        noteTemp+=(1.0/(plateauTest.nbrPierre()-pla.nbrPierre()));
                    }
                    else{
                        noteTemp-=(1.0/(plateauTest.nbrPierre()-pla.nbrPierre()));
                    }
                }
            }

            if (noteTemp>noteMax){
                noteMax=noteTemp;
                preference=ve[i];
            }
            pla.unDo(ve[i]);
        }
    }
    time_t end =time(0);
    int timme = difftime(end, start);
    cout << pla.nbrPierre()  << ". Notation : " << noteMax
        << ". Coup prefere : " << preference
    	<< ". Temps de calcul :"<< timme << endl;
    pla.canDo(preference);
}

void coupPeuReflechi(Plateau& pla){
    if (pla.victoireEnUn())
        pla.joueEnUn();
    else
        pla.coupAlea();
}

int evaluate(Plateau& pla){
    Jeton coul=pla.trait();
    Plateau plateauTest(pla.taille());

    if (pla.victoireEnUn())
        return 2;

    int best(-1);
    for (int i=0; i<36;i++){
        for (int j=0;j<8;j++){
            if (pla.jeton(i)==Vide){
                plateauTest.become(pla);
                plateauTest.pose(plateauTest.trait(), i);
                plateauTest.tournebis(j);
                if (plateauTest.victoire(!coul) && !plateauTest.victoire(coul))
                    best=max(best,-1);
                else if (plateauTest.egalite())
                    best=max(best,0);
                else best=1;
            }
        }
    }
    if (best==-1)
        return -1;
    else if (best==0)
        return 0;

    best=-1;
    for (int i=0; i<36;i++){
        for (int j=0;j<8;j++){
            if (pla.jeton(i)==Vide){
                plateauTest.become(pla);
                plateauTest.pose(plateauTest.trait(), i);
                plateauTest.tournebis(j);
                if (evaluate(plateauTest)==-1)
                    return 1;
                else if (evaluate(plateauTest)==1)
                    best=max(best,-1);
                else if (evaluate(plateauTest)==0)
                    best=max(best,0);
            }
        }
    }
    return best;
}
int evaluateReflechi(Plateau& pla){
    Jeton coul(pla.trait());
    if (pla.victoire(!coul) && !pla.victoire(coul))
        return -1;

    coupReflechi(pla, 10);
    if (pla.victoire(coul) && !pla.victoire(!coul))
        return 1;


    Plateau plateauTest(pla.taille());

//    if (coupReflechi(plateauTest))
//        return evaluateReflechi(plateauTest);
//    else
    {
        int best(-1);
        for (int i=0; i<36;i++){
            for (int j=0;j<8;j++){
                if (pla.jeton(i)==Vide){
                    plateauTest.become(pla);
                    plateauTest.pose(plateauTest.trait(), i);
                    plateauTest.tournebis(j);
                    if (evaluateReflechi(plateauTest)==-1)
                        return max(best,-1);
                    else if (evaluateReflechi(plateauTest)==1)
                        return 1;
                    else if (evaluateReflechi(plateauTest)==0)
                        best=max(best,0);
                }
            }
        }
        return best;
    }
}
int evaluate(Plateau& pla, int k){

    if (k==0)
        return 1;

    Jeton coul=pla.trait();
    int best(-1);

    for (int i=0; i<36;i++){
        if (pla.pose(coul, i)){
            for (int j=0;j<8;j++){
                pla.tournebis(j);
                if (pla.victoire(coul)){
                    pla.undoIJ(i,j);
                    return 2;
                }
                else if (pla.victoire(!coul)){
                    if (j%2)
                        pla.tournebis(j-1);
                    else
                        pla.tournebis(j+1);
                    best=max(best,-1);
                }
                else if (pla.egalite()){
                    if (j%2)
                        pla.tournebis(j-1);
                    else
                        pla.tournebis(j+1);
                    best=max(best,0);
                }
                else{
                    if (j%2)
                        pla.tournebis(j-1);
                    else
                        pla.tournebis(j+1);
                    best=1;
                }
            }
            pla.remplace(Vide,i);
        }
    }
    if (best==-1)
        return -1;
    else if (best==0)
        return 0;
//    cout << "passe" << endl;

    best=-1;
    for (int i=0; i<36;i++){
        if (pla.pose(coul,i)){
            for (int j=0;j<8;j++)
//            if (!pla.dejaCalcule(i, j))
            {
                pla.tournebis(j);
                if (evaluate(pla,k-1)==-1){
                    pla.undoIJ(i,j);
                    return 2;
                }
                else if (evaluate(pla,k-1)==2){
                    if (j%2)
                        pla.tournebis(j-1);
                    else
                        pla.tournebis(j+1);
                    best=max(best,-1);
                }
                else if (evaluate(pla,k-1)==0){
                    if (j%2)
                        pla.tournebis(j-1);
                    else
                        pla.tournebis(j+1);
                    best=max(best,0);
                }
                else {
                    if (j%2)
                        pla.tournebis(j-1);
                    else
                        pla.tournebis(j+1);
                    best=1;
                }
            }
            pla.remplace(Vide,i);
        }
    }
    return best;
}

//template <typename DerivedA,typename DerivedB>
Jeton joueUnePartie(const int m[36][288]){
    Plateau plateau(taille);
    int res[288];
    int maxCol(0);

    while (plateau.nbrPierre()<36 && !plateau.victoire()){
       if (plateau.trait()==Blanc){
            // coup choisi par les matrices ci-dessus
            for (int i=0; i<36; i++){
                res[i]=0;
                for (int j=0; j<288; j++){
                    res[j]+=plateau.jeton(j)*m[i][j];
                }
            }
            do{
                maxCol = distance(res, max_element(res, res+288));
                res[maxCol]=-1;
            }while(!plateau.pose(plateau.trait(), maxCol%36));
            plateau.tournebis((int)(maxCol/36));
            // plateau.joue(plateau.trait());
            // plateau.coupAlea();
        }
        else{
           // coupPeuReflechi(plateau);
           plateau.coupAlea();
        }
    }

   if (plateau.victoire(Blanc) && plateau.victoire(Noir))
        return Vide;
    else if (plateau.victoire(Noir))
        return Noir;
    else if (plateau.victoire(Blanc))
        return Blanc;
    else return Vide;
}
