#include <linux/module.h>       // Needed by all modules
#include <linux/kernel.h>       // Needed for KERN_INFO

int init_module(void) {
    printk(KERN_INFO "Hello world 1.\n");
    
    // A non 0 return means init_module failed; module can't be loaded.
    return 0;
}

void cleanup_module(void) {
    printk(KERN_INFO "Goodbye world 1.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Hello World Kernel Module");
MODULE_VERSION("0.1");
