// Ne modifiez pas ce fichier
#ifdef _CORRECTION__
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <conio.h>
#include "cvm 21.h"
#include "test.h"
#include "flux_io.h"
#include "Livres.h"
#include "Clients.h"
#include "Location.h"
#include "utils.h"

const std::string CORRECTION_FILENAME = "grille.csv";
const char DELIMITER = ',';
static int nbResult;
static int Total;

static std::stringstream resultCorrection;

static float roundUp(float toRound) {
	float t = toRound * 100;
	float c = ceil(t);
	return c / 100;
}

static void CreateCorrectionFile() {
	nbResult = 0;
	Total = 0;
	std::fstream Fichier;
	Fichier.open(CORRECTION_FILENAME, std::ios::out);

	Fichier << "Ajout au fichier de Livre (/1)"
		<< DELIMITER << "Ajout unique dans NouveauLivre (/1)"
		<< DELIMITER << "Ajout au fichier de Client (/1)"
		<< DELIMITER << "Ajout unique dans NouveauClient (/1)"
		<< DELIMITER << "Recherche de livre (/4)"
		<< DELIMITER << "Recherche de client (/4)"
		<< DELIMITER << "Mise à jour d'un livre (/1)"
		<< DELIMITER << "Mise à jour d'un client (/1)"
		<< DELIMITER << "Location d'un livre (/5)"
		<< DELIMITER << "Retour d'un livre (/11)"
		<< DELIMITER << "Affichage d'un livre (/5)"
		<< DELIMITER << "Affichage d'un client (/10)"
		<< DELIMITER << "Affichage de la liste des livres prêté (/2)"
		<< DELIMITER << "Affichage de la liste des clients en retard (/3)"
		<< DELIMITER << "Total (/50)"
		<< "\n";

	Fichier.close();
}

static void CloseCorrection() {
	std::fstream Fichier;
	Fichier.open(CORRECTION_FILENAME, std::ios::app);

	Fichier << DELIMITER << Total;
	resultCorrection << "\n\nTotal de l'évaluation : " << Total << "/50\n";

	Fichier.close();

	if (globalFileName != "--null") {
		std::string studentResultLine = "";
		// Récupération des résultats d'évaluation
		std::fstream Fichier;
		Fichier.open(CORRECTION_FILENAME, std::ios::in);
		std::getline(Fichier, studentResultLine);
		std::getline(Fichier, studentResultLine);
		Fichier.close();
		// Ajout à la feuille d'évaluation globale
		Fichier.open(globalFileName, std::ios::app);
		
		Fichier << MATRICULE << DELIMITER << NOM << DELIMITER << PRENOM
			<< studentResultLine << "\n";

		Fichier.close();
	}
}

static void AjoutPoint(int pointage) {
	std::fstream Fichier;
	Fichier.open(CORRECTION_FILENAME, std::ios::app);

	if (nbResult > 0)
		Fichier << DELIMITER;
	Fichier << pointage;
	resultCorrection << pointage;
	nbResult++;
	Total += pointage;

	Fichier.close();
}

static void SetFlux() {
	ecrire.clear();
	ecrire.str("");
	lire.clear();
}

static void TestNouveauLivre() {
	resultCorrection << "Test de la fonction NouveauLivre :\n";
	remove(FILENAME_LIVRE.c_str());
	std::string Titre = "Un guide galactique";
	std::string Auteur = "Douglas Adam";
	lire << Titre << "\n" << Auteur;
	NouveauLivre();

	std::fstream Fichier;
	Fichier.open(FILENAME_LIVRE, std::ios::in | std::ios::binary);

	// Parcourir tout le fichier -> doit posséder une seule entrée
	Livre_s resultLivre = {};
	Livre_s test = { "Un guide galactique", "Douglas Adam", false };

	resultCorrection << "\tAjout du livre {" << test.Titre << ", " << test.Auteur << "} : ";
	Fichier.read((char*)&resultLivre, sizeof(Livre_s));
	if (LivreEquals(resultLivre, test)) 
		AjoutPoint(1);
	else
		AjoutPoint(0);

	resultCorrection << "/1\n";

	resultLivre = {};
	resultCorrection << "\tAjout unique d'un livre : ";
	Fichier.read((char*)&resultLivre, sizeof(Livre_s));
	if (Fichier.eof() && LivreEquals(resultLivre, {}))
		AjoutPoint(1);
	else
		AjoutPoint(0);
	resultCorrection << "/1\n";

	Fichier.close();
}

static void TestNouveauClient() {
	resultCorrection << "\n\nTest de la fonction NouveauClient :\n";
	remove(FILENAME_CLIENT.c_str());
	lire << "Gutenberg\nJohannes\n"
		<< "22101454\n" << 1455 
		<< " De la Bible\nAllemagne\nC1C1C1";
	NouveauClient();

	std::fstream Fichier;
	Fichier.open(FILENAME_CLIENT, std::ios::in | std::ios::binary);

	// Parcourir tout le fichier -> doit posséder une seule entrée
	Client_s result = {};
	Client_s test = { "Gutenberg", "Johannes", "22101454", { 1455, "De la Bible", "Allemagne", "C1C1C1" }, 0, { } };

	resultCorrection << "\tAjout du client {Gutenberg, Johannes} : ";
	Fichier.read((char*)&result, sizeof(Client_s));
	if (ClientEquals(result, test))
		AjoutPoint(1);
	else
		AjoutPoint(0);
	resultCorrection << "/1\n";

	resultCorrection << "\tAjout unique d'un client : ";
	result = {};
	Fichier.read((char*)&result, sizeof(Client_s));
	if (Fichier.eof() && ClientEquals(result, {}))
		AjoutPoint(1);
	else
		AjoutPoint(0);
	resultCorrection << "/1\n";

	Fichier.close();
}

static void TestRechercheClient() {
	resultCorrection << "\n\nTest de la fonction RechercheClient :\n";
	int pointage = 0;
	lire << "Roberge\nJean-Francois\n"
		<< "5147883325\n" << 600
		<< " Fullum 10e Etage\nMontreal\nH2K3L6\n"<< "Plante\nValerie\n"
		<< "5148720311\n" << 275
		<< " Notre Dame E\nMontreal\nH2Y1C6\n"<< "Trudeau\nJustin\n"
		<< "6139575555\n" << 24
		<< " Sussez\nOttawa\nK1M1M4";
	NouveauClient();
	NouveauClient();
	NouveauClient();

	Client_s resultat = RechercherDossierClient(0);
	Client_s test = { "Gutenberg", "Johannes", "22101454", { 1455, "De la Bible", "Allemagne", "C1C1C1" }, 0, { } };
	pointage += ClientEquals(resultat, test);
	resultCorrection << "\tRecherche du client {Gutenberg, Johannes} : " << ClientEquals(resultat, test) << "/1\n";

	resultat = RechercherDossierClient(1);
	test = { "Roberge", "Jean-Francois", "5147883325", { 600, "Fullum 10e Etage", "Montreal", "H2K3L6" }, 0, { } };
	pointage += ClientEquals(resultat, test);
	resultCorrection << "\tRecherche du client {Roberge, Jean-Francois} : " << ClientEquals(resultat, test) << "/1\n";

	resultat = RechercherDossierClient(2);
	test = { "Plante", "Valerie", "5148720311", { 275, "Notre Dame E", "Montreal", "H2Y1C6" }, 0, { } };
	pointage += ClientEquals(resultat, test);
	resultCorrection << "\tRecherche du client {Plante, Valerie} : " << ClientEquals(resultat, test) << "/1\n";

	resultat = RechercherDossierClient(45);
	test = { };
	pointage += ClientEquals(resultat, test);
	resultCorrection << "\tRecherche du client inexistant {} : " << ClientEquals(resultat, test) << "/1\n";
	
	resultCorrection << "\t\tTotal : "; 
	AjoutPoint(pointage);
	resultCorrection << "/4\n";
}

static void TestRechercheLivre() {
	resultCorrection << "\n\nTest de la fonction RechercheLivre :\n";
	int pointage = 0;
	lire << "Office 2019\nCollette Michel";
	NouveauLivre();
	SetFlux();
	lire << "Petit guide de survie\nComité Chômage de Montréal";
	NouveauLivre();
	SetFlux();
	lire << "Programmer en langage C\nClaude Delannoy";
	NouveauLivre();
	SetFlux();
	lire << "Le langage C++\nClaus Richter";
	NouveauLivre();

	Livre_s resultat = RechercherLivre(0);
	Livre_s test = { "Un guide galactique", "Douglas Adam", false };
	pointage += (int)LivreEquals(resultat, test);
	resultCorrection << "\tRecherche du livre {Un guide galactique, Douglas Adam} : " << LivreEquals(resultat, test) << "/1\n";

	resultat = RechercherLivre(2);
	test = { "Petit guide de survie", "Comité Chômage de Montréal", false };
	pointage += (int)LivreEquals(resultat, test);
	resultCorrection << "\tRecherche du livre {Petit guide de survie, Comité Chômage de Montréal} : " << LivreEquals(resultat, test) << "/1\n";

	resultat = RechercherLivre(4);
	test = { "Le langage C++", "Claus Richter", false };
	pointage += (int)LivreEquals(resultat, test);
	resultCorrection << "\tRecherche du livre {Le langage C++, Claus Richter} : " << LivreEquals(resultat, test) << "/1\n";

	resultat = RechercherLivre(900);
	test = { };
	pointage += (int)LivreEquals(resultat, test);
	resultCorrection << "\tRecherche du livre inexistant {} : " << LivreEquals(resultat, test) << "/1\n";

	resultCorrection << "\t\tTotal : ";
	AjoutPoint(pointage);
	resultCorrection << "/4\n";
}

static void TestMiseJourLivre() {
	Livre_s maj = { "Petit guide de survie des chômeurs et chômeuses", "Comité Chômage de Montréal", false };
	MiseAJourLivre(2, maj);
	Livre_s recherche = RechercherLivre(2);

	resultCorrection << "\n\nTest de la fonction MiseAJourLivre :\n"
		<< "\tMise à jour du livre {Petit guide de survie des chômeurs et chômeuses, Comité Chômage de Montréal} : ";
	AjoutPoint(LivreEquals(recherche, maj));
	resultCorrection << "/1\n";
}

static void TestMiseJourClient() {
	Client_s maj = { "Plante", "Valerie", "5148820311", { 275, "Notre Dame E", "Montreal", "H2Y1C6" }, 0, { } };
	MiseAJourClient(2, maj);
	Client_s recherche = RechercherDossierClient(2);

	resultCorrection << "\n\nTest de la fonction MiseAJourClient :\n"
		<< "\tMise à jour du client {Plante, Valerie} : ";
	AjoutPoint(ClientEquals(recherche, maj));
	resultCorrection << "/1\n";
}

static void TestLocation() {
	resultCorrection << "\n\nTest de la fonction Location :\n";
	int pointage = 0;
	Date_s location = Aujourdhui(), retour = AjouterJours(location, 15);
	Livre_s majLivre = { "Petit guide de survie des chômeurs et chômeuses", "Comité Chômage de Montréal", true };
	Client_s majClient = { "Plante", "Valerie", "5148820311", { 275, "Notre Dame E", "Montreal", "H2Y1C6" }, 1, { {2, location, retour}, {}, {} } };

	Date_s dateRetour = Location(2, 2);
	Client_s resultatClient = RechercherDossierClient(2);
	Livre_s resultatLivre = RechercherLivre(2);
	pointage += (int)ClientEquals(resultatClient, majClient) 
		+ (int)LivreEquals(resultatLivre, majLivre)
		+ (int)DateEqual(dateRetour, retour);

	resultCorrection << "\tLocation effectué au client {Plante, Valerie} : " << ClientEquals(resultatClient, majClient) << "/1\n"
		<< "\tLocation effectué pour le livre {Petit guide de survie des chômeurs et chômeuses, Comité Chômage de Montréal} : " << LivreEquals(resultatLivre, majLivre) << "/1\n"
		<< "\tLa date de retour est {" << retour.jour << "/" << retour.mois << "/" << retour.annee << "} : " << DateEqual(dateRetour, retour) << "/1\n";

	dateRetour = Location(0, 2);
	pointage += (int)DateEqual(dateRetour, {});
	resultCorrection << "\tLocation non effectué pour le livre {Petit guide de survie des chômeurs et chômeuses, Comité Chômage de Montréal} (car déjà prêté) : " 
		<< DateEqual(dateRetour, {}) << "/1\n";

	Location(2, 0);
	Location(2, 1);
	dateRetour = Location(2, 3);
	pointage += (int)DateEqual(dateRetour, {});
	resultCorrection << "\tLocation non effectué pour le client {Plante, Valerie} (car déjà au maximum) : "
		<< DateEqual(dateRetour, {}) << "/1\n";

	resultCorrection << "\t\tTotal : ";
	AjoutPoint(pointage);
	resultCorrection << "/5\n";
}

static void TestRetour() {
	resultCorrection << "\n\nTest de la fonction Retour :\n";
	size_t pointage = 0;
	Date_s location = Aujourdhui(), retour = AjouterJours(location, 15);
	Livre_s livreRetour1 = { "Petit guide de survie des chômeurs et chômeuses", "Comité Chômage de Montréal", false };
	Livre_s livreRetour2 = { "Un guide galactique", "Douglas Adam", true };
	Livre_s livreRetour3 = { "Office 2019", "Collette Michel", true };
	Client_s client1 = { "Plante", "Valerie", "5148820311", { 275, "Notre Dame E", "Montreal", "H2Y1C6" }, 2, { {0, location, retour}, {1, location, retour}, {} } };
	Client_s client2 = { "Plante", "Valerie", "5148820311", { 275, "Notre Dame E", "Montreal", "H2Y1C6" }, 1, { {1, location, retour}, {}, {} } };
	Client_s client3 = { "Plante", "Valerie", "5148820311", { 275, "Notre Dame E", "Montreal", "H2Y1C6" }, 0, { } };

	int p = (int)(roundUp(Retour(2, 2, AjouterJours(retour, 2))) == 11.5);
	resultCorrection << "\tRetour effectué pour le livre {Petit guide de survie des chômeurs et chômeuses, Comité Chômage de Montréal} (avec bon frais de retard) : "
		<< p << "/1\n";
	pointage += p;

	pointage += (int)LivreEquals(RechercherLivre(2), livreRetour1);
	resultCorrection << "\tBon état (dans le fichier) pour le premier livre retourné : "
		<< LivreEquals(RechercherLivre(2), livreRetour1) << "/1\n";

	pointage += (int)LivreEquals(RechercherLivre(0), livreRetour2);
	resultCorrection << "\tBon état (dans le fichier) pour le deuxième livre retourné (toujours emprunté) : "
		<< LivreEquals(RechercherLivre(0), livreRetour2) << "/1\n";
	livreRetour2.Loue = false;

	pointage += (int)LivreEquals(RechercherLivre(1), livreRetour3);
	resultCorrection << "\tBon état (dans le fichier) pour le troisième livre retourné (toujours emprunté) : "
		<< LivreEquals(RechercherLivre(1), livreRetour3) << "/1\n";
	livreRetour3.Loue = false;

	pointage += (int)ClientEquals(RechercherDossierClient(2), client1);
	resultCorrection << "\tBon état (dans le fichier) pour le client {Plante, Valerie} : "
		<< ClientEquals(RechercherDossierClient(2), client1) << "/1\n";

	p = (int)(roundUp(Retour(2, 0, retour)) == 0);
	resultCorrection << "\tRetour effectué pour le livre {Un guide galactique, Douglas Adam} (sans frais de retard) : "
		<< p << "/1\n";
	pointage += p;

	pointage += (int)LivreEquals(RechercherLivre(0), livreRetour2);
	resultCorrection << "\tBon état (dans le fichier) pour le deuxième livre retourné (maintenant retourné) : "
		<< LivreEquals(RechercherLivre(0), livreRetour2) << "/1\n";

	pointage += (int)ClientEquals(RechercherDossierClient(2), client2);
	resultCorrection << "\tBon état (dans le fichier) pour le client {Plante, Valerie} : "
		<< ClientEquals(RechercherDossierClient(2), client2) << "/1\n";

	p = (int)(roundUp(Retour(2, 1, AjouterJours(retour, -3))) == 0);
	resultCorrection << "\tRetour effectué pour le livre {Un guide galactique, Douglas Adam} (retour en avance) : "
		<< p << "/1\n";
	pointage += p;

	pointage += (int)LivreEquals(RechercherLivre(1), livreRetour3);
	resultCorrection << "\tBon état (dans le fichier) pour le troisième livre retourné (toujours emprunté) : "
		<< LivreEquals(RechercherLivre(1), livreRetour3) << "/1\n";

	pointage += (int)ClientEquals(RechercherDossierClient(2), client3);
	resultCorrection << "\tBon état (dans le fichier) pour le client {Plante, Valerie} : "
		<< ClientEquals(RechercherDossierClient(2), client3) << "/1\n";

	resultCorrection << "\t\tTotal : ";
	AjoutPoint(pointage);
	resultCorrection << "/11\n";
}

void TestAfficherLivre() {
	resultCorrection << "\n\nTest de la fonction AfficherLivre :\n";

	std::cout << "-------------Afficher livre-------------\n\n\nDONNÉE ATTENDU (dispo) :\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< "ID du livre : " << 2 << "\n"
		<< "Titre du livre : Petit guide de survie des chômeurs et chômeuses\n"
		<< "Auteur du livre : Comité Chômage de Montréal\n"
		<< "Disponible : Oui"
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	AfficherLivre(2);
	std::cout << "\n\nAFFICHAGE DE LA PERSONNE ETUDIANTE\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< ecrire.str()
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	std::cout << "Combien de donnée presente est valide (max 4) ? ";
	size_t affichage, pointage = 0;
	affichage = _getche() - 48;
	if (affichage < 0 || affichage > 9)
		affichage = 0;
	clrscr();
	pointage = (affichage > 4 ? 4 : affichage);
	resultCorrection << "\tAffichage des données du livre {Petit guide de survie des chômeurs et chômeuses, Comité Chômage de Montréal} : "
		<< (affichage > 4 ? 4 : affichage) << "/4\n";

	std::cout << "-------------Afficher livre-------------\n\n\nDONNÉE ATTENDU (non-dispo) :\n===========================\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< "Ce livre n'existe pas."
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	SetFlux();
	AfficherLivre(900);
	std::cout << "\n\nAFFICHAGE DE LA PERSONNE ETUDIANTE\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< ecrire.str()
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	std::cout << "Est-ce que le message est bien present (O/N) ? ";
	char rep = toupper(_getche());
	if (rep == 'O')
		pointage++;
	resultCorrection << "\tAffichage des données du livre inexistant { } : "
		<< (rep == 'O') << "/1\n";
	clrscr();

	resultCorrection << "\t\tTotal : ";
	AjoutPoint(pointage);
	resultCorrection << "/5\n";
}

void TestAfficherDossierClient() {
	resultCorrection << "\n\nTest de la fonction AfficherDossierClient :\n";

	Location(0, 3);
	std::cout << "-------------Afficher client-------------\n\n\nDONNÉE ATTENDU (dispo) :\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< "ID du client : 0\n"
		<< "Nom du client : Gutenberg\n"
		<< "Prénom du client : Johannes\n"
		<< "Téléphone du client : 22101454\n"
		<< "Adresse du client : " << 1455 << " De la Bible, Allemagne (C1C1C1)\n"
		<< "Nombre de livre prêté : 1\n"
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	AfficherDossierClient(0);
	std::cout << "\n\nAFFICHAGE DE LA PERSONNE ETUDIANTE\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< ecrire.str()
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	std::cout << "Combien de donnée presente est valide (max 9) ? ";
	size_t affichage, pointage = 0;
	affichage = _getche() - 48;
	if (affichage < 0 || affichage > 9)
		affichage = 0;
	clrscr();
	pointage = (affichage > 9 ? 9 : affichage);
	resultCorrection << "\tAffichage des données du client {Gutenberg, Johannes} : "
		<< (affichage > 9 ? 9 : affichage) << "/9\n";

	std::cout << "-------------Afficher client-------------\n\n\nDONNÉE ATTENDU (non-dispo) :\n===========================\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< "Ce client n'existe pas."
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	SetFlux();
	AfficherDossierClient(900);
	std::cout << "\n\nAFFICHAGE DE LA PERSONNE ETUDIANTE\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< ecrire.str()
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	std::cout << "Est-ce que le message est bien present (O/N) ? ";
	char rep = toupper(_getche());
	if (rep == 'O')
		pointage++;
	resultCorrection << "\tAffichage des données du client inexistant { } : "
		<< (rep == 'O') << "/1\n";
	clrscr();

	resultCorrection << "\t\tTotal : ";
	AjoutPoint(pointage);
	resultCorrection << "/10\n";
}

void TestListeDesLivresPretes() {
	resultCorrection << "\n\nTest de la fonction ListeDesLivresPretes :\n";

	std::cout << "-------------Afficher liste livre prêté-------------\n\n\nDONNÉE ATTENDU (dispo) :\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< "Liste des livres Prêtés\n=======================\nID : Titre\n----------\n3 : Programmer en langage C"
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	ListeDesLivresPretes();
	std::cout << "\n\nAFFICHAGE DE LA PERSONNE ETUDIANTE\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< ecrire.str()
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	std::cout << "Combien de donnée presente est valide (max 2) ? ";
	size_t affichage, pointage = 0;
	affichage = _getche() - 48;
	if (affichage < 0 || affichage > 9)
		affichage = 0;
	clrscr();
	pointage = (affichage > 2 ? 2 : affichage);

	resultCorrection << "\tAffichage du livre prêté {Programmer en langage C, Claus Richter} : ";
	AjoutPoint(pointage);
	resultCorrection << "/2\n";
}

void TestListeDesClientsEnRetard() {
	Date_s date_retour = AjouterJours(Aujourdhui(), 15);
	resultCorrection << "\n\nTest de la fonction ListeDesClientsEnRetard :\n";
	std::cout << "-------------Afficher liste client en retard-------------\n\n\nDONNÉE ATTENDU (dispo) :\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< "Liste des clients en retard\n=======================\n\nNom (Téléphone) : Date de retour prévu\n----------\n"
		<< "Johannes Gutenberg (22101454) : " << date_retour.jour << "/" << date_retour.mois << "/" << date_retour.annee
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	ListeDesClientsEnRetard();
	std::cout << "\n\nAFFICHAGE DE LA PERSONNE ETUDIANTE\n"
		<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
		<< ecrire.str()
		<< "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
	std::cout << "Combien de donnée presente est valide (max 3) ? ";
	size_t affichage, pointage = 0;
	affichage = _getche() - 48;
	if (affichage < 0 || affichage > 9)
		affichage = 0;
	clrscr();
	pointage = (affichage > 3 ? 3 : affichage);

	resultCorrection << "\tAffichage du client en retard {Gutenberg, Johannes} : ";
	AjoutPoint(pointage);
	resultCorrection << "/3\n";
}

void RunTest() {
	resultCorrection.str("");
	setwsize(149, 30);

	CreateCorrectionFile();

	TestNouveauLivre();
	SetFlux(); TestNouveauClient();
	SetFlux(); TestRechercheLivre();
	SetFlux(); TestRechercheClient();
	SetFlux(); TestMiseJourLivre();
	SetFlux(); TestMiseJourClient();
	SetFlux(); TestLocation();
	SetFlux(); TestRetour();
	SetFlux(); TestAfficherLivre();
	SetFlux(); TestAfficherDossierClient();
	SetFlux(); TestListeDesLivresPretes();
	SetFlux(); TestListeDesClientsEnRetard();

	CloseCorrection();

	if (globalFileName == "--null") {
		std::string str = resultCorrection.str();
		std::cout << str << "\nAppuyez sur une touche pour continuer.";
		_getch();
	}
}
#endif // DEBUG