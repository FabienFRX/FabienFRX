#!/bin/bash

# Create output directory
mkdir -p test_results

# Function to run conversion and check for errors
run_conversion() {
    input_file=$1
    output_format=$2
    input_format=$3
    output_file=$4
    
    echo "Converting $input_file to $output_format..."
    
    # Run the conversion and capture output and exit code
    output=$(./mypandoc -i "$input_file" -f "$output_format" -e "$input_format" -o "$output_file" 2>&1)
    exit_code=$?
    
    # Check for failure
    if [[ $exit_code -ne 0 ]] || [[ $output == *"Failed to parse"* ]]; then
        echo -e "\033[0;31mFAILED: $input_file to $output_format\033[0m"
        echo "$output"
        return 1
    else
        echo -e "\033[0;32mSUCCESS: $input_file to $output_format\033[0m"
        return 0
    fi
}

# Test Markdown files
for file in FileTypes/markdown/*.md; do
    filename=$(basename "$file" .md)
    complexity=$filename  # simple, formatting, or complex
    
    run_conversion "$file" "json" "markdown" "test_results/${complexity}_markdown_to_json.json"
    run_conversion "$file" "xml" "markdown" "test_results/${complexity}_markdown_to_xml.xml"
done

# Test JSON files
for file in FileTypes/json/*.json; do
    filename=$(basename "$file" .json)
    complexity=$filename  # simple, formatting, or complex
    
    run_conversion "$file" "markdown" "json" "test_results/${complexity}_json_to_markdown.md"
    run_conversion "$file" "xml" "json" "test_results/${complexity}_json_to_xml.xml"
done

# Test XML files
for file in FileTypes/xml/*.xml; do
    filename=$(basename "$file" .xml)
    complexity=$filename  # simple, formatting, or complex
    
    run_conversion "$file" "markdown" "xml" "test_results/${complexity}_xml_to_markdown.md"
    run_conversion "$file" "json" "xml" "test_results/${complexity}_xml_to_json.json"
done

echo "All tests completed. Results saved in test_results directory."