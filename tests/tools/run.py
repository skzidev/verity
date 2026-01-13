import os
import subprocess
import time

# Source - https://stackoverflow.com/a/287944
# Posted by joeld, modified by community. See post 'Timeline' for change history
# Retrieved 2025-12-25, License - CC BY-SA 4.0


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


failed_test_log = ""


def get_test_name(sourceName):
    return sourceName.replace(".c", "").replace("test_", "")


print("running testing suite")
print(" ===== unit tests ===== ")

unit_tests = [test for test in os.listdir("./tests/unit") if test.startswith("test_")]

print(f"\t- found {len(unit_tests)} test(s). attempting all.")

passed_unit_tests = []
failed_unit_tests = []

for test in unit_tests:
    print(f"\t- running test '{get_test_name(test)}'... ", end="")
    start = time.time()
    output = subprocess.run(
        ["./bin/" + test.replace(".c", "")], capture_output=True, text=True, check=False
    )
    end = time.time()
    duration = (end - start) * 1000
    if output.returncode == 0:
        print(bcolors.OKGREEN + "PASS" + bcolors.ENDC)
        passed_unit_tests.append(
            {"name": get_test_name(test), "path": test, "duration": duration}
        )
    else:
        print(bcolors.FAIL + "FAIL" + bcolors.ENDC)
        failed_unit_tests.append(test)
        failed_test_log += f" ===== log for test '{get_test_name(test)}' =====\n"
        failed_test_log += f"\t- test failed with exit code {output.returncode}\n"
        failed_test_log += "-- stdout --\n" + (output.stdout)
        failed_test_log += "-- stderr --\n" + (output.stderr)

print(" ===== e2e tests =====")

e2e_tests = []

print(f"\t- found {len(e2e_tests)} test(s). attempting all.")

passed_e2e_tests = []
failed_e2e_tests = []

for test in e2e_tests:
    print(f"\t- running test '{get_test_name(test)}'... ", end="")
    output = subprocess.check_output(["./bin/" + test.replace(".c", "")])
    if str(output).__contains__("[PASS]"):
        print(bcolors.OKGREEN + "PASS" + bcolors.ENDC)
        passed_unit_tests.append(test)
    else:
        print(bcolors.FAIL + "FAIL" + bcolors.ENDC)
        failed_unit_tests.append(test)
        failed_test_log += f" ===== log for test '{get_test_name(test)}' ====="
        failed_test_log += str(output)

passed = len(passed_e2e_tests) + len(passed_unit_tests)
failed = len(failed_e2e_tests) + len(failed_unit_tests)
print(
    f"{passed + failed} ran, {bcolors.OKGREEN}{passed}{bcolors.ENDC} passed, {bcolors.FAIL}{failed}{bcolors.ENDC} failed ({(passed / (passed + failed)) * 100}% passing rate)."
)

if not os.getenv("GITHUB_STEP_SUMMARY", "default") == "default":
    print("Generating GH action output")
    fname: str = os.getenv("GITHUB_STEP_SUMMARY", "")
    f = open(fname, "w")
    output = "## Unit test results\n\n"
    output += f"**Job ID**: {os.getenv('GITHUB_JOB')}\n\n"
    output += f"**Cause**: {os.getenv('GITHUB_REF_TYPE')}\n\n"
    if failed > 0:
        output += "Overall status: ❌ failed\n"
    else:
        output += "Overall status: ✅ passed\n"
    output += "| Test | Status | Duration |\n"
    output += "|------|--------|---------|\n"
    for test in passed_unit_tests:
        output += f"| {test['name']} | ✅ Passed | {round(test['duration'], 5)}ms |\n"
    for test in failed_unit_tests:
        output += f"| {test['name']} | ❌ failed | {round(test['duration'], 5)}ms |\n"
    output += "\n\nFailure log:\n```"
    output += failed_test_log if failed_test_log else "\nNo failure found\n"
    output += "```"
    f.write(output)

if failed > 0:
    with open("./failed.log", "w") as f:
        f.write(failed_test_log)
    print("the log of failed tests has been made available in './failed.log'")
    exit(1)
else:
    exit(0)
