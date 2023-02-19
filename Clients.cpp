// Ajouter vos include
#include "Clients.h"

extern const string FILENAME_CLIENT = "Clients.bin";

void NouveauClient() {
	Client_s  Client = {};
	
	Client.NombrePretActif = 0;

	ecrire << "Nom du client : ";						LireChaineC(Client.Nom, MAX_CHAR);
	ecrire << "Prenom du client : ";					LireChaineC(Client.Prenom, MAX_CHAR);
	ecrire << "Numero de telephone : ";					LireChaineC(Client.Telephone, 11);
	ecrire << "Numero civique : ";						lire >> Client.Adresse.NumCivil;
	ecrire << "Rue : ";									LireChaineC(Client.Adresse.Rue, MAX_CHAR);
	ecrire << "Ville : ";								LireChaineC(Client.Adresse.Ville, MAX_CHAR);
	ecrire << "Code postal (X0X0X0 - Aucun espace) : "; LireChaineC(Client.Adresse.CodePostal, 7);

	fstream fichierClient;
	fichierClient.open(FILENAME_CLIENT, ios::app | ios::binary);
	if (fichierClient.fail())
		exit(EXIT_FAILURE);
	fichierClient.write((char*)&Client, sizeof(Client_s));
	fichierClient.close();
}

Client_s RechercherDossierClient(int id) {
	// Si pas de client, retourner un client vide -> Client_s client = {}
	int nbClient, finFichier;
	Client_s client;
	fstream Fichier;

	Fichier.open(FILENAME_CLIENT, ios::in | ios::binary);
	if (Fichier.fail())
		exit(EXIT_FAILURE);

	nbClient = 0;
	Fichier.seekg(0, ios::end);
	finFichier = Fichier.tellp();
	nbClient = finFichier / sizeof(Client_s);

	if (id > nbClient)
		client = {};
	else
	{
		Fichier.seekg(sizeof(Client_s) * (id), ios::beg);
		Fichier.read((char*)&client, sizeof(Client_s));
	}

	Fichier.close();
	return client;
}

void MiseAJourClient(int id, Client_s client) {
	fstream Fichier;
	Fichier.open(FILENAME_CLIENT, ios::in | ios::out | ios::binary);
	if (Fichier.fail())
		exit(EXIT_FAILURE);
	Fichier.seekp(sizeof(Client_s) * (id), ios::beg);
	Fichier.write((char*)&client, sizeof(Client_s));
	Fichier.close();
}

void AfficherDossierClient(int id) {
	// Si aucun client -> afficher "Ce client n'existe pas"
	Client_s client = {};

	if (ClientEquals(client, RechercherDossierClient(id)))
		ecrire << "Ce client n'existe pas.";	
	else
	{
		client = RechercherDossierClient(id);

		ecrire << "\n\nINFORMATIONS DU CLIENT\n\n";
		ecrire << "ID : " << id << endl;
		ecrire << "Nom : " << client.Prenom << " " << client.Nom << endl;
		ecrire << "Telephone : " << client.Telephone << endl;
		ecrire << "Adresse : " << client.Adresse.NumCivil << " " << client.Adresse.Rue << ", " << client.Adresse.Ville << " " << client.Adresse.CodePostal << endl;
		ecrire << "Nombre de prets : " << client.NombrePretActif << endl;
	}	
}

void ListeDesClientsEnRetard() {
	Client_s client;
	fstream Fichier;
	int nbJourRetard;

	Fichier.open(FILENAME_CLIENT, ios::in | ios::binary);
	if (Fichier.fail())
		exit(EXIT_FAILURE);

	//parcours chaque client
	Fichier.read((char*)&client, sizeof(Client_s));
	while (!Fichier.eof())
	{
		if (client.NombrePretActif > 0)
		{
			for (size_t i = 0; i < 3; i++)
			{
				nbJourRetard = NombreJours(Aujourdhui(),client.Prets[i].DateRetourPrevu);
				if (nbJourRetard > 0)
				{
					ecrire << client.Prenom << " " << client.Nom << " (" << client.Telephone << ") : " <<
						client.Prets[i].DateRetourPrevu.jour << "/" << client.Prets[i].DateRetourPrevu.mois << "/" << client.Prets[i].DateRetourPrevu.annee << endl;
					break;
				}
			}
		}
		Fichier.read((char*)&client, sizeof(Client_s));
	}
	Fichier.close();
}