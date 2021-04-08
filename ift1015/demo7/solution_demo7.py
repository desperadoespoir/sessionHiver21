# Solution
# La fonction unique prend en parametre un tableau et retourne un
# nouveau tableau contenant les elements du tableau en parametre, dans
# le meme ordre, mais ou les valeurs dupliquees ont ete eliminees.

'''
def unique(tab):
 n = 0
 for i in range(len(tab)):
    for j in range(i):
        if tab[j] == tab[i]:
            break  # n'exécute pas le n += 1
    else:
        n += 1  # trouvé une nouvelle valeur
 resultat = [0] * n
 n = 0
 for i in range(len(tab)):
    for j in range(i):
        if tab[j] == tab[i]:
            break  # n'exécute pas le n += 1
    else:
        resultat[n] = tab[i]  # trouvé une nouvelle valeur
        n += 1
 return resultat


def testUnique():
 assert unique([3, 11, 5, 22, 11, 11, 1, 22, 9]) == [3, 11, 5, 22, 1, 9]
 assert unique([]) == []
 assert unique([1, 1, 1, 1]) == [1]
 assert unique([1, 1, 2, 1, 1, 2, 3, 3]) == [1, 2, 3]
 assert unique(['a', 'b']) == ['a', 'b']
 assert unique(['a', 'b', 'a']) == ['a', 'b']


print(unique([3, 11, 5, 22, 11, 11, 1, 22, 9]))

testUnique()


'''

# Version avec la methode append.
# La fonction position prend un tableau et une valeur en parametre.
# La fonction retourne -1 si la valeur n'est pas un element du
# tableau, sinon l'index de la premiere occurrence de la valeur dans
# le tableau est retournee.


def position(tab, val):
    for i in range(len(tab)):
        if tab[i] == val:
          return i  # on a trouvé!
    return -1  # code indiquant échec
# La fonction unique prend en parametre un tableau et retourne un
# nouveau tableau contenant les elements du tableau en parametre, dans
# le meme ordre, mais ou les valeurs dupliquees ont ete eliminees.


def unique(tab):
 resultat = []
 for i in range(len(tab)):
    if position(resultat, tab[i]) == -1:
        resultat.append(tab[i])
 return resultat


def testUnique():
 assert unique([3, 11, 5, 22, 11, 11, 1, 22, 9]) == [3, 11, 5, 22, 1, 9]
 assert unique([]) == []
 assert unique([1, 1, 1, 1]) == [1]
 assert unique([1, 1, 2, 1, 1, 2, 3, 3]) == [1, 2, 3]
 assert unique(['a', 'b']) == ['a', 'b']
 assert unique(['a', 'b', 'a']) == ['a', 'b']


testUnique()


# Solution
 # La fonction position prend un tableau et une valeur en parametre.
 # La fonction retourne -1 si la valeur n'est pas un element du
 # tableau, sinon l'index de la premiere occurrence de la valeur dans
 # le tableau est retournee.


def position(tab, val):
    for i in range(len(tab)):
        if tab[i] == val:
            return i  # on a trouvé!
    return -1  # code indiquant échec
# La fonction intersection prend deux tableaux en parametre et
# retourne un nouveau tableau contenant uniquement les valeurs qui
# sont a la fois dans les deux tableaux. L'ordre des elements dans le
# tableau resultant respecte l'ordre du premier tableau.


def intersection(tab1, tab2):
 resultat = []  # tableau resultant
 for i in range(len(tab1)):  # pour tous les elements de tab1
    elem = tab1[i]
    if (position(tab2, elem) >= 0 and  # est-ce que elem existe dans tab2
         position(resultat, elem) < 0):  # mais pas encore dans resultat?
         resultat.append(elem)  # ajouter au resultat
 return resultat


def testIntersection():
 assert intersection([], []) == []
 assert intersection([1], []) == []
 assert intersection([], [1]) == []
 assert intersection([1], [1]) == [1]
 assert intersection([1], [1, 1]) == [1]
 assert intersection([1, 1], [1]) == [1]
 assert intersection([1], [1, 2, 1]) == [1]
 assert intersection([1, 2, 1], [1]) == [1]

 assert intersection([3, 1, 4, 1, 5], [2, 4, 4, 3]) == [3, 4]


testIntersection()

