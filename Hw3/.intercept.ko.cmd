cmd_/home/student/Hw/Hw3/intercept.ko := ld -r -m elf_x86_64 -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /home/student/Hw/Hw3/intercept.ko /home/student/Hw/Hw3/intercept.o /home/student/Hw/Hw3/intercept.mod.o ;  true