#!/bin/bash

# Circular Dependency Checker for C++ Projects
# Usage: ./check_deps.sh [directory]

# Set the starting directory (default to current directory)
START_DIR="${1:-.}"

echo "🔍 Checking for circular dependencies in: $START_DIR"
echo "=================================================="

# Function to extract included local headers
extract_includes() {
    local file="$1"
    # Extract #include statements for local headers (not system headers)
    grep -n '#include' "$file" | grep -E '\.(hpp|h)"' | sed 's/.*#include[[:space:]]*["<]\([^">]*\)[">].*/\1/'
}

# Function to build dependency map
build_dependency_map() {
    declare -A dep_map
    
    echo "📋 Building dependency map..."
    echo
    
    # Find all header files recursively
    find "$START_DIR" -name "*.hpp" -o -name "*.h" | while read -r file; do
        # Get relative path for cleaner output
        rel_file=$(realpath --relative-to="$START_DIR" "$file")
        
        echo "📁 File: $rel_file"
        
        # Get all includes for this file
        includes=$(extract_includes "$file")
        
        if [ -n "$includes" ]; then
            echo "   Dependencies:"
            echo "$includes" | while read -r include; do
                if [ -n "$include" ]; then
                    echo "   → $include"
                fi
            done
        else
            echo "   → No local dependencies"
        fi
        echo
    done
}

# Function to detect potential circular dependencies
detect_circular_deps() {
    echo
    echo "🔄 Potential Circular Dependencies:"
    echo "=================================="
    
    # Create temporary files for analysis
    temp_deps=$(mktemp)
    temp_pairs=$(mktemp)
    
    # Build a list of file->dependency pairs
    find "$START_DIR" -name "*.hpp" -o -name "*.h" | while read -r file; do
        basename_file=$(basename "$file")
        extract_includes "$file" | while read -r include; do
            if [ -n "$include" ]; then
                include_basename=$(basename "$include")
                echo "$basename_file -> $include_basename" >> "$temp_deps"
            fi
        done
    done
    
    # Look for mutual dependencies (A->B and B->A)
    if [ -s "$temp_deps" ]; then
        sort "$temp_deps" | uniq > "$temp_pairs"
        
        found_circular=false
        while read -r line; do
            if [ -n "$line" ]; then
                # Extract A and B from "A -> B"
                file_a=$(echo "$line" | cut -d' ' -f1)
                file_b=$(echo "$line" | cut -d' ' -f3)
                
                # Check if reverse dependency exists
                reverse_dep="$file_b -> $file_a"
                if grep -q "^$reverse_dep$" "$temp_pairs"; then
                    echo "⚠️  CIRCULAR: $file_a ↔ $file_b"
                    found_circular=true
                fi
            fi
        done < "$temp_pairs"
        
        if [ "$found_circular" = false ]; then
            echo "✅ No obvious circular dependencies detected!"
        fi
    else
        echo "✅ No dependencies found to analyze."
    fi
    
    # Cleanup
    rm -f "$temp_deps" "$temp_pairs"
}

# Function to show statistics
show_stats() {
    echo
    echo "📊 Project Statistics:"
    echo "====================="
    
    header_count=$(find "$START_DIR" -name "*.hpp" -o -name "*.h" | wc -l)
    source_count=$(find "$START_DIR" -name "*.cpp" -o -name "*.c" | wc -l)
    
    echo "Header files (.h/.hpp): $header_count"
    echo "Source files (.c/.cpp): $source_count"
    
    # Count total includes
    include_count=$(find "$START_DIR" -name "*.hpp" -o -name "*.h" -exec grep -l '#include.*\.(hpp|h)"' {} \; | wc -l)
    echo "Files with local includes: $include_count"
}

# Main execution
main() {
    if [ ! -d "$START_DIR" ]; then
        echo "❌ Error: Directory '$START_DIR' does not exist."
        exit 1
    fi
    
    build_dependency_map
    detect_circular_deps
    show_stats
    
    echo
    echo "💡 Tips to fix circular dependencies:"
    echo "   • Use forward declarations instead of #include when possible"
    echo "   • Move implementations to .cpp files"
    echo "   • Consider dependency injection or interface segregation"
    echo "   • Break large classes into smaller, more focused ones"
}

# Run the main function
main