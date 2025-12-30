"""
	Create a unit test macro: Run this file (`python3 ./tests/gen.py {test_type} {unit_name}`)
	to generate a test file that tests a unit of the inputted module name.
"""

import sys
import os

template_header = "This file is a test template, and should be modified before use in the testing suite"

def generate_unit_test(unit_name):
	header = "// " + template_header + "\n"
	includes = ["<assert.h>", "<stdio.h>", "<stdbool.h>", f"\"../../src/{unit_name}/{unit_name}.h\""]
	includes = ["#include " + include for include in includes]

	body = f"void test_{unit_name}(void){{\n\t// Add your test logic here\n\tassert(true);\n}}\n"
	body += f"int main(){{\n\ttest_{unit_name}();\n\tprintf(\"[PASS]: Test '{unit_name}' Passed\\n\");\n\treturn 0;\n}}"
	
	return header + "\n".join(includes) + "\n\n" + body

def generate_e2e_test(unit_name):
	return f"// {template_header}\nproc main(string[] args) returns int {{\n\t// Write your E2E test here\n\treturn 0;\n}}"

def generate_test(unit_name, unit_type):
	if(unit_type == "unit"):
		return generate_unit_test(unit_name)
	elif(unit_type == "e2e"):
		return generate_e2e_test(unit_name)
	else:
		raise TypeError(f"Unsupported test type: {unit_type}")

if __name__ == "__main__":
	if(len(sys.argv) < 3):
		raise TypeError("Insufficient arguments. usage: python3 ./tests/gen.py <test_type> <unit_name>")
	test_type = sys.argv[1]
	unit_name = sys.argv[2]
	content = generate_test(unit_name, test_type)
	test_path = os.path.join("./tests", f"{test_type}/test_{unit_name}." + ("c" if test_type == "unit" else "vty"))
	with open(test_path, 'w') as f:
		f.write(content)
	print(f"Successfully generated {test_type} test \"{unit_name}\"")