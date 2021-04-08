# Bareme:
# - Math : 4
# - inverse de 0 correct: 1
# - test inverser: 3 (1.5 par cas significatif (nombre finissant par 0, nombre
# contenant le meme chiffre, nombre a un seul chiffre, nombre palyndrome comme 235532))
# - Bonnes pratiques: 2

# Nom de l'auteur : Abd-El-Karim Kanj
# Date de création : 17 février 2021
#
# Cette fonction à comme tâche le fait d'inverser l'ordre dans lequel les
# chiffres d'un nombre apparaissent. L'unique paramètre de la fonction,nmbr,
# est un entier positif. Le résultat obtenu est aussi un entier positif. Le
# nom le plus adéquat pour cette fonction est inverser. inverser(7631) = 1367

"""
def inverser(nmbr):
    inverse = 0
    while(nmbr > 0):
        rappel = nmbr % 10
        inverse = (inverse * 10) + rappel
        nmbr = nmbr // 10
    return inverse
"""
# Cette fonction permet de vérifier si le code utilisé dans la fonction
# inverser fonctionne lors des cas spéciaux et lorsqu'il est utilisé
# dans des cas normaux.

"""
def testInverser():
    assert inverser(7631) == 1367
    assert inverser(777) == 777
    assert inverser(420) == 24
    assert inverser(96) == 69
    assert inverser(4) == 4

print(inverser(0))
"""

# https://udemontreal-my.sharepoint.com/:x:/g/personal/mahdi_zarour_umontreal_ca/EeZCGEYeoE1CkPUJ7T8DslQBDS1WPgamUepplhMB-25D4A?rtime=E4bBMObh2Eg


def change(n, coins_available, coins_so_far):
	if sum(coins_so_far) == n:
		yield coins_so_far
	elif sum(coins_so_far) > n:
		pass
	elif coins_available == []:
		pass
	else:
		for c in change(n, coins_available[:], coins_so_far+[coins_available[0]]):
			yield c
		for c in change(n, coins_available[1:], coins_so_far):
			yield c



n = 8
coins = [1, 4, 6]
solutions = [s for s in change(n, coins, [])]
for s in solutions:
	print (s)
print ('optimal solution:', min(solutions, key=len))

##print(list(change([1, 4, 6], 8)))  # [(2, 2, 2, 2, 2)]
