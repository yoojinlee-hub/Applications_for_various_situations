#!/bin/bash

# Help function to display usage information
usage() {
    echo "Usage: $0 -a <start_bus> -b <end_bus>"
    echo "Scans the I2C buses in the specified range, finds connected devices, and checks if the device is active."
    echo "Options:"
    echo "  -a, --start-bus <number>   Start bus number"
    echo "  -b, --end-bus <number>     End bus number"
    echo "  -h, --help                 Display this help message"
    exit 1
}

# Function to scan I2C buses within the specified range
scan_i2c_buses() {
    for bus in $(seq "$start_bus" "$end_bus"); do
        echo "Scanning I2C bus $bus:"
        i2cdetect -y "$bus"
        local result
        result=$(i2cdetect -y "$bus")
        device_address=$(echo "$result" | grep -o -P '(?<=^)[0-9a-f]{2}:(?:[0-9a-f ]{2}|  )+')
        if [ -n "$device_address" ]; then
            echo "Found devices on bus $bus: $device_address"
            check_device_activation "$bus" "$device_address"
        fi
    done
}

# Function to check if the specified device is active on a particular bus
check_device_activation() {
    local bus="$1"
    local addresses="$2"
    IFS=' ' read -ra addr_array <<< "$addresses"
    for addr in "${addr_array[@]}"; do
        local result
        result=$(i2cdetect -y "$bus" | grep "$addr")
        if [ -n "$result" ]; then
            echo "Device found at address $addr on bus $bus."
            # Here you can add more commands to access the device and check its activity.
            # For instance, you might use `i2cget` or `i2cset` to communicate with the device.
            # i2cget -y "$bus" "$addr" <register_address>
            # i2cset -y "$bus" "$addr" <register_address> <value>
        fi
    done
}

# Parse options
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -a|--start-bus) start_bus="$2"; shift ;;
        -b|--end-bus) end_bus="$2"; shift ;;
        -h|--help) usage ;;
        *) echo "Unknown parameter passed: $1"; usage ;;
    esac
    shift
done

# Check if all necessary parameters are provided
if [ -z "$start_bus" ] || [ -z "$end_bus" ]; then
    echo "Please provide start and end bus numbers."
    usage
fi

# Call the function to scan I2C buses and check device activation
scan_i2c_buses

