# Solution

def readFile(path):
    return open(path, 'rb').read().decode('utf-8')

def writeFile(path, texte):
    f = open(path, 'wb')
    f.write(texte.encode('utf-8'))
    f.close()

def sommeFichier(path):

    contenu = readFile(path)
    sansSauts = contenu.replace('\r\n', ' ')  # retirer sauts de ligne
    nombres = sansSauts.split(' ')  # extraire nombres

    somme = 0

    for n in nombres:
        if n != '':  # traiter plusieurs espaces consécutifs
            somme += int(n)

    return somme

print(sommeFichier('donnees.txt'))
