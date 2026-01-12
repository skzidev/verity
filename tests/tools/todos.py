import subprocess
from sys import argv


class bcolors:
    HEADER = "\033[95m"
    OKBLUE = "\033[94m"
    OKCYAN = "\033[96m"
    OKGREEN = "\033[92m"
    WARNING = "\033[93m"
    FAIL = "\033[91m"
    ENDC = "\033[0m"
    BOLD = "\033[1m"
    UNDERLINE = "\033[4m"


def getTodos(content, fname):
    todoLines = []
    lines = content.split("\n")
    lineNumber = 1
    for line in lines:
        if "TODO" in line:
            todoLines.append(
                {
                    "text": line.strip().replace("//", ""),
                    "file": fname,
                    "number": lineNumber,
                }
            )
        lineNumber += 1

    return todoLines


files = subprocess.getoutput("find src \\( -name '*.c' -o -name '*.h' \\)").split("\n")
todos = []
for file in files:
    if "-b" in argv:
        print(f"reading file {file}")
    with open(file) as f:
        todos += getTodos(f.read(), f.name)

i = 0
for todo in todos:
    print(
        todo["text"]
        + bcolors.BOLD
        + f" ({todo['file']}:{todo['number']})"
        + bcolors.ENDC
    )  # print(todo.split("//")[1]) # optionally instead
    i += 1
