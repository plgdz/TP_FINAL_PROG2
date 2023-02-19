#define _CRT_SECURE_NO_WARNINGS 1
#include <string>
#include "flux_io.h"
#include <ctime>
#include "utils.h"

using namespace std;

void LireChaineC(char Chaine_C[], const int DIM)
{
    string Chaine;

    getline(lire, Chaine);

    Chaine.resize(DIM - 1);  // permet de tronquer Chaine si n�cessaire

    strcpy(Chaine_C, Chaine.c_str());   // affecte Chaine � Chaine_C

    return;
}

Date_s Aujourdhui() {
    Date_s date;
    time_t t = time(NULL);
    tm* now = localtime(&t);
    date.jour = now->tm_mday;
    date.mois = now->tm_mon + 1;
    date.annee = now->tm_year + 1900;
    date.heure = now->tm_hour;
    date.minute = now->tm_min;
    date.seconde = now->tm_sec;
    return date;
}

static bool Bissextile(int annee)
{
    if (annee % 400 == 0)
        return true;

    if (annee % 100 == 0)
        return false;

    if (annee % 4 == 0)
        return true;

    return false;
}

static int NbreJoursMois(int mois, int annee) {
    switch (mois) {
    case 1: case 3: case 5: case 7:
    case 8: case 10: case 12:
        return 31;
    case 2:
        if (Bissextile(annee))
            return 29;
        else return 28;
    default:
        return 30;
    }
}

Date_s AjouterJours(Date_s date, int nbreJour) {
    Date_s nDate = date;
    nDate.jour += nbreJour;
    bool fin = false;
    int nJourMois;
    do {
        nJourMois = NbreJoursMois(nDate.mois, nDate.mois);
        if (nDate.jour > nJourMois) {
            nDate.mois++;
            if (nDate.mois > 12) {
                nDate.mois = 1;
                nDate.annee++;
            }
            nDate.jour -= nJourMois;
            nJourMois = NbreJoursMois(nDate.mois, nDate.annee);
        }
    } while (nDate.jour > nJourMois);

    return nDate;
}

static int nbreJoursMois[] = { 0, 31, 59, 90, 120, 151,
    181, 223, 243, 273, 304, 334 };
static int nbreJourAnnee = 365;

static int nbJourBissextile(int a) {
    return (int)floor(a / 4) - (int)floor(a / 100)
        + (int)floor(a / 400);
}

static int correction(Date_s d1, Date_s d2) {
    int n = nbJourBissextile(d2.annee) - nbJourBissextile(d1.annee);

    if (Bissextile(d1.annee) and d1.mois < 3)
        n++;
    if (Bissextile(d2.annee) and d2.mois > 2)
        n--;

    return n;
}

int NombreJours(Date_s d1, Date_s d2) {
    int a = (d2.annee - d1.annee) * nbreJourAnnee;
    int b = correction(d1, d2);
    int m = nbreJoursMois[d2.mois - 1] - nbreJoursMois[d1.mois - 1];
    int j = d2.jour - d1.jour;

    return j + m + a + b;
}

bool LivreEquals(const Livre_s l1, const Livre_s l2) {
    return strcmp(l1.Titre, l2.Titre) == 0
        and strcmp(l1.Auteur, l2.Auteur) == 0
        and l1.Loue == l2.Loue;
}

bool AdresseEqual(const Adresse_s a1, const Adresse_s a2) {
    return a1.NumCivil == a2.NumCivil
        and strcmp(a1.Rue, a2.Rue) == 0
        and strcmp(a1.Ville, a2.Ville) == 0
        and strcmp(a1.CodePostal, a2.CodePostal) == 0;
}

bool PretEqual(const Pret_s p1, const Pret_s p2) {
    return p1.NumLivre == p2.NumLivre
        and DateEqual(p1.DateLocation, p2.DateLocation)
        and DateEqual(p1.DateRetourPrevu, p2.DateRetourPrevu);
}

bool DateEqual(const Date_s d1, const Date_s d2) {
    return d1.jour == d2.jour and d1.mois == d2.mois
        and d1.annee == d2.annee;
}

bool ClientEquals(const Client_s c1, const Client_s c2) {
    return strcmp(c1.Nom, c2.Nom) == 0
        and strcmp(c1.Prenom, c2.Prenom) == 0
        and PretEqual(c1.Prets[0], c2.Prets[0])
        and PretEqual(c1.Prets[1], c2.Prets[1])
        and PretEqual(c1.Prets[2], c2.Prets[2]);
}