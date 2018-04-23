#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>

volatile u_int64_t freelock;
int share_data = 0;

#define trylock(plock) (*(plock)==0 && __sync_bool_compare_and_swap((plock), 0, 1))
#define unlock(plock) *(plock) = 0

void *testthr(void *pinvok)
{
	for(int i=0; i<20; ++i)
	{
		if (!trylock(&freelock))
		{
			printf("thr getlock err.\n");
			continue;
		}
		share_data = 66;
		printf("thr getlock %d\n", share_data);
		printf("thr leve lock.\n");
		unlock(&freelock);
	}
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, testthr, NULL);
	 for(int i=0; i<20; ++i)
    {
        if (!trylock(&freelock))
        {   
            printf("main getlock err.\n");
            continue;
        }   
        share_data = 88;
        printf("main getlock %d\n", share_data);
        printf("main leve lock.\n");
        unlock(&freelock);
    }
	pthread_join(tid, NULL);
	return 0;
}
