# Solution


def readFile(path):
    return open(path, 'rb').read().decode('utf-8')


def writeFile(path, texte):
    f = open(path, 'wb')
    f.write(texte.encode('utf-8'))
    f.close()


writeFile('b.txt', readFile('a.txt').upper())
