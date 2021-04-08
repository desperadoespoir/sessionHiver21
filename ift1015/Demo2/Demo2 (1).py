

************        
EXO 1 :
**** *******
conversion codeboot : 
     
0b1111     ##  pour le binaire -> decimale
15
0xffff  ##  pour le hexa -> decimal 
65535
0o7656  ##  pour le octal -> decimal 
4014
f = 15 E =14 D =13  C=12 B=11  A=10

conversion de decimal en base 16 : 
passer par  base 2 (binaire) et ->base 16 : regroupement de 16 = 2**4 => 4 bits , 
 rajoute 0 a l fin s'il manque  
ex : 15 -> 1111 -> 0x0f  = f
     16 -> 10000 -> (0001 0000) ->(10)  ( representation avec les lettre s'arrete a 15 
                                         on peut pas dire ici = A)
conversion de decimal en base 8 : 
passer par  base 2 (binaire) et --> base 8 : regroupement de 8 = 2**3 => 3 bits , 
 rajoute 0 a l fin s'il manque  

    Ex: 17 -> 10001  ----010 001 ---> 21
        
************        
EXO 2 :
**** *******

64en  base 16  -> dec 
4 * 16**0 + 6* 16**1

650 en  base 16  -> dec
6 *16**2 + 5x16^1 +  0*16**0 = 1616

0xff : f = 15 E =14 D =13  C=12 B=11  A=10
    15* 16**0 +15* 16**1 =
    
************        
EXO 3 :
**** ******* 

 &  = et logique
|  = ou logique   
^ = ou exclusif   si pareil 0 sinon 1
∼ = complement ( nom à check)  Formule : ∼x = -x-1

3 & 5 -- converion en binaire --->  011 & 101  

Pour X << n  : conersion en binaire et on rajoute n zero a droite 
3<<4 -- conv en binaire --- >  11 ---> 110000 = 48 code boot te donne le resultat

Pour X >> n  : conersion en binaire et on supprime  n bite  a droite 

100>>2   ----- con bin -->  01100100  ---> 011001  = 25  ( 0b1100100 dans codeboot te donne la reponse)


************        
EXO 4 :
**** ******* 
astuce : un nombre paire a le premier bit qui est 0  et un impaire a 1        
si :  n&1  ---> 1 => impaire  sinon si  paire

    
**************    
Exo 5 :
**************
 a = 2 # partie 1
 b = 1 # partie 2
 c = 5 # partie 3

 *******  facteur = 100/( a+b +c) ***
    
 print (a* facteur )
 print (b* facteur )
 print (c* facteur )


**************    
Exo 6 :
**************

 # Ce programme permet de trouver le jour de la semaine
 # pour une certaine date . Le r´e sultat imprim ´e indique
 # le jour de la semaine , avec 1= dimanche , 2= lundi , etc .

annee = 2020 # annee de la date
mois = 9 # mois de la date
quant = 8 # quantieme de la date

# Calculer l’ajustement des annees bisextiles
nbMois = annee *12 + mois - 3 # nombres de mois ´ecoules
nbAns = nbMois // 12 # nombres d’annees ´ecoulees
nb4Ans = nbAns // 4 # nombre periodes de 4 ans ´ecoulees
nb100Ans = nbAns // 100 # nombre periodes de 100 ans ´ecoulees
nb400Ans = nbAns // 400 # nombre periodes de 400 ans ´ecoulees
ajustBisext = nbAns + ( mois +9) //12*4 + nb4Ans - nb100Ans + nb400Ans

# Calculer le jour de la semaine
jour = (23* mois //9 + ajustBisext + quant + 5) % 7 + 1
print(jour)


