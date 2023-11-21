#!/bin/bash

# Allocate memory for the LOFS image
sudo truncate -s 50M lofs.img

# Setup a loop device for the LOFS image
sudo losetup -fP lofs.img

# Create a Loop File System (LOFS) with ext4
sudo mkfs.ext4 /dev/loop0

# Compile the C program ex1.c
gcc ex1.c -o ex1

# Create a new directory for the LOFS mount point
mkdir ./lofsdisk

# Copy the compiled program to the LOFS directory
cp ex1 ./lofsdisk/ex1

# Create basic directories within the LOFS
sudo mkdir -p ./lofsdisk/{bin,lib,lib64}

# Populate files in the LOFS
echo "Dmitrii" | sudo tee ./lofsdisk/file1
echo "Nekrasov" | sudo tee ./lofsdisk/file2

# Function to get shared libraries of a binary
get_libs() {
    sudo ldd "$1" | awk '{print $3}'
}

# Get shared libraries and copy them to the LOFS
sudo cp /bin/bash ./lofsdisk/bin/
sudo cp $(get_libs /bin/bash) ./lofsdisk/lib/
sudo cp /bin/cat ./lofsdisk/bin/
sudo cp $(get_libs /bin/cat) ./lofsdisk/lib/
sudo cp /bin/echo ./lofsdisk/bin/
sudo cp $(get_libs /bin/echo) ./lofsdisk/lib/
sudo cp /bin/ls ./lofsdisk/bin/
sudo cp $(get_libs /bin/ls) ./lofsdisk/lib/

# Change the root directory to the LOFS and run the program ex1
sudo chroot lofsdisk /ex1 >> /ex1.txt

# Additional actions within chroot can be performed here if needed
# Example: sudo chroot lofsdisk /bin/bash -c "echo 'Hello from chroot' > /hello.txt"

# Unmount the loop device and clean up
sudo umount ./lofsdisk
sudo losetup -d /dev/loop0
