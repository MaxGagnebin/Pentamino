
#include <iostream>
#include <fstream>
#include "Config.h"
using namespace std;

Config::Config(int taille):m_taille(taille){
    for(int i=0;i<m_taille;i++)
        for(int j=0;j<m_taille;j++)
            m_config[i*m_taille+j]=Vide;
}

Config::Config(string Fichier, int depart, int taille):m_taille(taille){
    //NB: il faut que dŽpart soit un multiple de 42 !
    ifstream monFlux(Fichier.c_str());
    if(monFlux){
        string ligne;
        while(getline(monFlux, ligne)){
            int position = monFlux.tellg();
            for (int i=0; i<taille; i++) {
                if (ligne[i]=='2')
                    m_config[((position/7)-1)*6+i]=Blanc;
                else if (ligne[i]=='1')
                    m_config[((position/7)-1)*6+i]=Noir;
                else m_config[((position/7)-1)*6+i]=Vide;
            }
        }
    }
    else{
        cout<<"Erreur avec le fichier."<< endl;
    }
}

Config::Config(int taille, int c0[6*6]):m_taille(taille){
    for (int i=0; i<36; i++){
        if (c0[i]==0)
            m_config[i]=Vide;
        else if (c0[i]==1)
            m_config[i]=Noir;
        else
            m_config[i]=Blanc;
    }
}

Config::~Config(){}

int Config::taille() const{
    return m_taille;
}

int Config::nbrPierre() const{
    int count(0);
    for(int i=0; i<36;i++){
        if (m_config[i]!=Vide)
            count++;
    }
    return count;
}

int Config::nbrPierre(Jeton coul) const{
    int count(0);
    for (int i=0; i<36; i++) {
        if (m_config[i]==coul)
            count++;
    }
    return count;
}

bool Config::pose(Jeton coul, int ca){
    if (m_config[ca]==Vide && 0<=ca && ca<=35){
        m_config[ca]=coul;
        return true;
    }
    else return false;
}

void Config::remplace(Jeton coul, int ca){
    if (0<=ca && ca<=35)
        m_config[ca]=coul;
}

Jeton Config::jeton(int ca) const{
    return m_config[ca];
}

void Config::rotation(int c1, int c2, int c3, int c4){
    Jeton temp(Vide);
    temp=m_config[c1];
    m_config[c1]=m_config[c2];
    m_config[c2]=m_config[c3];
    m_config[c3]=m_config[c4];
    m_config[c4]=temp;
}

void Config::reflexion(){
    Jeton temp;
    for (int i=0; i<6; i++) {
        for (int j=0; j<3; j++){
            temp=m_config[i*m_taille+j];
            m_config[i*m_taille+j]=m_config[i*m_taille-j+5];
            m_config[i*m_taille+5-j]=temp;
        }
    }
}

void Config::rotation(){
    for(int i=0;i<m_taille-1;i++){
        rotation(i,5+i*m_taille,35-i,30-6*i);
    }
    for(int i=0; i<m_taille-3;i++){
        rotation(7+i,10+6*i,28-i,25-6*i);
    }
    rotation(14,15,21,20);
}

bool Config::egal(const Config & a) const{
    for (int j=0; j<36; j++){
        if (jeton(j)!=a.jeton(j)){
            return false;
        }
    }
    return true;
}

void Config::become(Config const& a){
    for (int i=0; i<36;i++){
        m_config[i]=a.jeton(i);
    }
}

bool Config::couleurPresente(Jeton coul, int c1, int c2, int c3, int c4, int c5) const{
    if (jeton(c1)==coul) return true;
    else if (jeton(c2)==coul) return true;
    else if (jeton(c3)==coul) return true;
    else if (jeton(c4)==coul) return true;
    else if (jeton(c5)==coul) return true;
    else return false;
}

bool operator==(Config const& a, Config const& b){
    Config temp(b);
    for (int i=0; i<4; i++) {
        if (a.egal(temp))
            return true;
        temp.rotation();
    }
    temp.reflexion();
    for (int i=0; i<4; i++) {
        if (a.egal(temp))
            return true;
        temp.rotation();
    }
    return false;
}

Jeton operator!(Jeton const&a){
    if (a==Blanc)
        return Noir;
    else if (a==Noir)
        return Blanc;
    else return Vide;
}
