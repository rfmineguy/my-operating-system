# my-operating-system
An intro to operating systems development

# To build an iso
1. sh start-docker-container.sh
2. make build-x86_64
3. The iso file will be generated under /dist/x86_64/kernel.iso
- Note, the docker image is purely for building the operating system iso file. Nothing else.

# To load OS
1. qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso
