#!/bin/bash

# Input filename
filename="$1"

# Log file
logfile="processing.log"

# Clear the log file
> "$logfile"

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
build/_deps/suffixient-build/suffixient -o "$output_filename" < "$filename" >> "$logfile" 2>&1

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
build/thirdparty/bigrepair/bigrepair -v "${filename}" >> "$logfile" 2>&1

# Check if the grammar is generated
if [ $? -ne 0 ]; then
    log "Failed to generate grammar for file: $filename"
    echo "Failed to generate grammar. Check '$logfile' for details."
    exit 1
else
    log "Grammar generated successfully for file: $filename"
    echo "Grammar generated successfully for file: $filename"
fi

# Build ShapedSlp
log "Building ShapedSlp for file: $filename"
build/_deps/shapedslp-build/SlpEncBuild -i "${filename}" -f Bigrepair -e PlainSlp_32Fblc -o "${filename}.PlainSlp_32Fblc" >> "$logfile" 2>&1

# Check if the ShapedSlp is built
if [ $? -ne 0 ]; then
    log "Failed to build ShapedSlp for file: $filename"
    echo "Failed to build ShapedSlp. Check '$logfile' for details."
    exit 1
else
    log "ShapedSlp built successfully for file: $filename"
    echo "ShapedSlp built successfully for file: $filename"
fi

# Build the rho-index
log "Building the rho-index for file: $filename"
build/build_index "${filename}" >> "$logfile" 2>&1

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
