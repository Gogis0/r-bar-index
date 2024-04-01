#!/bin/bash

# Input filename
filename="$1"

# Check if filename is provided
if [ -z "$filename" ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# Replace the original extension with .suffixient
output_filename="${filename}.suffixient"

# Start with generating the suffixient set
~/externalSoftware/suffixient/build/suffixient -o "$output_filename" < "$filename"

# Check if the suffixient set is generated
if [ $? -ne 0 ]; then
    echo "Failed to generate suffixient set"
    exit 1
fi

# Next step is to generate the grammar using bigrepair
~/externalSoftware/bigrepair/bigrepair -v "${filename}"
