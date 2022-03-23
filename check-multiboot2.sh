if grub-file --is-x86-multiboot2 dist/x86_64/kernel.bin; then
    echo "Multiboot2 confirmed"
else
    echo "Not using multiboot"
fi
