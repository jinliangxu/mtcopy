/************************************************
* mtcopy:
* 	multi-thread file copy 
* Usage:
* 	mthread <source file> <destination file> <thread number>
* 
* Multi-thread study, By Liang, 2014/03/03
************************************************/

#include "mtcopy.h"
using namespace std;


/************************************************
* get_size:
* 	return size of file to copy
************************************************/
off_t get_size(const char *file_name)
{
    struct stat st; 
    memset(&st, 0, sizeof(st));
	/* retrieve file info by file name */
    stat(file_name, &st);
    return st.st_size;
}

/************************************************
* thread_copy:
* 	do thread copy operation
************************************************/
void * thread_copy(void *fi)
{
	off_t file_size, start_pos, total=0;
	ssize_t len;
	char buff[BUFFER_SIZE] = {'\0'};
    file_info f = *((file_info *)fi);    
    int fdin = open(f.from, O_RDONLY);
    int fdout = open(f.to, O_WRONLY|O_CREAT, 0700);

    file_size = get_size(f.from);
	start_pos = file_size*(static_cast<off_t>(f.thread_idx))/(static_cast<off_t>(f.thread_max));
     
    lseek(fdin, start_pos, SEEK_SET);
    lseek(fdout, start_pos, SEEK_SET);
	
    while((len = read(fdin, buff, sizeof(buff))) > 0)
    {
		write(fdout, buff, len); 
		total += static_cast<off_t>(len);
		if(total > (file_size/(static_cast<off_t>(f.thread_max))))
			break;
    }

    close(fdin);
    close(fdout);

}


/************************************************
* main function
************************************************/
int main(int argc, char *argv[])
{
	int i, num=0;
	file_info f;
	vector<pthread_t> pids;
	vector<pthread_t>::iterator iter;
	
	if(argc!=4)
	{
		cerr<<"Usage:"<<endl
			<<"\tmthread <source file> <destination file> <thread number>"<<endl;
		return -1;
	}
	
	num = atoi(argv[3]);

	if(num > MAX_THREAD_NO)
	{
		#ifdef _DEBUG
		cout<<"Maximum number of threads can be only "<<MAX_THREAD_NO<<endl;
		#endif
		num = MAX_THREAD_NO;
	}

	#ifdef _DEBUG
	cout<<"Multithread copy starts..."<<endl;
	cout<<num<<" threads will start..."<<endl;
	#endif
	
	
    creat(argv[2], 0777);
    truncate(argv[2], get_size(argv[1]));

   
	/* pthread_create */
	for(i = 0; i < num; i++)
	{
		pthread_t pid;	
		memset(&f, 0, sizeof(f));
		f.from = argv[1];
		f.to = argv[2];
		f.thread_idx = i;
		f.thread_max = num;
		pthread_create(&pid, NULL, thread_copy, (void*)&f);
		pids.push_back(pid);

		#ifdef _DEBUG
		off_t start_point = get_size(f.from)*(static_cast<off_t>(f.thread_idx))/(static_cast<off_t>(f.thread_max));
		off_t end_point = get_size(f.from)/(static_cast<off_t>(f.thread_max))+start_point;
		cout<<endl
			<<"Size is: "<<get_size(f.from)<<endl
			<<"Index is: "<<i<<endl
			<<"Start point is: "<<start_point<<endl
			<<"End point is: "<<end_point<<endl;
		#endif
    }

	/* pthread_join */
	for(iter = pids.begin(); iter != pids.end(); ++iter)
    {
		pthread_join(*iter, NULL);
    }
	
    #ifdef _DEBUG
	cout<<endl
		<<"Multithread copy ends..."<<endl;
	#endif
	
    return 0;
    
}


