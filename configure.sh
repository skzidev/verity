#!/bin/bash
echo "checking required tools..."

required_tools=("gcc" "ld" "make" "cppcheck" "clang-tidy" "python3")
for tool in "${required_tools[@]}"; do
	echo "checking for $tool"
	if ! command -v "$tool" &> /dev/null; then
		echo "$tool is required but not found. installing with apt".
		sudo apt install $tool
	fi
done

sudo apt-get install llvm-18-dev  # Debian/Ubuntu

echo "checking for makefile..."
if ! [[ -f "./makefile" ]]; then
	echo "makefile does not exist. ensure you cloned the repo correctly".
	exit 1
fi

echo "done configuring"
echo "you can now build the project with '$ make'"
