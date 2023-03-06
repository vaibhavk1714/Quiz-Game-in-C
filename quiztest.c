#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include<sys/time.h>
#include "loader.h"

#define MAX_LINE_LENGTH 300
typedef struct quest
{
	char question[MAX_LINE_LENGTH];
	char answer;
	char options[4][100];
} questions;

typedef struct
{
	int score;
	float time;
}scores;


bool ques(const nd *, const int);

//void show_prize(int);

void view_highscore(const scores, const char*);

void print_qna(const questions);

scores endless(nd *);

scores playing(nd *);

bool asked(const int, const int, const bool);

int main()
{
	system("clear");
	srand(time(NULL));
	nd *f = NULL;
	char key, choice, inchoice;
	bool playflag = true;
	scores classiccurr_score = {0, 0}, endlesscurr_score = {0, 0};
	f = NULL;
	f = nodeinfo_loader(f);
	while(true)
	{
		choice = key = '0';
		printf("\n-----------Welcome to the Quiz-----------\n");
		printf("Enter 1 to play classic mode\nEnter 2 to play endless mode\nEnter 3 to view high score\nEnter 4 to quit\n");
		scanf(" %c", &choice);
		system("clear");
		switch (choice)
		{
		case '1': 
			classiccurr_score = playing(f);
			asked(0, 0, 1);
			system("clear");
			break;

		case '2':
			endlesscurr_score = endless(f);
			asked(0, 0, 1);
			system("clear");
			break;
			break;
			
		case '3':
			inchoice = '0';
			while(inchoice != '1' && inchoice != '2')
			{
				printf("Press 1 for Classic mode, 2 for Endless mode\n");
				scanf(" %c", &inchoice);
				switch(inchoice)
				{
					case '1':	view_highscore(classiccurr_score, "classic");
							break;
					case '2': view_highscore(endlesscurr_score, "endless");
							break;
					default:printf("Invalid choice\n");
				}
			}
			system("clear");
			break;

		case '4':
			fr(f);
			printf("Thank you for playing the quiz!\n");
			return 0;
		default:
			printf("Invalid choice\n");
		}
	}
}


bool ques(const nd *f, const int score)
{
	bool flag = false, aflag = true;
	int lines = 0, current_line = 0;
	char ch, key = '0', ans;
	questions q;
	long int bytes = 0;
	FILE *qfile = NULL, *afile = NULL;
	qfile = fopen(f->q_filename, "r");
	afile = fopen(f->a_filename, "r");
	while (!feof (qfile))
	{
		ch = fgetc (qfile);
		if (ch == '\n')
			lines++;
	}
	rewind(qfile);
	int random_line = rand () % lines;
	while((asked(atoi(((f->q_filename)+1)) - 1, random_line, 0)))
		random_line = rand() % lines;
	while(true)
	{
		fgets(q.question, MAX_LINE_LENGTH, qfile);
		if (current_line == random_line)
		{
			int line_no = 0;
			while (!feof (afile))
			{
				if (line_no == random_line)
				{
					bytes = ftell (afile);
					fseek (afile, bytes, SEEK_SET);
					break;
				}
				ch = fgetc (afile);
				if (ch == '\n')
					line_no++;
			}
			break;
		}
		current_line++;
	}
	for (size_t i = 0; i < 4; i++)
	{
		fscanf(afile, "%[^_]", q.options[i]);
		fseek(afile, 1, SEEK_CUR);
	}
	fscanf(afile, "%c", &q.answer);
	printf("Question is loading...\n");
	sleep(1);
	system("clear");
	printf("Current Score : %d\n", score);
	printf("\n-----------Question-----------\n");
	print_qna(q);
	printf("\n");
	fclose(qfile);
	fclose(afile);
	while(aflag)
	{
		printf("Enter your answer: ");
		scanf(" %c", &ans);
		ans = toupper(ans);
		if(ans == 'A' || ans == 'B' || ans == 'C' || ans == 'D')
			aflag = false;
		else
			printf("Invalid option...Please enter valid options\n");
	}
	if (ans == q.answer)
	{
		printf("Correct answer!\n");
		flag = true;
	}
	else
	{
		printf("Wrong answer!\n");
		printf("Right answer: %c\n", q.answer);
		printf("Better Luck Next Time...\n");
	}
	return flag;
}

/*void show_prize(int score)
{
	long int prize[] = {0, 1000, 2000, 5000, 10000, 20000, 40000, 80000, 160000, 320000, 640000};
	printf("Right Answers : Prize\n");
	for(int i = 0; i <= 10; i ++)
		printf("       %2d       %ld\n",i , prize[i]);
}*/
void view_highscore(const scores curr_score, const char *type)
{
	char key = '0';
	static scores endhigh_score = {0, 0};
	static scores classichigh_score = {0, 0};
	if(strcmp(type,"classic") == 0)
	{
		if(curr_score.score > classichigh_score.score)
			classichigh_score = curr_score;
		else if(curr_score.score == classichigh_score.score)
			if(curr_score.time < classichigh_score.time)
				classichigh_score = curr_score;
		printf("Highest Score: %d in %f seconds\n", classichigh_score.score, classichigh_score.time);
	}
	else
	{
		if(curr_score.score > endhigh_score.score)
			endhigh_score = curr_score;
		else if(curr_score.score == endhigh_score.score)
			if(curr_score.time < endhigh_score.time)
				endhigh_score = curr_score;
		printf("Highest Score: %d in %f seconds\n", endhigh_score.score, endhigh_score.time);
	}
	if(curr_score.time)
		printf("Current Score: %d in %f seconds\n", curr_score.score, curr_score.time);
	while(key != 'b' && key !='B')
			{ 
				printf("Press b to return to home screen\n");
				scanf(" %c",&key);
			}
}
void print_qna(const questions q)
{
	printf("%s\n", q.question);
	printf("\n-----------Options-----------\n");
	for (int i = 0; i < 4; i++)
		printf("%s\n", q.options[i]);
}

scores playing(nd *f)
{
	nd *cycle = f;
	scores curr_score = {0,0};
	bool play = true;
	struct timeval start, end;
	double time_taken = 0;
	char ch;
	do
	{
		gettimeofday(&start, NULL);
		play = ques(f, curr_score.score);
		gettimeofday(&end, NULL);
		time_taken = (end.tv_sec - start.tv_sec) * 1e6;
		time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
		curr_score.time += time_taken - 1;
		if(play)
		{
			(curr_score.score)++;
			f = f->link;
		}
		else
			break;
		sleep(2);
		system("clear");
	}while(f != cycle);
	if(curr_score.score != 0 && f == cycle)
		printf("Congratulations\nYou Won\n");
	printf("Score: %d\n",curr_score.score);
	printf("Time taken: %lf\n", curr_score.time);
	while(ch != 'v' && ch != 'V' && ch != 'b' && ch !='B')
	{
		printf("Press v to view high score\n");
		printf("Press b to return to main menu\n");
		scanf(" %c",&ch);
			if(ch == 'v' || ch == 'V')
			{
				system("clear");
				view_highscore(curr_score, "classic");
				return curr_score;
			}
			else if(ch == 'b' || ch == 'B')
				return curr_score;
	}
}

scores endless(nd *f)
{
	scores curr_score = {0,0};
	bool play = true;
	struct timeval start, end;
	double time_taken = 0;
	char ch;
	while(curr_score.score < 100)
	{
		gettimeofday(&start, NULL);
		play = ques(f, curr_score.score);
		gettimeofday(&end, NULL);
		time_taken = (end.tv_sec - start.tv_sec) * 1e6;
		time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
		curr_score.time += time_taken - 1;
		if(play)
		{
			(curr_score.score)++;
			f = f->link;
		}
		else
			break;
		sleep(2);
		system("clear");
	};
	if(curr_score.score ==100)
		printf("Sorry, we do not have any more questions to ask\n");
	printf("Score: %d\n",curr_score.score);
	printf("Time taken: %lf\n", curr_score.time);
	while(ch != 'v' && ch != 'V' && ch != 'b' && ch !='B')
	{
		printf("Press v to view high score\n");
		printf("Press b to return to main menu\n");
		scanf(" %c",&ch);
			if(ch == 'v' || ch == 'V')
			{
				system("clear");
				view_highscore(curr_score, "endless");
				return curr_score;
			}
			else if(ch == 'b' || ch == 'B')
				return curr_score;
	}
}

bool asked(const int file_no,const int line_no,const bool clearflag)
{
	static int **repeat_tracker = NULL;
	if(!repeat_tracker)
	{
		int **temp = NULL;
		while(!temp)
			temp =(int**)calloc(sizeof(int*),10);
		for(int i = 0;i < 10;i ++)
			temp[i] = (int*)calloc(sizeof(int),10);
		repeat_tracker = temp;
	}
	if(clearflag)
	{
		free(repeat_tracker);
		repeat_tracker = NULL;
		return 1;
	}
	if(repeat_tracker[file_no][line_no])
		return 1;
	else
	{
		repeat_tracker[file_no][line_no] = 1;
		return 0;
	}
}
