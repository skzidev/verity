"""
	Create a unit test macro: Run this file (`python3 ./tests/gen.py {module_name}`)
	to generate a test file that tests a unit of the inputted module name.
"""

import sys


def generate_test_unit(unit_name):
	file = f"#include <assert.h>\n#include <stdio.h>\n#include \"../../src/{unit_name}/{unit_name}.c\"\n\n"

	def generate_function_def(name, return_type, code):
		return f"{return_type} {name}()\u007b\n\t{code}\n\u007d"
	
	file += generate_function_def("test_" + unit_name, "void", "// Define your unit testing logic here\n") + "\n\n"
	file += generate_function_def("main", "int", f"// Define arguments for your test here\n\n\t// Run your test here\n\ttest_{unit_name}();\n\tprintf(\"[PASS]: Test '{unit_name}' Passed\\n\");\n\treturn 0;")

	return file

def generate_test_e2e(unit_name):
	return "proc main(string[] args) returns int {\n\t// Define your program here\n}"

if(len(sys.argv) < 3):
	raise TypeError("insufficient arguments")

types = ["unit", "e2e", "integration", "golden"]

type = sys.argv[1]
if(type not in types): raise TypeError("invalid test type: '{}'".format(type))
unit = sys.argv[2]

new_file = ""

if(type == 'unit'): new_file = generate_test_unit(unit)
elif(type == "e2e"): new_file = generate_test_e2e(unit)
else: raise TypeError("test type '{}' not supported by this test script (only unit and e2e)".format(type))
with open(f"./tests/{type}/test_{unit}.c", 'w') as f:
	f.write(new_file)
	print(f"successfully generated test '{unit}'")