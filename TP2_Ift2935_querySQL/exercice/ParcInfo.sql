-- Database: TP2_DB

BEGIN TRANSACTION;
create schema ParcInfo;
set search_path to ParcInfo;

CREATE TABLE Segment(
  indIP INTEGER,   -- primary key  
  nomSegment TEXT NOT NULL,
  etage INTEGER,
  --constraint deptpk primary key (indIP)
  PRIMARY KEY (indIP)	
);

CREATE TABLE Salle(
  nSalle INTEGER,   -- primary key
  nomSalle TEXT NOT NULL,
  nbPoste INTEGER,
  indIP  INTEGER,
  PRIMARY KEY (nSalle)	
);
CREATE DOMAIN val_ad as TEXT -- standard SQL
CHECK (CAST ( VALUE AS INTEGER ) >= 0 AND CAST ( VALUE AS INTEGER ) <= 255);

CREATE TABLE Poste(
  nPoste INTEGER,   -- primary key
  nomPoste TEXT NOT NULL,
  indIP	 INTEGER,   -- cle etrangere
  ad  	val_ad NOT NULL ,
  typePoste TEXT,  -- cle etrangere
  nSalle INTEGER,	 -- cle etrangere   
  PRIMARY KEY (nPoste)	
);

CREATE DOMAIN val_prix as REAL -- standard SQL
CHECK (VALUE >=0);

CREATE TABLE Logiciel(
  nLog INTEGER,   -- primary key
  nomLog TEXT,
  dateAch DATE ,
  version 	TEXT,
  typeLog 	TEXT,
  prix val_prix NOT NULL,	
  PRIMARY KEY (nLog)
);

CREATE TABLE Installer(
  numIns SERIAL,  -- primary key
  nPoste INTEGER,  -- cle etrangere
  nLog INTEGER,    -- cle etrangere
  dateIns TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL,
  delai INTEGER,
  PRIMARY KEY (numIns)
);

CREATE TABLE Types(
  typeLP TEXT,   -- primary key  
  nomType TEXT,
  PRIMARY KEY (typeLP)	
);

COMMIT;
