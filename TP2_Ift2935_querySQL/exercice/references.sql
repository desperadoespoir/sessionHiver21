BEGIN TRANSACTION;

-- Question 6
DELETE FROM Logiciel AS l WHERE NOT EXISTS (SELECT t.typeLP FROM Types t WHERE t.typeLP = l.typeLog);
DELETE FROM Salle AS sa WHERE NOT EXISTS (SELECT se.indIP FROM Segment se WHERE se.indIP = sa.indIP);

-- Question 4
ALTER TABLE Logiciel ADD CONSTRAINT logiciel_types_fk FOREIGN KEY (typeLog) REFERENCES Types(typeLP);

ALTER TABLE Poste ADD CONSTRAINT poste_segment_fk FOREIGN KEY (indIP) REFERENCES Segment(indIP),
    ADD CONSTRAINT poste_types_fk FOREIGN KEY (typePoste) REFERENCES Types(typeLP),
    ADD CONSTRAINT poste_salle_fk FOREIGN KEY (nSalle) REFERENCES Salle(nSalle);

ALTER TABLE Salle ADD CONSTRAINT salle_segment_fk FOREIGN KEY (indIP) REFERENCES Segment(indIP);

ALTER TABLE Installer ADD CONSTRAINT installer_poste_fk FOREIGN KEY (nPoste) REFERENCES Poste(nPoste),
    ADD CONSTRAINT installer_logiciel_fk FOREIGN KEY (nLog) REFERENCES Logiciel(nLog);

COMMIT;

/*
Question 5 :

psql:references.sql:7: ERROR:  insert or update on table "salle" violates foreign key constraint "salle_segment_fk"
DETAIL:  Key (indip)=(130.120.83) is not present in table "segment".

psql:references.sql:3: ERROR:  insert or update on table "logiciel" violates foreign key constraint "logiciel_types_fk"
DETAIL:  Key (typelog)=(BeOS) is not present in table "types".

SELECT * FROM Salle AS sa WHERE NOT EXISTS (SELECT se.indIP FROM Segment se WHERE se.indIP = sa.indIP);
 nsalle | nomsalle | nbposte |   indip    
--------+----------+---------+------------
 s22    | Salle 22 |       0 | 130.120.83
 s23    | Salle 23 |       0 | 130.120.83
(2 rows)

SELECT * FROM Logiciel AS l WHERE NOT EXISTS (SELECT t.typeLP FROM Types t WHERE t.typeLP = l.typeLog);
 nlog |   nomlog    |  dateach   | version | typelog | prix 
------+-------------+------------+---------+---------+------
 log8 | DreamWeaver | 2003-09-21 | 2.0     | BeOS    | 1400
(1 row)

Explication : Certaines lignes dans les tables Salle et Logiciel contiennent des entrées dont les références n'existent pas,
c'est-à-dire que, pour la table Salle, il existe des valeurs de indIP qui n'existent pas dans les valeurs indIP de la table Segment et,
pour la table Logiciel, il existe des valeurs de typeLog qui n'existent pas dans les valeurs typeLP de la table Types. 
*/

/*
Question 7

Ordre de création :
    1. Types, Segment
    2. Salle, Logiciel
    3. Poste
    4. Installer

Ordre de suppression :
    1. Installer
    2. Poste
    3. Salle, Logiciel
    4. Types, Segment

Explication :
    - Types et Segment ne dépendent d'aucune table (i.e. elles ne font référence à rien)
    - Salle fait référence à Segment, donc doit être créée après Segment, sinon on ne peut créer la contrainte de FOREIGN KEY
    - Logiciel fait référence à Types et pas à Salle, donc doit être créée après Types, mais pas nécessairement après Salle
    - Poste fait référence à Segment, Types et Salle, donc doit être créée après ces trois tables
    - Installer fait référence à Postes et Logiciel, donc doit être créée après ces deux tables
    
    - L'ordre de suppression est l'inverse de l'ordre d'insertion car on doit d'abbord supprimer les références à une table avant de supprimer ladite table
*/