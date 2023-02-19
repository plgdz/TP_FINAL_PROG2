// Ajouter vos include
#include "Livres.h"

extern const string FILENAME_LIVRE = "Livres.bin";

void NouveauLivre() {
	Livre_s Livre;

	Livre.Loue = false;

	ecrire << "\n\nTitre du livre : ";	LireChaineC(Livre.Titre, MAX_CHAR); 
	ecrire << "\nAuteur : ";			LireChaineC(Livre.Auteur, MAX_CHAR);

	fstream fichierLivre;
	fichierLivre.open(FILENAME_LIVRE, ios::app | ios::binary);
	if (fichierLivre.fail())
		exit(EXIT_FAILURE);
	fichierLivre.write((char*)&Livre, sizeof(Livre));
	fichierLivre.close();

	return;
}

// Commence à 0
Livre_s RechercherLivre(int id) {
	// Si pas de livre, retourner un livre vide -> Livre_s livre = {}
	size_t nbLivre, finFichier;
	Livre_s Livre;
	fstream Fichier;
	Fichier.open(FILENAME_LIVRE, ios::in | ios::binary);

	if (Fichier.fail())
		exit(EXIT_FAILURE);

	nbLivre = 0;

	Fichier.seekg(0, ios::end);
	finFichier = Fichier.tellp();
	nbLivre = finFichier / sizeof(Livre_s);

	if (id > nbLivre)
		Livre = {};
	else
	{
		Fichier.seekg(sizeof(Livre_s) * (id), ios::beg);
		Fichier.read((char*)&Livre, sizeof(Livre_s));
	}

	Fichier.close();
	return Livre;
}

void MiseAJourLivre(int id, Livre_s livre) {
	fstream Fichier;
	Fichier.open(FILENAME_LIVRE, ios::in | ios::out | ios::binary);
	if (Fichier.fail())
		exit(EXIT_FAILURE);
	Fichier.seekp(sizeof(Livre_s) * (id), ios::beg);
	Fichier.write((char *)&livre, sizeof(Livre_s));
	Fichier.close();
}

void AfficherLivre(int id) {
	// Si aucun livre -> afficher "Ce livre n'existe pas"
	Livre_s Livre = {};
	bool aucunLivre = false;
	string oui = "Oui", non = "Non";

	aucunLivre = LivreEquals(Livre, RechercherLivre(id));

	if (aucunLivre) {
		ecrire << "Ce livre n'exsite pas";
	}	
	else
	{
		Livre = RechercherLivre(id);

		ecrire << "ID : " << id << "\n";
		ecrire << "Titre : " << Livre.Titre << "\n";
		ecrire << "Auteur : " << Livre.Auteur << "\n";
		ecrire << "Disponible : ";
		if (Livre.Loue == true)
			ecrire << "Non";
		else
			ecrire << "Oui";
	}
}
 
void ListeDesLivresPretes() {
	Livre_s Vide = {}, Livre;
	fstream Fichier;
	int id;

	Fichier.open(FILENAME_LIVRE, ios::in | ios::binary);

	if (Fichier.fail())
	{
		exit(EXIT_FAILURE);
	}

	id = 0;
	Fichier.read((char*)&Livre, sizeof(Livre_s));

	gotoxy(0, 0); clreoscr();
	gotoxy(5, 5); ecrire << "Liste des livres pretes\n\n";
	while (!Fichier.eof())
	{
		if (Livre.Loue == true)
		{
			ecrire << setw(5) << "" << "ID : " << id << endl;
			ecrire << setw(5) << "" << "Titre : " << Livre.Titre;
		}
		Fichier.read((char*)&Livre, sizeof(Livre_s));
		id++;
	}
}