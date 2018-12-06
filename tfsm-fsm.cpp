// tfsm-fsm.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	
	if(argc != 3) 
	              {
		          puts("\nwrong number of args...press...");
				  getch();
				  return -1;
	              }
	
	FILE *f1 = fopen(argv[1],"rt");				//f1 - файл с временным автоматом
	FILE *f2 = fopen(argv[2],"wt");				//f2 - файл с асбтракцией
	// FILE *f3 = fopen(argv[3],"wt");				//f3 - файл для вывода максимального значения таймаута
	
	/*
	FILE *f1 = fopen("example.txt","rt");
	FILE *f2 = fopen("tf.txt","wt");
	FILE *f3 = fopen("m.txt","wt");
	*/
    fseek(f1,2,SEEK_CUR);
	int flag;
	fscanf(f1,"%d",&flag);
	//printf("flag = %d\n",flag);
	if(flag != 2) 
				  {
		          puts("\nwrong type of FSM...");
				  return -1;
	              }
	fseek(f1,3,SEEK_CUR);
	int s_num;
	fscanf(f1,"%d",&s_num);
	//printf("s_num = %d\n",s_num);
	fseek(f1,3,SEEK_CUR);
	int i_num;
	fscanf(f1,"%d",&i_num);
	//printf("i_num = %d\n",i_num);
	fseek(f1,3,SEEK_CUR);
	int o_num;
	fscanf(f1,"%d",&o_num);
	//printf("o_num = %d\n",o_num);
	fseek(f1,4,SEEK_CUR);
	int s0;
	fscanf(f1,"%d",&s0);
	//printf("s0 = %d\n",s0);
	fseek(f1,3,SEEK_CUR);
	int p_num;
	fscanf(f1,"%d",&p_num);
	//printf("p_num = %d\n",p_num);				//считали все параметры автомата
	int i;
	int **t;//transition array
	t = new int *[p_num];
	for(i = 0; i < p_num;i++)
		t[i] = new int[4];
	int **d;//delay array
	d = new int *[s_num];
	for(i = 0; i < s_num;i++)
		d[i] = new int[3];					//выделили память под матрицу переходов (4ки) и матрицу таймаутов (3ки)
	int tmp;
	for(i = 0;i < p_num;i++)				//считываем таблицу (матрицу) переходов
	{
		fscanf(f1,"%d",&t[i][0]);
		fscanf(f1,"%d",&t[i][1]);
		fscanf(f1,"%d",&t[i][2]);
		fscanf(f1,"%d",&t[i][3]);
		fscanf(f1,"%d",&tmp);
	}
	for(i = 0;i < s_num;i++)				//считываем таблицу таймаутов
	{
		fscanf(f1,"%d",&d[i][0]);
		fscanf(f1,"%d",&d[i][1]);
		if(d[i][1] == -1) d[i][1] = 2147483647;
		fscanf(f1,"%d",&d[i][2]);
	}

    
 //    puts("\nt...\n");
	// for(i = 0;i < p_num;i++)				//печатаем таблицу переходов
	// {
	// 	printf("%d ",t[i][0]);
	// 	printf("%d ",t[i][1]);
	// 	printf("%d ",t[i][2]);
	// 	printf("%d\n",t[i][3]);
	// }
 //    puts("d...\n");
	// for(i = 0;i < s_num;i++)				//печатаем таблицу таймаутов
	// {
	// 	printf("%d ",d[i][0]);
	// 	printf("%d ",d[i][1]);
	// 	printf("%d\n",d[i][2]);
	// }
    
	//find m
	unsigned int m = 0;						//m - maximal delay
	for(i = 0;i < s_num;i++)
	{
		if((d[i][1] > m) && (d[i][1] != 2147483647)) m = d[i][1];
	}
	// fprintf(f3,"m = %d\n",m);
	// fclose(f3);
	int i_num_new = i_num * (m + 1);	//количество входных символов после преобразования
//	int p_num_new = i_num_new * s_num;	//количество переходов после преобразования (для полностью определенного автомата!) 
	int p_num_new = p_num * (m + 1);
	int **t_new;						//transition array
	t_new = new int *[p_num_new];
	for(i = 0; i < p_num_new;i++)
		t_new[i] = new int[4];
	int old_state_pointer = 0;
	//printf ("\n\n");
	for(i = 0;i < p_num_new;)
	{
		// t_new[i][0] = i / i_num_new;
		// t_new[i][1] = i % i_num_new;		//не для каждого входного символа будет переход!
		for (int k = 0; k < (m + 1); k++){
			t_new[i][0] = t[old_state_pointer][0];
			t_new[i][1] = t[old_state_pointer][1] * (m + 1) + k;	//старый невременной вход преобразуется в новый временной
			//printf ("%d %d\n", t_new[i][0], t_new[i][1]);
			i++;
		}
		old_state_pointer++;
	}
	int d_curr, in, s, time;
	for(i = 0;i < p_num_new;i++)
	{
		s = t_new[i][0];					//s - текущее состояние
		d_curr = d[s][1];					//d_curr - таймаут в текущем состоянии
		time = t_new[i][1] % (m + 1);		//time - такт времени, в который подается невременной символ (задержка подачи)
		in = t_new[i][1] / (m + 1);			//in - невременной символ
		while(time >= d_curr)				//добраться до состояния в которое автомат перейдет по таймаутам (если пришли в состояние в котором 
											//задержка подачи входа меньше чем таймаут, то это конечное состояние перехода)
		{
			s = d[s][2];
			time = time - d_curr;
			d_curr = d[s][1];
		}

		for (int ii = 0; ii < p_num; ii++)
		{
			if (t[ii][0] == s && t[ii][1] == in) 
			{
				t_new[i][2] = t[ii][2];
				t_new[i][3] = t[ii][3];
				break;
			}
		}

		// t_new[i][2] = t[s * i_num + in][2];
		// t_new[i][3] = t[s * i_num + in][3];
	}
	//t_new -> to file3
	fprintf(f2,"F 0\n");
	fprintf(f2,"s %d\n",s_num);
	fprintf(f2,"i %d\n",i_num_new);
	fprintf(f2,"o %d\n",o_num);
	fprintf(f2,"n0 %d\n",s0);
	fprintf(f2,"p %d\n",p_num_new);
	for(i = 0;i < p_num_new;i++)
	{
		fprintf(f2,"%d ",t_new[i][0]);
		fprintf(f2,"%d ",t_new[i][1]);
		fprintf(f2,"%d ",t_new[i][2]);
		fprintf(f2,"%d\n",t_new[i][3]);
	}
	for(i = 0;i < p_num;i++)
		delete []t[i];
	delete []t;
	for(i = 0;i < p_num_new;i++)
		delete []t_new[i];
	delete []t_new;
	for(i = 0;i < s_num;i++)
		delete []d[i];
	delete []d;
	fclose(f1);
	fclose(f2);
	//getch();
	return 0;
}

