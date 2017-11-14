/*
 * main.c
 *
 *  Created on: Oct 31, 2016
 *      Author: Derek
 */

#include "filesystem.h"

int main(){
	fs* f = newFileSystem();
	char command[20];
	char path[1000];
	char name[21];
	while(scanf("%s", command) > 0){
		if(strcmp(command, "mkdir") == 0){
			scanf("%s", path);
			mkdir(f, path);
		}
		if(strcmp(command, "touch") == 0){
			scanf("%s", path);
			touch(f, path);
		}
		if(strcmp(command, "rm") == 0){
			scanf("%s", path);
			if(strcmp(path, "-f") == 0){
				scanf("%s", path);
				rm(f, path, 1);
			}
			else{
				rm(f, path, 0);
			}
		}
		if(strcmp(command, "rm -f") == 0){
			scanf("%s", path);
			rm(f, path, 1);
		}
		if(strcmp(command, "find") == 0){
			scanf("%s", name);
			find(f, name);
		}
		if(strcmp(command, "cd") == 0){
			scanf("%s", path);
			cd(f, path);
		}
		if(strcmp(command, "pwd") == 0){
			char* PWD = pwd(f,0);
			printf("%s\n", PWD);
		}
		if(strcmp(command, "ls") == 0){
			memset(path, 0, 1000);
			gets(path);
			if(path[0] == '0'){
				strcpy(path, "");
			}
			else{
				char* p = path;
				p += 1;
				strcpy(path, p);
			}
			ls(f, path);
		}
	}
	return 0;
}
