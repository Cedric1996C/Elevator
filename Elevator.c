#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<semaphore.h>


typedef struct threadinfo{
	pid_t *pdin;
	pid_t *pdout;
	sem_t sem0;
	sem_t sem1;
	char order[2];
	char state[3];
	int pidn;
}tinfo;	

void thread_fread(tinfo *info);
void thread_fwrite(tinfo *info);
int main(){
	pid_t pid[4];
	int pd[8][2];
	int ret[8];

	for(int i=0;i<8;i++){
                ret[i]=pipe(pd[i]);
                if(ret[i]<0){
                        printf("pipe create error!\n");
			return 0;
		}
	}

	if(pid[0]=fork()==0){

		tinfo info;
		sem_init(&info.sem0,0,1);
		sem_init(&info.sem1,0,0);
		info.pdin=pd[0];
		info.pdout=pd[4];
		info.state[0]='1';
		info.state[1]='1';
		info.pidn=1;
		pthread_t id[2];
		close(pd[0][1]);
		close(pd[4][0]);
		
		pthread_create(&id[0],NULL,(void*)thread_fread,(void*)&info);
		pthread_create(&id[1],NULL,(void*)thread_fwrite,(void*)&info);
		pthread_join(id[0],NULL);
		pthread_join(id[1],NULL);

		exit(0);
	}
	else{

		if(pid[1]=fork()==0){

			tinfo info;
			sem_init(&info.sem0,0,1);
			sem_init(&info.sem1,0,0);
			info.pdin=pd[1];
			info.pdout=pd[5];
			info.state[0]='1';
			info.state[1]='1';
			info.pidn=2;
			pthread_t id[2];
			close(pd[1][1]);
			close(pd[5][0]);

			pthread_create(&id[0],NULL,(void*)thread_fread,(void*)&info);
			pthread_create(&id[1],NULL,(void*)thread_fwrite,(void*)&info);
			pthread_join(id[0],NULL);
			pthread_join(id[1],NULL);

			exit(0);
		}
		else{
			if(pid[2]=fork()==0){

				tinfo info;
				sem_init(&info.sem0,0,1);
				sem_init(&info.sem1,0,0);
				info.pdin=pd[2];
				info.pdout=pd[6];
				info.state[0]='1';
				info.state[1]='1';
				info.pidn=3;
				pthread_t id[2];
				close(pd[2][1]);
				close(pd[6][0]);

				pthread_create(&id[0],NULL,(void*)thread_fread,(void*)&info);
				pthread_create(&id[1],NULL,(void*)thread_fwrite,(void*)&info);
				pthread_join(id[0],NULL);
				pthread_join(id[1],NULL);

				exit(0);
			}
			else{
				if(pid[3]=fork==0){

				}
				else{
					char order[5];
					char state[3]={'1','1'};
		
					for(int i=0;i<4;i++){
						close(pd[i][0]);
						close(pd[i+4][1]);
					}
					while(1){
						for(int i=0;i<3;i++){
							write(pd[i][1],state,strlen(state));	
							read(pd[i+4][0],order+i,1);
						}


/*						write(pd[0][1],state,strlen(state));	
						read(pd[4][0],order,1);
						write(pd[1][1],state,strlen(state));
						read(pd[5][0],order+1,1);
						write(pd[2][1],state,strlen(state));	
						read(pd[6][0],order+2,1);
						//printf("%d\n",i);
*/						printf("order:%s\n",order);
					}
					
					//printf("%s\n",strerror(errno));


				}
			}
		}
	}
}
void thread_fread(tinfo *info){
//	printf("!!!\n");
	tinfo *v=(tinfo*)info;
	while(1){
		
//		printf("1%x\n",v);
		sem_wait(&v->sem0);
		
		int i=read(v->pdin[0],v->state,strlen(v->state));
		
		sem_post(&v->sem1);
	}
}
void thread_fwrite(tinfo *info){
	tinfo *v=(tinfo*)info;
	while(1){
		sem_wait(&v->sem1);
		printf("floor%d operation:",v->pidn);
		v->order[0]=getchar();
		getchar();	
		int i=write(v->pdout[1],v->order,1);
		//printf("%d %d\n",v->pidn,i);
		sem_post(&v->sem0);
	}
}
