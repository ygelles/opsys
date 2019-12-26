#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/linkage.h>
#include <linux/kallsyms.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("YUVALGELLES");

// TODO: add command-line arguments

void allow_rw(unsigned long addr);
void disallow_rw(unsigned long addr);

        void** sys_call_table = NULL;
        char program_name_memory[16]={'B','i','l','l',0,0,0,0,0,0,0,0,0,0,0,0};
char* program_name=program_name_memory;
module_param(program_name,charp,0660);//TODO get prog name

asmlinkage long (*original_syscall)(int pid, int sig);

asmlinkage long our_sys_kill(int pid, int sig){
  printk("program name=%s, Bill name=%s, signal=%d, compare=%d",current->comm,program_name,sig,!strcmp(current->comm,program_name));
  if(strcmp(current->comm,program_name) == 0 && sig == 9){
    return  -EPERM;
  }
  return original_syscall(pid,sig);

}

/*
This function updates the entry of the kill system call in the system call table to point to our_syscall. 
*/
void plug_our_syscall(void){
  allow_rw((unsigned long)sys_call_table);
  original_syscall=sys_call_table[62];
  sys_call_table[62]=our_sys_kill;
  disallow_rw((unsigned long)sys_call_table);
}

/*
This function updates the entry of the kill system call in the system call table to point to the original kill system call. 
*/
void unplug_our_syscall(void){
  allow_rw((unsigned long)sys_call_table);
  sys_call_table[62]=original_syscall;
  disallow_rw((unsigned long)sys_call_table);
}

/*
turns on the R/W flag for addr. 
*/
void allow_rw(unsigned long addr){
  unsigned int level;
  pte_t *pte_adrr = lookup_address(addr, &level);

  if (pte_adrr->pte &~ _PAGE_RW) pte_adrr->pte |= _PAGE_RW;

}

/*
turns off the R/W flag for addr.
*/
void disallow_rw(unsigned long addr) {
  unsigned int level;
  pte_t *pte_adrr = lookup_address(addr, &level);

  pte_adrr->pte = pte_adrr->pte &~_PAGE_RW;}


/*
This function is called when loading the module (i.e insmod <module_name>)
*/
int init_module(void) {
  //strcpy(program_name,"Bill");
  sys_call_table = kallsyms_lookup_name("sys_call_table");
  //printk("adress is:%p",sys_call_table);
   plug_our_syscall();
}

void cleanup_module(void) {
  unplug_our_syscall();
}
