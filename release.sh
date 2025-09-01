#!/bin/bash

set -e  # Exit on any error

echo "Lamp 3-Step Release Builder"
echo "=========================="

# Clean up any previous builds
echo "Cleaning up previous builds..."
rm -rf "../mounts"
rm -rf "../kicad"
rm -rf "../firmware"
rm -f "lamp-3-step-release.zip"

mkdir "../mounts"
mkdir "../kicad"
mkdir "../firmware"
mkdir "../firmware/release"
mkdir "../firmware/debug"


# Render OpenSCAD files
echo "Rendering OpenSCAD files..."
rendered_count=0
for file in Mounts/*.scad; do
    if [[ -f "$file" ]]; then
        filename=$(basename "$file" .scad)
        # Skip files starting with underscore
        if [[ ! "$filename" =~ ^_ ]]; then
            echo "  Rendering $file -> ../mounts/${filename}.stl"
            openscad -o "../mounts/${filename}.stl" "./Mounts/${filename}.scad"
            if [[ $? -eq 0 ]]; then
                rendered_count=$((rendered_count + 1))
                echo "    Success"
            else
                echo "    Failed to render $file"
                exit 1
            fi
        else
            echo "  Skipping $file (starts with underscore)"
        fi
    fi
done

echo "Rendered $rendered_count STL files"

# List rendered files
if [[ $rendered_count -gt 0 ]]; then
    echo "Rendered files:"
    ls -la ../mounts/
fi

# Process KiCad files
echo "Processing KiCad files..."
kicad_project_path="Schematics/Main Board"

if [[ -d "$kicad_project_path" ]]; then
    # Find the .kicad_pro file
    kicad_pro_file=$(find "$kicad_project_path" -name "*.kicad_pro" | head -1)

    if [[ -n "$kicad_pro_file" ]]; then
        project_name=$(basename "$kicad_pro_file" .kicad_pro)
        sch_file="$kicad_project_path/${project_name}.kicad_sch"
        pcb_file="$kicad_project_path/${project_name}.kicad_pcb"

        echo "  Found KiCad project: $project_name"

        # Generate schematic PDF
        if [[ -f "$sch_file" ]]; then
            echo "  Generating schematic PDF..."
            kicad-cli sch export pdf "$sch_file" -o "../kicad/${project_name}-schematic.pdf"
            echo "    Schematic PDF created"
        else
            echo "    Warning: Schematic file not found: $sch_file"
        fi

        # Generate gerber files
        if [[ -f "$pcb_file" ]]; then
            echo "  Generating gerber files..."
            mkdir -p "../kicad/gerbers"
            kicad-cli pcb export gerbers "$pcb_file" -o "../kicad/gerbers/" --layers "F.Cu,B.Cu,F.Paste,B.Paste,F.Silkscreen,B.Silkscreen,F.Mask,B.Mask,Edge.Cuts"
            kicad-cli pcb export drill "$pcb_file" -o "../kicad/gerbers/"
            echo "    Gerber files created"
        else
            echo "    Warning: PCB file not found: $pcb_file"
        fi
    else
        echo "    Warning: No .kicad_pro file found in $kicad_project_path"
    fi
else
    echo "    Warning: KiCad project directory not found: $kicad_project_path"
fi

echo "Processed KiCad files"

# List processed files
echo "Processed files:"
if [[ -d "../kicad" ]]; then
    ls -la ../kicad/
    if [[ -d "../kicad/gerbers" ]]; then
        echo "Gerber files:"
        ls -la ../kicad/gerbers/
    fi
fi

# Process firmware builds
echo "Processing firmware builds..."
firmware_project_path="Code/v_0.2"
delay_header_path="$firmware_project_path/include/delay.h"

if [[ -d "$firmware_project_path" ]]; then
    echo "  Found firmware project: $firmware_project_path"

    # Backup the original delay.h file
    if [[ -f "$delay_header_path" ]]; then
        echo "  Backing up delay.h..."
        cp "$delay_header_path" "$delay_header_path.bak"

        # Build firmware with different delay values
        for delay in $(seq 100 100 5000); do
            echo "  Building firmware with delay: ${delay}ms"

            # Update delay.h with new value
            cat > "$delay_header_path" << EOF

#ifndef DELAY_H
#define DELAY_H

#define DELAY $delay

#endif //DELAY_H
EOF

            # Change to firmware project directory
            cd "$firmware_project_path"

            # Build debug version
            echo "    Building debug version..."
            platformio run --environment debug
            if [[ -f ".pio/build/debug/firmware.bin" ]]; then
                cp ".pio/build/debug/firmware.bin" "../../../firmware/debug/debug_${delay}.bin"
                echo "      Debug build created: debug_${delay}.bin"
            elif [[ -f ".pio/build/debug/firmware.hex" ]]; then
                cp ".pio/build/debug/firmware.hex" "../../../firmware/debug/debug_${delay}.hex"
                echo "      Debug build created: debug_${delay}.hex"
            else
                echo "      Warning: Debug build output not found"
            fi

            # Build release version
            echo "    Building release version..."
            platformio run --environment release
            if [[ -f ".pio/build/release/firmware.bin" ]]; then
                cp ".pio/build/release/firmware.bin" "../../../firmware/release/release_${delay}.bin"
                echo "      Release build created: release_${delay}.bin"
            elif [[ -f ".pio/build/release/firmware.hex" ]]; then
                cp ".pio/build/release/firmware.hex" "../../../firmware/release/release_${delay}.hex"
                echo "      Release build created: release_${delay}.hex"
            else
                echo "      Warning: Release build output not found"
            fi

            # Return to root directory
            cd - > /dev/null
        done

        # Restore the original delay.h file
        echo "  Restoring original delay.h..."
        mv "$delay_header_path.bak" "$delay_header_path"

    else
        echo "    Warning: delay.h not found at: $delay_header_path"
    fi
else
    echo "    Warning: Firmware project directory not found: $firmware_project_path"
fi

echo "Firmware builds completed"

# List built firmware files
echo "Built firmware files:"
if [[ -d "../firmware/debug" ]]; then
    echo "Debug builds:"
    ls -la ../firmware/debug/
fi
if [[ -d "../firmware/release" ]]; then
    echo "Release builds:"
    ls -la ../firmware/release/
fi

# Create the final zip
echo "Creating release zip..."
current_dir=$(basename "$PWD")
cd ..
zip -r lamp-3-step-release.zip "$current_dir" "mounts" "kicad" "firmware" -x "*.git*" "*.github*" "*.DS_Store" "*/.*" "*/.idea*"
mv lamp-3-step-release.zip "$current_dir/"
cd "$current_dir"

rm -rf "../mounts"
rm -rf "../kicad"
rm -rf "../firmware"

# Verify the zip
echo "Verifying release zip..."
if [[ -f "lamp-3-step-release.zip" ]]; then
    echo "Release zip created successfully!"
    ls -lh lamp-3-step-release.zip
    echo "Build complete! Your release zip is ready: lamp-3-step-release.zip"
else
    echo "Failed to create release zip"
    exit 1
fi

echo ""
echo "Release build completed successfully!"
echo "File: lamp-3-step-release.zip"
echo "Location: $(pwd)/lamp-3-step-release.zip"