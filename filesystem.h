/*
 * file.h
 *
 *  Created on: Oct 31, 2016
 *      Author: Derek
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "stdheader.h"

typedef struct _node node;
struct _node{
	char name[21];
	char type;
	node* P;
	node* FC;
	node* NS;
};

typedef struct _filesystem fs;
struct _filesystem{
	node* root;
	node* CD;
};

//Tree Functions
int comparison(node*,node*);
void display(node*);
void insert(node*,char*);
node* recinsert(node*,char*);
void clear(fs*);
void recclear(node*);

//File System Functions
fs* newFileSystem();
void destroy(fs*);
void ls(fs*,char*);
char* pwd(fs*,int);
char* recPWD(node*,char*,int,int);
void mkdir(fs*,char*);
void recAddDir(node*,char*);
void touch(fs*,char*);
void recAddFile(node*,char*);
void order(fs*);
void recOrder(node*);
void cd(fs*,char*);
void rm(fs*,char*,int);
void recremove(node*);
void rmf(fs*,char*);
void find(fs*,char*);
void recFind(fs*,node*,char*);
void findHelp(fs*,node*,char*);

#endif /* FILESYSTEM_H_ */
