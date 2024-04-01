#!/bin/bash

# Input filename
filename="$1"

# Log file
logfile="processing.log"

# Function to log messages
log() {
    timestamp=$(date +"%Y-%m-%d %T")
    echo "[$timestamp] $1" >> "$logfile"
}

# Check if filename is provided
if [ -z "$filename" ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# Replace the original extension with .suffixient
output_filename="${filename}.suffixient"

# Start with generating the suffixient set
log "Generating suffixient set for file: $filename"
~/externalSoftware/suffixient/build/suffixient -o "$output_filename" < "$filename" >> "$logfile" 2>&1

# Check if the suffixient set is generated
if [ $? -ne 0 ]; then
    log "Failed to generate suffixient set for file: $filename"
    echo "Failed to generate suffixient set. Check '$logfile' for details."
    exit 1
else
    log "Suffixient set generated successfully for file: $filename"
    echo "Suffixient set generated successfully for file: $filename"
fi

# Next step is to generate the grammar using bigrepair
log "Generating grammar using bigrepair for file: $filename"
~/externalSoftware/bigrepair/bigrepair -v "${filename}" >> "$logfile" 2>&1

# Build the rho-index
log "Building the rho-index for file: $filename"
~/externalSoftware/rho-index/build/build_index "${filename}" >> "$logfile" 2>&1

# Check if the build_index command succeeded
if [ $? -ne 0 ]; then
    log "Failed to build the rho-index for file: $filename"
    echo "Failed to build the rho-index. Check '$logfile' for details."
    exit 1
else
    log "Rho-index built successfully for file: $filename"
    echo "Rho-index built successfully for file: $filename"
fi

log "Processing complete for file: $filename"
echo "Processing complete for file: $filename"
