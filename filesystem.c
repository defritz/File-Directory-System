/*
 * file.c
 *
 *  Created on: Oct 31, 2016
 *      Author: Derek
 */

#include "filesystem.h"

fs* newFileSystem(){
	fs* f = (fs*)malloc(sizeof(fs));
	f -> root = (node*)malloc(sizeof(node));
	f -> root -> FC = NULL;
	f -> root -> NS = NULL;
	f -> root -> P = NULL;
	strcpy(f -> root -> name, "/");
	f -> CD = f -> root;
	return f;
}

int comparison(node* n1, node* n2){
	int size, x = 0;
	if(n1 -> type == 'D'){
		if(n2 -> type == 'F')
			return -1;
	}
	if(n1 -> type == 'F'){
		if(n2 -> type == 'D')
			return 1;
	}
	if(strcmp(n1 -> name, n2 -> name) == 0)
		return 0;
	if (strlen(n1 -> name) < strlen(n2 -> name))
		return -1;
	else if(strlen(n1 -> name) > strlen(n2 -> name))
		return 1;
	else
		size = strlen(n1 -> name);
	for(x = 0; x < size - 1; x++){
		char c1 = n1 -> name[x];
		char c2 = n2 -> name[x];
		if(c1 == c2)
			continue;
		if(c1 == '\0')
			return -1;
		if(c2 == '\0')
			return 1;
		if(c1 == '.')
			return -1;
		if(c2 == '.')
			return 1;
		if(c1 == '-')
			return -1;
		if(c2 == '-')
			return 1;
		if(c1 == '_')
			return -1;
		if(c2 == '_')
			return 1;
		if(c1 < c2)
			return -1;
		if(c1 > c2)
			return 1;
	}
	return 0;
}

void mkdir(fs* f, char* path){
	char* token = strtok(path, "/");
	node* temp = f -> root -> FC;
	while(1){
		if(temp == NULL){ //if file system is empty other than root
			temp = (node*)malloc(sizeof(node));
			node* newDir = (node*)malloc(sizeof(node));
			strcpy(newDir -> name, token);
			newDir -> type = 'D';
			newDir -> FC = NULL;
			newDir -> NS = NULL;
			newDir -> P = f -> root;
			f -> root -> FC = newDir;
			recAddDir(f -> root -> FC, token);
			return;
		}

		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){ //match found, but the found directory is empty
				token = strtok(NULL, "/");
				if(token == NULL){
					printf("Error Output: Make Dir Error: Cannot create directory.\n");
					return;
				}
				node* newDir = (node*)malloc(sizeof(node));
				strcpy(newDir -> name, token);
				newDir -> type = 'D';
			    newDir -> FC = NULL;
			    newDir -> NS = NULL;
			    newDir -> P = temp;
				temp -> FC = newDir;
				temp = temp -> FC;
				recAddDir(temp, token);
				return;
			}
			else{ //match found in current directory, proceed inside
				temp = temp -> FC;
				token = strtok(NULL, "/");
			}
		}
		else{ //match not found, look at the next entry in current directory
			if(temp -> NS == NULL){ //current directory exhausted, add the directory in current directory
				node* newDir = (node*)malloc(sizeof(node));
				strcpy(newDir -> name, token);
				newDir -> type = 'D';
				newDir -> FC = NULL;
				newDir -> NS = NULL;
				newDir -> P = temp -> P;
				////// ORDERING //////
				node* temp2 = temp -> P -> FC;
				int added = 0;
				newDir -> NS = temp2; //add to beginning of current directory
				temp -> P -> FC = newDir;
				node* prev = newDir -> NS;
				while(newDir -> NS != NULL){
					if(comparison(newDir, newDir -> NS) == 1){ //if it is lower precedence, then move down the list
						if(newDir == temp -> P -> FC){
							temp -> P -> FC = temp2;
						}
						node* temp3 = newDir -> NS -> NS;
						node* temp4 = newDir -> NS;
						newDir -> NS -> NS = newDir;
						newDir -> NS = temp3;
						if(prev != temp4){
							prev -> NS = temp4;
							prev = prev -> NS;
						}
						added = 1;
					}
					else{
						added = 1;
						break;
					}
				}
				////// ORDERING //////
				if(added == 0){
					temp -> NS = newDir;
					temp = temp -> NS;
				}
				recAddDir(temp, token);
				return;
			}
			else
				temp = temp -> NS;
		}
	}
}


void recAddDir(node* n, char* token){
	token = strtok(NULL, "/");
	if(token != NULL){
		node* newNode = (node*)malloc(sizeof(node));
		strcpy(newNode -> name, token);
		newNode -> type = 'D';
		newNode -> FC = NULL;
		newNode -> NS = NULL;
		newNode -> P = n;
		n -> FC = newNode;
		recAddDir(n -> FC, token);
	}
}

void touch(fs* f, char* path){
	char* token = strtok(path, "/");
	node* temp = f -> root -> FC;
	while(1){
		if(temp == NULL){ //if file system is empty other than root
			temp = (node*)malloc(sizeof(node));
			node* newFile = (node*)malloc(sizeof(node));
			strcpy(newFile -> name, token);
			newFile -> type = 'D';
			newFile -> FC = NULL;
			newFile -> NS = NULL;
			newFile -> P = f -> root;
			f -> root -> FC = newFile;
			token = strtok(NULL, "/"); ///
			recAddFile(f -> root -> FC, token);
			return;
		}
		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){ //match found, but the found directory is empty
				token = strtok(NULL, "/");
				if(token == NULL){
					printf("Error Output: Touch Error: Cannot create file.\n");
					return;
				}
				node* newFile = (node*)malloc(sizeof(node));
				strcpy(newFile -> name, token);
				newFile -> type = 'D';
			    newFile -> FC = NULL;
			    newFile -> NS = NULL;
			    newFile -> P = temp;
				temp -> FC = newFile;
				temp = temp -> FC;
				token = strtok(NULL, "/"); ///
				recAddFile(temp, token);
				return;
			}
			else{ //match found in current directory, proceed inside
				temp = temp -> FC;
				token = strtok(NULL, "/");
			}
		}
		else{ //match not found, look at the next entry in current directory
			if(temp -> NS == NULL){ //current directory exhausted, add the directory in current directory
				node* newFile = (node*)malloc(sizeof(node));
				strcpy(newFile -> name, token);
				token = strtok(NULL, "/");
				if(token == NULL)
					newFile -> type = 'F';
				else
					newFile -> type = 'D';
				newFile -> FC = NULL;
				newFile -> NS = NULL;
				newFile -> P = temp -> P;
				////// ORDERING //////
				node* temp2 = temp -> P -> FC;
				int added = 0;
				newFile -> NS = temp2; //add to beginning of current directory
				temp -> P -> FC = newFile;
				node* prev = newFile -> NS;
				while(newFile -> NS != NULL){
					if(comparison(newFile, newFile -> NS) == 1){ //if it is lower precedence, then move down the list
						if(newFile == temp -> P -> FC){
							temp -> P -> FC = temp2;
						}
						node* temp3 = newFile -> NS -> NS;
						node* temp4 = newFile -> NS;
						newFile -> NS -> NS = newFile;
						newFile -> NS = temp3;
						if(prev != temp4){
							prev -> NS = temp4;
							prev = prev -> NS;
						}
						added = 1;
					}
					else{
						added = 1;
						break;
					}
				}
				////// ORDERING //////
				if(added == 0){
					temp -> NS = newFile;
					temp = temp -> NS;
				}
				recAddFile(newFile, token);
				return;
			}
			else
				temp = temp -> NS;
		}
	}
}

void recAddFile(node* n, char* token){
	if(token == NULL){
		n -> type = 'F';
	}
	if(token != NULL){
		node* newNode = (node*)malloc(sizeof(node));
		strcpy(newNode -> name, token);
		newNode -> type = 'D';
		newNode -> FC = NULL;
		newNode -> NS = NULL;
		newNode -> P = n;
		n -> FC = newNode;
		token = strtok(NULL, "/");
		recAddFile(n -> FC, token);
	}
}

void order(fs* f){
	recOrder(f -> root -> FC);
}

void recOrder(node* n){
	if(n -> NS != NULL){
		printf("%s", n -> name);
		if(comparison(n, n -> NS) == 1){
			node* temp = n;
			n = n -> NS;
			n -> NS = temp;
			recOrder(n);
		}
	}
	if(n -> NS != NULL){
		recOrder(n -> NS);
	}
}

void recRemove(node* n){
	if(n -> FC != NULL){
		recRemove(n -> FC);
	}
	if(n -> NS != NULL){
		recRemove(n -> NS);
	}
	node* parent = n -> P;
	parent -> FC = NULL;
	free(n);

}

void rm(fs* f, char* path, int force){
	char* token = strtok(path, "/");
	node* temp = f -> root -> FC;
	while(1){
		if(temp == NULL){
			printf("Error Output: Remove Error: Cannot remove file or directory.\n");
			return;
		}
		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){
				token = strtok(NULL, "/");
				if(token == NULL){
					node* parent = temp -> P;
					parent -> FC = NULL;
					free(temp);
					return;
				}
				else{
					printf("Error Output: Remove Error: Cannot remove file or directory.\n");
					return;
				}
			}
			else{
				token = strtok(NULL, "/");
				if(token == NULL){
					if(force == 1){
						recRemove(temp);
						return;
					}
					else{
						printf("Error Output: Remove Error: directory '%s' is not empty.\n", path);
						return;
					}
				}
				temp = temp -> FC;
			}
		}
		else{
			if(temp -> NS == NULL){
				printf("Error Output: Remove Error: Cannot remove file or directory.\n");
				return;
			}
			else{
				temp = temp -> NS;
			}
		}
	}
}

void find(fs* f, char* name){
	printf("Searching For '%s':\n", name);
	recFind(f,f -> root -> FC, name);
}

void recFind(fs* f, node* n, char* name){
	findHelp(f, n, name);
	if(n -> FC != NULL){
		recFind(f, n -> FC, name);
	}
	if(n -> NS != NULL){
		recFind(f, n -> NS, name);
	}
}

void findHelp(fs* f, node* n, char* name){
	int i = 0;
	int j = 0;
	int c = 0;
	char PWD[1000];
	memset(PWD, 0, 1000);
	int complete = strlen(name);
	for(i = 0; i < strlen(n -> name); i++){
		if(n -> name[i] == name[j]){
			c++;
			j++;
			if(c == complete){
				node* temp = f -> CD;
				f -> CD = n;
				strcpy(PWD, pwd(f,1));
				printf("%c ", n -> type);
				printf("%s\n", PWD);
				f -> CD = temp;
				break;
			}
		}
	}
}

char* pwd(fs* f,int finder){
	char PWD[1000];
	memset(PWD, 0, 1000);
	if(f -> CD == f -> root){
		strcpy(PWD, "/");
		return PWD;
	}
	return recPWD(f -> CD, PWD, 0, finder);
}

char* recPWD(node* n, char* PWD, int first, int finder){
	char* temp;
	char temp2[21];
	if(n != NULL){
		temp = strdup(PWD);
		if(strcmp(n -> name, "/") != 0 && first > 0){
			strcpy(temp2, n -> name);
			strcat(temp2, "/");
			strcpy(PWD, temp2);
		}
		else{
			printf('\0');
			strcpy(PWD, n -> name);
		}
		strcat(PWD, temp);
		recPWD(n -> P, PWD, 1, finder);
	}
	free(temp);
	return PWD;
}

void cd(fs* f, char* path){
	if(strcmp(path, "/") == 0){
		f -> CD = f -> root;
		return;
	}
	char* token = strtok(path, "/");
	node* temp = f -> root -> FC;

	while(1){
		if(token == NULL || (strcmp(token, ".") != 0 && strcmp(token, "..") != 0)){ //return to normal cd method
			break;
		}
		if(strcmp(token, ".") == 0){
			temp = f -> CD;
			token = strtok(NULL, "/");
			if(token == NULL){
				f -> CD = temp;
				return;
			}
		}
		else if(strcmp(token, "..") == 0){
			if(f -> CD == f -> root)
				temp = f -> root;
			else{
				temp = f -> CD -> P;
				f -> CD = f -> CD -> P;
			}
			token = strtok(NULL, "/");
			if(token == NULL){
				f -> CD = temp;
				return;
			}
		}
	}

	while(1){
		if(temp == NULL){
			printf("Error Output: Change Dir Error: Cannot change working directory.\n");
			return;
		}
		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){
				token = strtok(NULL, "/");
				if(token == NULL){
					if(temp -> type == 'D'){
						f -> CD = temp;
						return;
					}
					else{
						printf("Error Output: Change Dir Error: Cannot change working directory.\n");
						return;
					}
				}
				else{
					printf("Error Output: Change Dir Error: Cannot change working directory.\n");
					return;
				}
			}
			else{
				token = strtok(NULL, "/");
				if(token == NULL){
					if(temp -> type == 'D'){
						f -> CD = temp;
						return;
					}
					else{
						printf("Error Output: Change Dir Error: Cannot change working directory.\n");
						return;
					}
				}
				temp = temp -> FC;
			}
		}
		else{
			if(temp -> NS == NULL){
				printf("Error Output: Change Dir Error: Cannot change working directory.\n");
				return;
			}
			else{
				temp = temp -> NS;
			}
		}
	}
}

void ls(fs* f, char* path){
	if(strcmp(path, "") == 0){
		node* temp = f -> CD -> FC;
		char* PWD = pwd(f,0);
		printf("Listing For '%s':\n", PWD);
		while(temp != NULL){
			if(temp -> type == 'D')
				printf("D ");
			else
				printf("F ");
			printf("%s\n", temp -> name);
			temp = temp -> NS;
		}
	}
	else{
		char* token = strtok(path, "/");
		node* temp = f -> root -> FC;
		while(1){
			if(temp == NULL){
				printf("Error Output: List Error: Cannot perform list operation.\n");
				return;
			}
			if(strcmp(token, ".") == 0){
				temp = f -> CD;
				token = strtok(NULL, "/");
				if(token == NULL)
					break;
			}
			if(strcmp(token, "..") == 0){
				if(f -> CD == f -> root){
					temp = f -> root;
				}
				else{
					temp = f -> CD -> P;
					f -> CD = f -> CD -> P;
				}
				token = strtok(NULL, "/");
				if(token == NULL)
					break;
			}
			if(strcmp(temp -> name, token) == 0){
				if(temp -> FC == NULL){ //match found, but the found directory is empty
					token = strtok(NULL, "/");
					if(token == NULL)
						break;
					else{
						printf("Error Output: List Error: Cannot perform list operation.\n");
						return;
					}
				}
				else{ //match found in current directory, proceed inside
					token = strtok(NULL, "/");
					if(token == NULL){
						break;
					}
					temp = temp -> FC;
				}
			}
			else{ //match not found, look at the next entry in current directory
				if(temp -> NS == NULL){ //current directory exhausted, add the directory in current directory
					printf("Error Output: List Error: Cannot perform list operation.\n");
					return;
				}
				else
					temp = temp -> NS;
			}
		}
		if(temp -> type == 'F'){
			printf("Error Output: List Error: Cannot perform list operation. '%s' is a file.\n", path);
			return;
		}
		else{
			printf("Listing For '/%s':\n", path);
			temp = temp -> FC;
			while(temp != NULL){
				if(temp -> type == 'D')
					printf("D ");
				else
					printf("F ");
				printf("%s\n", temp -> name);
				temp = temp -> NS;
			}
		}
	}
}








