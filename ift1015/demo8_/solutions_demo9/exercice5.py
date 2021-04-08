# Solution

def readFile(path):
    return open(path, 'rb').read().decode('utf-8')

def writeFile(path, texte):
    f = open(path, 'wb')
    f.write(texte.encode('utf-8'))
    f.close()

def convertir(texte):
    resultat = ''
    dehors = True  # état de l'automate (en dehors des apostrophes)
    for c in texte:
        if c == "'":
            dehors = not dehors
        elif dehors:
            c = c.upper()
        resultat += c
    return resultat


texte = convertir(readFile('a.txt'))
writeFile('b.txt', texte)
