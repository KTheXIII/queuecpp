#!/usr/bin/env sh

set -e

out_name=$1
out_name="${out_name%.*}"

cpp_version=-std=c++17
warnings="-Wall -Wextra -Wpedantic -Werror"
input_file=$1
target_dir=bin

mkdir -p $target_dir

echo "Building ${out_name}..."

c++ $cpp_version $warnings $input_file -o $target_dir/$out_name

green=$"\u001b[32m"
reset=$"\u001b[0m"

echo "Done!\n"

