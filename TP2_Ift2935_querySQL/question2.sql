-- Database: TP2_DB
/**
2 – Modifier le domaine des colonnes suivantes : indIP, nSalle, nLog, nPoste : Chaine de caractère, 
et supprimer la contrainte not null de la colonne dateIns. Nommez le fichier : question2.sql (20) 

**/
BEGIN TRANSACTION;



ALTER TABLE poste ADD CONSTRAINT  poste_segment_fk FOREIGN KEY (indIP) REFERENCES segment(indIP) ,
	ADD CONSTRAINT  poste_types_fk FOREIGN KEY (typePoste) REFERENCES Types(typeLP) ,
	ADD CONSTRAINT  poste_salle_fk FOREIGN KEY (nSalle) REFERENCES Salle(nSalle) ;
ALTER TABLE Installer ADD CONSTRAINT  Installer_poste_fk FOREIGN KEY (nPoste) REFERENCES Poste (nPoste) ,
	ADD CONSTRAINT   Installer_logiciel_fk FOREIGN KEY (nLog) REFERENCES Logiciel (nLog) ;
COMMIT;