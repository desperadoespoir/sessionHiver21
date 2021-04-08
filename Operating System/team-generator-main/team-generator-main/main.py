import csv
import random
from coolname import generate_slug

FILE = "teams.csv"

dico = {}

with open('ohno.csv', "r", encoding="UTF-8") as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    next(csv_reader)
    for row in csv_reader:
        if row[1] in dico and dico[row[1]] != row[3]:
            print("HERETIC ANSWER", dico[row[1]], row[1], row[3])
        dico[row[1]] = row[3]

print("nb_students", len(dico) * 2)

try:
    with open(FILE, "r", encoding="UTF-8") as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        next(csv_reader)
        for row in csv_reader:
            for key in row[:-1]:
                try:
                    dico.pop(key)
                except:
                    for key2, val in dico.items():
                        if val == key:
                            dico.pop(key2)
except Exception as e:
    print(e)
print(dico)

dico_list = [(key, val) for key, val in dico.items()]


def pop_random(lst):
    idx = random.randrange(0, len(lst))
    return lst.pop(idx)


def genslug():
    global slugset
    slug = generate_slug(2)
    while slug in slugset:
        slug = generate_slug(2)
    slugset.add(slug)
    return "tp2-" + slug


makeheader = False
try:
    with open(FILE, "r", encoding="UTF-8") as f:
        if len(f.readlines()) <= 1:
            makeheader = True
except:
    makeheader = True

with open(FILE, "a", encoding="UTF-8") as f:
    if makeheader:
        f.write("First,Second,Third,Fourth,Slug\n")

    slugset = set()
    while True:
        if len(dico_list) == 1:
            f.write(f"{dico_list[0][0]},{dico_list[0][1]},,,{genslug()}")
            exit(0)
        elif len(dico_list) == 0:
            exit(0)

        pair1 = pop_random(dico_list)
        pair2 = pop_random(dico_list)

        f.write(f"{pair1[0]},{pair1[1]},{pair2[0]},{pair2[1]},{genslug()}\n")
