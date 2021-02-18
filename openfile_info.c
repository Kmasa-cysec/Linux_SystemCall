#include <linux/net.h>
#include <net/sock.h>
#include <uapi/linux/in.h>
#include <linux/fdtable.h>
#include <linux/fs.h>
#include <net/inet_sock.h>


//SYSCALL_DEFINE0(openfile_info)
asmlinkage long sys_openfile_info(unsigned long *buff_value,unsigned char *buff_name, unsigned int op_file_num)
{
	static struct task_struct *tmp;
	tmp = current;
	struct files_struct *fs;
	fs = tmp->files;
	struct file *file;
	unsigned int word_maxcount = 20; //max_filename_len
	unsigned long inode_num[op_file_num];
	unsigned char file_name[op_file_num][word_maxcount];
	struct inode *ino;
	unsigned int count = 0;
	unsigned int count_sub;
	unsigned int filename_len = 0;
	
	for (count = 3; count < op_file_num + 3; count++){
		file = fs->fdt->fd[count];
		if(!file){
			return -ENOENT;
		}
		filename_len = file->f_path.dentry->d_name.len; 
		count_sub = 0;
		//get_filename
		for(count_sub = 0; count_sub < filename_len; count_sub++){
			file_name[count - 3][count_sub] = *(file->f_path.dentry->d_iname + count_sub);
		}
		file_name[count - 3][filename_len] = '\0'; //insert_null
		ino = file->f_path.dentry->d_inode;
		inode_num[count - 3] = ino->i_ino; //get_inodenum
	}	
	copy_to_user(buff_value, inode_num, op_file_num*sizeof(unsigned long));
	copy_to_user(buff_name, file_name, op_file_num*word_maxcount*sizeof(unsigned char));
	
	return 1;
}