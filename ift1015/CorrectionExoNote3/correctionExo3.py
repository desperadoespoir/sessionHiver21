'''
2
Justin Labelle_15418305_assignsubmission_file_
Nima Fallahi_15418404_assignsubmission_file_
Peilin Wang_15418444_assignsubmission_file_
Wassim Mahieddine_15418310_assignsubmission_file_


2
Mathieu Vallières_15418353_assignsubmission_file_


Abd-El-Karim Kanj_15418377_assignsubmission_file_
Alexander Stanar_15418314_assignsubmission_file_
Antony Junior Diaz Salazar_15418408_assignsubmission_file_
Basma-Djamila Aoued_15418368_assignsubmission_file_
Beryl Jane Santos_15418283_assignsubmission_file_
Bilal Tabbara_15418417_assignsubmission_file_
Diego Rodriguez_15418398_assignsubmission_file_
Eloïse Beaupré_15418352_assignsubmission_file_
Eran Troshani_15418329_assignsubmission_file_
Gabriel Hazan_15418383_assignsubmission_file_
Grace Lee_15418442_assignsubmission_file_
Jenny Diep_15418392_assignsubmission_file_
Khaoula Benzaarit_15418343_assignsubmission_file_
Laurent Bouchard_15418460_assignsubmission_file_
Lisa Puran_15418420_assignsubmission_file_
Maxence Puech_15418359_assignsubmission_file_
Mohamed Amine Haddad_15418446_assignsubmission_file_
Naserdin Kahia_15418278_assignsubmission_file_
Nicholas Gebran_15418445_assignsubmission_file_
Nicolas Lesage_15418311_assignsubmission_file_
Raphael Osvaldo Gonzalez Carvajal_15418425_assignsubmission_fil


'''


# Exercice 3
# Auteur : Noémie Arbour
# Date de création : 5 février 2021

# Ce programme calcul l'approximation de pi par la somme de termes. Il imprime
# le nombre de termes à utiliser pour une justesse donnée (3.14, 3.141, 3.1415)

nombTermes = int(1)  # Nombre (entiers) de termes commençant à 1
somme = float(0)     # Somme initiale vide

nombTermes1 = int(0)  # Variable vide Nombre de termes pour inconnu 1 (3.14...)
# Variable vide Nombre de termes pour inconnu 2 (3.141...)
nombTermes2 = int(0)
# Variable vide Nombre de termes pour inconnu 3 (3.1415...)
nombTermes3 = int(0)

# Condition = tant que la dernière valeur (nombTermes3) n'a pas été affectée :
while nombTermes3 == 0:

    # Ajout du prochain terme à la somme
    somme += ((-1)**(nombTermes+1))*(4/(nombTermes*2-1))

    # Entre dans le if si la somme commence par 3.14...
    if nombTermes1 == 0 and somme < 3.15 and somme >= 3.14:
        # Affectation du nombre de termes nécessaires pour obtenir 3.14...
        nombTermes1 += nombTermes

    # Entre dans le if si la somme commence par 3.141...
    if nombTermes2 == 0 and somme < 3.142 and somme >= 3.141:
        # Affectation du nombre de termes nécessaires pour obtenir 3.141...
        nombTermes2 += nombTermes

    # Entre dans le if si la somme commence par 3.1415...
    if nombTermes3 == 0 and somme < 3.1416 and somme >= 3.1415:
        # Affectation du nombre de termes nécessaires pour obtenir 3.1415...
        nombTermes3 += nombTermes

    # +1 au nombre de termes
    nombTermes += 1


# Imprimer les valeurs trouvées pour les différentes approximations de pi

print('3.14...', nombTermes1)
print('3.141...', nombTermes2)
print('3.1415...', nombTermes3)

'''
3.14... 119
3.141... 1688
3.1415... 10794

'''
