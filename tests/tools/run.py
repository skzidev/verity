import os
import subprocess

# Source - https://stackoverflow.com/a/287944
# Posted by joeld, modified by community. See post 'Timeline' for change history
# Retrieved 2025-12-25, License - CC BY-SA 4.0

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

tests = [test for test in os.listdir('./tests/unit') if test.startswith("test_")]

# TODO add e2e test support once compiler can build binaries

print(f"running local testing suite, output > stdout\n")
print(" ===== unit tests ===== \n")

passed = []
failed = []

i = 0

def get_test_name(file_name): return file_name.replace("test_", "").replace(".c", "")

def update_counter(out_of=-1):
	print(f"  - Testing... ({i}/{out_of if out_of >= 0 else len(tests)})", end="\r")

update_counter()

for test in tests:
	execFile = ("./bin/" + test.replace(".c", ""))
	output = subprocess.check_output([execFile])
	if(chr(output[0]) == "["):
		passed.append(test)
	else:
		failed.append(test)

	i += 1
	update_counter()

print("\n")

for test in failed:
	print(f"{bcolors.FAIL}test failure: '{get_test_name(test)}'{bcolors.ENDC}")

print(" ===== e2e tests ===== \n")

print(f"\nran {len(tests)} test(s): {bcolors.OKGREEN}{len(passed)}{bcolors.ENDC} passed,"
	  + f" {bcolors.FAIL}{len(failed)}{bcolors.ENDC} failed ({(len(passed)/len(tests))*100}% passing rate).")