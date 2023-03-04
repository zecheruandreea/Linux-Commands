#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir {
  char *name;
  struct Dir *parent;
  struct File *head_children_files;
  struct Dir *head_children_dirs;
  struct Dir *next;
} Dir;

typedef struct File {
  char *name;
  struct Dir *parent;
  struct File *next;
} File;

void free_dir(Dir *dir);

void free_file(File *tmp);

void touch(Dir *parent, char *name) {

  // Create file
  File *final_file = calloc(1, sizeof(File));
  final_file->name = calloc(1, MAX_INPUT_LINE_SIZE);
  strcat(final_file->name, name);
  final_file->next = NULL;
  final_file->parent = parent;

  // Check if it exists
  if (parent->head_children_files == NULL) {
    parent->head_children_files = final_file;
	  return;
  } else {
	    File *copy_head = parent->head_children_files;
	    while (copy_head != NULL) {
		    if (strcmp(copy_head->name, final_file->name) == 0) {
			    free_file(final_file);
			    printf("File already exists\n");
			    return;
		    }
	    copy_head = copy_head->next;
	  }
  }

  // If it doesnt exist, add to end
  File* tmp = parent->head_children_files;
  while(tmp != NULL && tmp->next != NULL) 
	  tmp = tmp->next;
  tmp->next = final_file;
}

void mkdir(Dir *parent, char *name) {

  Dir *final_dir = (Dir *)calloc(1, sizeof(Dir));
  final_dir->name = calloc(1, MAX_INPUT_LINE_SIZE);
  strcat(final_dir->name, name);
  final_dir->next = NULL;
  final_dir->parent = parent;

  if (parent->head_children_dirs == NULL) {
    parent->head_children_dirs = final_dir;
  } else {
    Dir *copy_head = parent->head_children_dirs;
    while (copy_head ->next != NULL) {
      if (strcmp(copy_head->name, final_dir->name) == 0) {
        printf("Directory already exists\n");
        free(final_dir->name);
        free(final_dir);
        return;
      }
      copy_head = copy_head->next;
    }

    if (strcmp(copy_head->name, final_dir->name) == 0) {
      printf("Directory already exists\n");
      free(final_dir->name);
      free(final_dir);
      return;
    }
    copy_head->next = final_dir;
  }
}

void ls(Dir *parent) {

  Dir *iterator_dir = parent->head_children_dirs;
  while (iterator_dir != NULL) {
    printf("%s\n", iterator_dir->name);
    iterator_dir = iterator_dir->next;
  }

  File *iterator_file = parent->head_children_files;
  while (iterator_file != NULL) {
    printf("%s\n", iterator_file->name);
    iterator_file = iterator_file->next;
  }
}

void free_file(File *tmp) {
  free(tmp->name);
  free(tmp);
  tmp = NULL;
}

void rm(Dir *parent, char *name) {

  if (parent == NULL) {
    printf("Could not find the file\n");
    return;
  }

  if (parent->head_children_files == NULL) {
    printf("Could not find the file\n");
    return;
	
  } else { // are copii
	  // primul element
	  File* start = parent->head_children_files;
	  if(strcmp(start->name, name) == 0) {
		  parent->head_children_files = parent->head_children_files->next;
		  free_file(start);
		  return;
	  } else {
		  while(start->next != NULL) {
			  if(strcmp(start->next->name, name) == 0) {
				  File* keep = start->next;
				  start->next = start->next->next;
				  free_file(keep);
				  return;
			  }
			start = start->next;
		  }
	  }
  }
  printf("Could not find the file\n");
}

void rmdir(Dir *parent, char *name) {

  Dir *copy_head = parent->head_children_dirs;

  if (parent->head_children_dirs == NULL) {
    printf("Could not find the dir\n");
    return;
  }

  if (copy_head != NULL && strcmp(copy_head->name, name) == 0) {
    parent->head_children_dirs = copy_head->next;
    free_dir(copy_head);
    return;
  }

  while (copy_head->next != NULL) {
    if (strcmp(copy_head->next->name, name) == 0) {
      Dir *tmp = copy_head->next;
      copy_head->next = copy_head->next->next;
      free_dir(tmp);
      return;
    }
    copy_head = copy_head->next;
  }

  if (copy_head->next == NULL) {
    printf("Could not find the dir\n");
    return;
  }

  free_dir(copy_head);
}

void cd(Dir **target, char *name) {

  if (strcmp(name, "..") == 0) {
    if ((*target)->parent == NULL) {
      return;
    } else {
      *target = (*target)->parent;
      return;
    }
  }

  Dir *copy_head = (*target)->head_children_dirs;
  while (copy_head != NULL) {
    if (strcmp(copy_head->name, name) == 0) {
      *target = copy_head;
      return;
    }
    copy_head = copy_head->next;
  }

  printf("No directories found!\n");
}

char *pwd(Dir *target) {}

void free_dir(Dir *dir) {

  free(dir->name);

  File *copy_head = dir->head_children_files;
  File *tmp;

  while (copy_head != NULL) {
    tmp = copy_head;
    copy_head = copy_head->next;
    free(tmp->name);
    free(tmp);
    tmp = NULL;
  }

  free(dir);
  dir = NULL;
}

void stop(Dir *target) {
  
  if (target != NULL) {
    stop(target->head_children_dirs);
    stop(target->next);
    free_dir(target);
  }
}

void tree(Dir *target, int level) {}

void mv(Dir *parent, char *oldname, char *newname) {}

int main() {

  Dir *root = (Dir *)calloc(1, sizeof(Dir));
  root->name = calloc(1, MAX_INPUT_LINE_SIZE);
  strcat(root->name, "home");
  root->parent = NULL;
  root->head_children_files = NULL;
  root->head_children_dirs = NULL;
  root->next = NULL;
  Dir **folder_curent = &root;

  char* str = calloc(1, MAX_INPUT_LINE_SIZE);

  do {
  	fgets(str, MAX_INPUT_LINE_SIZE, stdin);
	  str[strlen(str) -1] = '\0';

    if (strstr(str, "touch") != NULL) {
      touch(*folder_curent, strchr(str, ' ') + 1);
    } else if (strstr(str, "mkdir") != NULL) {
      mkdir(*folder_curent, strchr(str, ' ') + 1);
    } else if (strstr(str, "ls") != NULL) {
      ls(*folder_curent);
    } else if (strstr(str, "rmdir") != NULL) {
      rmdir(*folder_curent, strchr(str, ' ') + 1);
    } else if (strstr(str, "rm") != NULL) {
      rm(*folder_curent, strchr(str, ' ') + 1);
    } else if (strstr(str, "cd") != NULL) {
      cd(folder_curent, strchr(str, ' ') + 1);
    } else if (strstr(str, "stop") != NULL) {
      while ((*folder_curent)->parent != NULL)
        *folder_curent = (*folder_curent)->parent;
      stop(*folder_curent);
      break;
    }
  } while(1);

  free(str);
  return 0;
}