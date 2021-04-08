# Solution


def readFile(path):
    return open(path, 'rb').read().decode('utf-8')


def writeFile(path, texte):
    f = open(path, 'wb')
    f.write(texte.encode('utf-8'))
    f.close()


def decouperEnLignes(contenu):
    lignes = contenu.split('\r\n')
    if len(lignes) == 1:
        lignes = contenu.split('\r')
        if len(lignes) == 1:
            lignes = contenu.split('\n')
    if lignes[-1] == '':
        lignes.pop()
    return lignes


def lireCSV(path):
    lignes = decouperEnLignes(readFile(path))
    resultat = []
    for ligne in lignes:
        resultat.append(ligne.split(','))
    return resultat


def ecrireCSV(path, matrice):
    contenu = ''
    for rangee in matrice:
        contenu += ','.join(rangee) + '\n'
    writeFile(path, contenu)


def positionNom(bottin, nom):
    for i in range(len(bottin)):
        if bottin[i][0] == nom:
            return i
    return -1


def positionNumero(bottin, numero):
    for i in range(len(bottin)):
        if bottin[i][1] == numero:
            return i
    return -1


def gererBottin():

    bottin = lireCSV('bottin.csv')
    msg = ''

    while True:

        cmd = input(msg + 'Entrez un nom ou numéro ou fin: ')

        if cmd.upper() == 'FIN' or cmd == '':
            break

        if cmd[0].isdecimal():

            # entré un numéro de téléphone... le chercher
            pos = positionNumero(bottin, cmd)
            if pos < 0:
                nom = input('Numéro pas trouvé... ' +
                            'entrez le nom à ajouter: ')
                bottin.append([nom, cmd])
                pos = len(bottin)-1
        else:

            # entré un nom... le chercher
            pos = positionNom(bottin, cmd)
            if pos < 0:
                numero = input('Nom pas trouvé... ' +
                               'entrez le numéro à ajouter: ')
                bottin.append([cmd, numero])
                pos = len(bottin)-1

        # afficher le nom et numéro de téléphone
        msg = bottin[pos][0] + ': ' + bottin[pos][1] + '\n'

    ecrireCSV('bottin.csv', bottin)


gererBottin()
