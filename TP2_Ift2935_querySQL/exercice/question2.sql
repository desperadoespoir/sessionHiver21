-- Database: TP2_DB
/**
2 – Modifier le domaine des colonnes suivantes : indIP, nSalle, nLog, nPoste : Chaine de caractère, 
et supprimer la contrainte not null de la colonne dateIns. Nommez le fichier : question2.sql (20) 

**/
BEGIN TRANSACTION;

ALTER TABLE Segment ALTER COLUMN indIP SET DATA TYPE TEXT;

ALTER TABLE Salle ALTER COLUMN nSalle SET DATA TYPE TEXT,
	ALTER COLUMN indIP SET DATA TYPE TEXT;

ALTER TABLE Logiciel ALTER COLUMN nLog SET DATA TYPE TEXT;

ALTER TABLE Poste ALTER COLUMN nPoste SET DATA TYPE TEXT,
	ALTER COLUMN indIP SET DATA TYPE TEXT,
	ALTER COLUMN nSalle SET DATA TYPE TEXT;
	
ALTER TABLE Installer ALTER COLUMN dateIns DROP NOT NULL,
	ALTER COLUMN nPoste SET DATA TYPE TEXT,
	ALTER COLUMN nLog SET DATA TYPE TEXT;

COMMIT;
