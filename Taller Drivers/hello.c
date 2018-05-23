#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

static dev_t hello_devno;
struct cdev hello_dev;
static struct class *hello_class;

char* ptr;

ssize_t hello_read(struct file *filp, char __user *data, size_t s, loff_t *off){
	printk(KERN_ALERT "Estas leyendo\n");
	copy_to_user(data, ptr, 9);
	return 9;
}

ssize_t hello_write(struct file *filp, const char __user *data, size_t s, loff_t *off){
	printk(KERN_ALERT "Estas escribiendo\n");
	char c;
	copy_from_user(&c,data,1);
	if(c == 'N'){
		if(ptr[4] == '0'){
			printk(KERN_ALERT "No se puede\n");
		}else{
			ptr[4] =ptr[4] -1;
		}
	}

	if(c == 'S'){
		if(ptr[4] == '9'){
			printk(KERN_ALERT "No se puede \n");
		}else{
			ptr[4] = ptr[4] +1;
		}
	}


	if(c == 'W'){
		if(ptr[2] == '0'){
			printk(KERN_ALERT "No se puede\n");
		}else{
			ptr[2] = ptr[2] -1;
		}
	}

	if(c == 'E'){
		if(ptr[2] == '9'){
			printk(KERN_ALERT "No se puede\n");
		}else{
			ptr[2] = ptr[2] +1;
		}
	}
	return s;

}

static struct file_operations mis_operaciones = {
	.owner = THIS_MODULE,
	.read = hello_read,
	.write = hello_write,
};

static int __init hello_init(void){
	printk(KERN_ALERT "Hola, Sistemas Operativos!\n");
	ptr = (char*)vmalloc(9);
	ptr[0] = 'A';
	ptr[1] = ' ';
	ptr[2] = '0';
	ptr[3] = '-';
	ptr[4] = '0';
	ptr[5] = ':';
	ptr[6] = ' ';
	ptr[7] = '_';
	ptr[8] = '\n';



	
	alloc_chrdev_region(&hello_devno, 0, 1, "disp");
	
	cdev_init(&hello_dev, &mis_operaciones);	
	cdev_add(&hello_dev, hello_devno, 1);

	hello_class = class_create(THIS_MODULE, "hello");
	device_create(hello_class, NULL, hello_devno, NULL, "hello");

	return 0;

}


static void __exit hello_exit(void){
	unregister_chrdev_region(&hello_devno, 1);
	cdev_del(&hello_dev);
	vfree(ptr);
	device_destroy(hello_class, hello_devno);
	class_destroy(hello_class);
	printk(KERN_ALERT "Adios, mundo cruel...\n");

}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SFDAJKLASDFJKL");
MODULE_DESCRIPTION("SARASA");
