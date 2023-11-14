#!/bin/bash

search_and_replace() {
    local filename=$1
    local oldstr=$2
    local newstr=$3

    while IFS='' read -r line || [[ -n "$line" ]]; do
        echo "$line" | awk -v old="$oldstr" -v new="$newstr" '{gsub(old, new); print}' >> temp_file
    done < "$filename"

    mv temp_file "$filename"
}

search_dir() {
    local dirpath=$1
    local oldstr=$2
    local newstr=$3

    for file in "$dirpath"/*; do
        if [[ -d $file ]]; then
            search_dir "$file" "$oldstr" "$newstr"
        elif [[ -f $file ]]; then
            search_and_replace "$file" "$oldstr" "$newstr"
        elif [[ -L $file ]]; then
            target=$(readlink -f "$file")
            if [[ -f $target ]]; then
                search_and_replace "$target" "$oldstr" "$newstr"
            else
                echo "Target of symbolic link is not a regular file: $file"
            fi
        else
            echo "Unsupported file type: $file"
        fi
    done
}

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 directory oldstr newstr"
    exit 1
fi

dirpath=$1
oldstr=$2
newstr=$3

search_dir "$dirpath" "$oldstr" "$newstr"

