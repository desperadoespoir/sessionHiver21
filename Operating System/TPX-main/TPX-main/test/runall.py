import re
import subprocess

with open("./checks.c") as f:
    string = f.read()

matches = re.findall(r"test_.*_\d", string)

dico = {
    "test_strlen": 1/7,
    "test_no_of_lines": 1/7,
    "test_readline": 1/7,
    "test_memcpy": 1/7,
    "test_execute": 2/7,
    "test_parse_line": 1/7
}

perfect_score = {key:0 for (key, _) in dico.items()}
points = perfect_score.copy()

failed = 0
for test_case in matches:
    ret = subprocess.call(f"./tests_build/check_tests {test_case}", shell=True)

    perfect_score[test_case[:-2]] += 1

    if ret == 0:
        points[test_case[:-2]] += 1

total = 0
for key, val in dico.items():
    print(f"{key}: {(points[key] / perfect_score[key])}")
    total += (points[key] / perfect_score[key])*val
print(f"GRADE:{{{total}}}")