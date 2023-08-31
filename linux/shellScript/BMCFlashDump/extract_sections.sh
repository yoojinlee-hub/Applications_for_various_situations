#!/bin/bash

# Input file name
read -p "Enter the input file name: " input_file

# Check if the input file exists
if [ ! -f "$input_file" ]; then
    echo "Input file does not exist"
    exit 1
fi

output_prefix="$input_file"

# Check if the input file size is 64MB
file_size=$(stat -c %s "$input_file")
if [ "$file_size" -ne $((64 * 1024 * 1024)) ]; then
    echo "Input file size is not 64MB"
    exit 1
fi

# Extract u-boot.bin
dd if="$input_file" of="${output_prefix}_u-boot.bin" bs=1 count=$((0x100000))

# Extract kernel.bin
dd if="$input_file" of="${output_prefix}_kernel.bin" bs=1 skip=$((0x100000)) count=$((0x800000 - 0x100000))

# Extract rofs.bin
dd if="$input_file" of="${output_prefix}_rofs.bin" bs=1 skip=$((0x800000)) count=$((0x2400000))

# Extract rwfs.bin
dd if="$input_file" of="${output_prefix}_rwfs.bin" bs=1 skip=$((0x2c00000)) count=$((0x4000000 - 0x2c00000))

