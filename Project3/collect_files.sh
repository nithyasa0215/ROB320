#!/bin/bash

# Ensure an input file is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <file_list.txt>"
    exit 1
fi

# Input file containing the list of filenames
file_list="$1"

# Directory to store collected files
submit_dir="submit"

# Wipe out all existing files in the directory
rm -rf "$submit_dir"

# Create the submit directory if it doesn't exist
mkdir -p "$submit_dir"

# Read each filename from the input file
while IFS= read -r filename; do
    # Find the file in subdirectories
    filepath=$(find . -type f -name "$filename" 2>/dev/null | head -n 1)

    # If the file is found, copy it to submit directory
    if [ -n "$filepath" ]; then
        cp "$filepath" "$submit_dir/"
        echo "Copied: $filepath -> $submit_dir/"
    else
        echo "File not found: $filename"
    fi
done < "$file_list"

echo "Collection complete."