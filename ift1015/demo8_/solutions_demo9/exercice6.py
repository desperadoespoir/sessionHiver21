# Solution

def readFile(path):
    return open(path, 'rb').read().decode('utf-8')

def writeFile(path, texte):
    f = open(path, 'wb')
    f.write(texte.encode('utf-8'))
    f.close()

def justifier(entree, sortie):

    resultat = ''

    for ligne in readFile(entree).split('\n'):
        resultat += ' '*(79-len(ligne)) + ligne + '\n'

    writeFile(sortie, resultat)

justifier('a.txt', 'b.txt')
