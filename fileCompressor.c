#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>


typedef struct LLNode{//node for linked list that contains all tokens read in
  char* tok;
  int fre;//frequency
  struct LLNode* next;
} LLNode;

typedef struct MHNode{//min heap node for huffman
  char* tok;
  int data;
  struct MHNode * left;
  struct MHNode * right;
} MHNode;

typedef struct MHeap{
  int num;//size
  int max;//cap
  struct MHNode ** arry;
} MHeap;
//Helper

typedef struct LLCNode{
  char* tok;
  char* bin;//frequency
  struct LLCNode* next;

} LLCNode;

void swapMHNode(MHNode ** first, MHNode ** second){
  MHNode * temp = *first;
  *first = *second;
  *second = temp;
}

//Heapify, turn tree into min heap
void heapify(MHeap * heap, int x){
  int qwe = x;
  int l = 2*x+1;
  int r = 2*x+2;

  if(l < heap->num && heap->arry[l]->data < heap->arry[qwe]->data) qwe = l;

  if(r < heap->num && heap->arry[r]->data < heap->arry[qwe]->data) qwe = r;
  if(qwe != x){//recur
    swapMHNode(&heap->arry[qwe], &heap->arry[x]);
    heapify(heap, qwe);
  }
}

void buildHeap(MHeap * heap){
  int qwe = heap->num-1;
  int q = 0;

  for(q = (qwe-1)/2; q >=0;q--){
    heapify(heap,q);

  }

}


void insert(MHeap * heap, MHNode* node){
  int qwe =heap->num;

  (heap->num)++;
  while(qwe && node->data < heap->arry[(qwe-1)/2]->data){
    heap->arry[qwe] = heap->arry[(qwe-1)/2];
    qwe = (qwe-1)/2;
  }

  heap->arry[qwe] = node;

}

MHNode* getMin(MHeap* heap){
  MHNode* qwe = heap->arry[0];
  heap->arry[0] = heap->arry[heap->num - 1];
  (heap->num)--;


  heapify(heap, 0);


  return qwe;
}

//a is the number of nodes in head
MHeap * buildHeap_Inputs(LLNode* head, int a){
  int i;
  LLNode* ptr = head;
  MHeap* heap = (MHeap*)malloc(sizeof(MHeap));

  heap->num = 0;
  heap->max = a;
  heap->arry= (MHNode**)malloc(a*sizeof(MHNode*));
  for(i = 0; i < a; i++){
    MHNode * qwe = (MHNode*)malloc(sizeof(MHNode));
    qwe->tok=ptr->tok;
    qwe->data=ptr->fre;
    qwe->left=NULL;
    qwe->right = NULL;
    heap->arry[i] = qwe;
    ptr = ptr->next;
  }
  heap->num = a;
  buildHeap(heap);

  return heap;
}

int oscount=2;
void createCodebook(MHNode* head, int store[], int a){//codebook recursion
  int fd = open("./HuffmanCodebook",O_RDWR | O_CREAT);

  char* cstore = (char*)malloc((a+1)*sizeof(char*));
  int i = 0;
  if(head->left){//not NULL
    store[a] = 0;
    createCodebook(head->left, store, a+1);
  }
  if(head->right){//not NULL
    store[a] = 1;
    createCodebook(head->right, store,a+1);
  }

  if(!(head->left) && !(head->right)){//leaf node

    for(i = 0; i<a; i++){
      cstore[i] = (char)(store[i]+48);
    }
    cstore[i] = '\0';

    lseek(fd, oscount, SEEK_CUR);

    write(fd, cstore, a);
    write(fd, "\t", 1);
    write(fd, head->tok, strlen(head->tok));
    write(fd, "\n", 1);

    oscount += (a+2+strlen(head->tok));
  }
  return;
}


void doHuffman(LLNode* head, int a){
  //printf("gate");

  MHNode* rt = NULL;
  MHNode* lt = NULL;
  MHNode* huff = (MHNode*)malloc(sizeof(MHNode));
  int qwe = 0,wer = 0;
  //printf("gate");

  MHeap* heap = buildHeap_Inputs(head,a);
  //  printf("gate");
  if(heap->num != 1) qwe = 1;


  while(qwe == 1){
    lt= getMin(heap);

    rt = getMin(heap);
    //  printf("gateloop");

    wer = lt->data + rt->data;
    //qwe = 0;


    MHNode* temp = (MHNode*)malloc(sizeof(MHNode));

    temp->tok = (char*)' ';
    temp->data=wer;
    temp->left=lt;
    temp->right = rt;
    // printf("2\n");

    insert(heap, temp);
    // printf("3\n");

    if(heap->num == 1) qwe = 0;
    // printf("4, %d\n", qwe);

  }

  huff = getMin(heap);

  oscount = 2;
  int fd = open("./HuffmanCodebook",O_RDWR | O_CREAT, 0744);
  int obama =write(fd, "$", 1);
  write(fd, "\n", 1);
  int temp2[a];
  createCodebook(huff, temp2, 0);
  return;
}

LLNode* LLinsert (LLNode* head, char* token, int size) {
  //printf("'%s'\n", token);
  int i;
  if (head->tok == NULL) {
    head->tok = (char*)malloc(sizeof(token));
    strcpy(head->tok, token);
    head->fre = 1;
    head->next = NULL;
    //printf("%s\n", head->tok);
    return head;
  }
  LLNode* temp = (LLNode*) malloc(sizeof(LLNode*));
  temp->next = (LLNode*) malloc(sizeof(LLNode*));
  temp->next->tok = NULL;
  temp->tok = (char*)malloc(sizeof(token));
  strcpy(temp->tok, token);
  //printf("'%s'\n", temp->tok);
  temp->fre = 1;
  LLNode* ptr = head;
  for(i = 0; i < size - 1; i++) {
    //printf("%s\n", ptr->tok);
    ptr = ptr->next;
  }
  ptr->next = temp;
  ptr = head;
  return head;
}

int tokenize (char* path, LLNode* head, int size) {
  int i;

  int file = open(path, O_RDONLY);
  int counter = open(path, O_RDONLY);
  printf("---> %s\n", path);

  char c, temp, term;
  int count;
  int repeat = 0;

  while (read(counter, &c, 1) == 1) {
    read(file, &temp, 1);

    count = 1;
    char temp1[] = {'$', '\0'};
    while (read(counter, &term, 1) == 1) {
      if (term == ' ') {
        temp1[1] = 's';
        break;
      } else if (term == '\n') {
        temp1[1] = 'n';
        break;
      } else if (term == '\t') {
        temp1[1] = 't';
        break;
      } else if (term == '\0') {
        temp1[1] = '0';
        break;
      }
      count++;
    }
    //printf("%d\n", count);


    char* rest = (char*)malloc(sizeof(char) * count);
    for (i = 0; i < count; i++) rest[i] = '\0';

    for (i = 0; i < count; i++) {
      read(file, &temp, 1);
      if (temp == ' ' || temp == ',' || temp == '\n' || temp == '\0') break;
      rest[i] = temp;
    }

    char* result = (char*)malloc((sizeof(char*) * count) + sizeof(char));
    result[0] = c;
    strcat(result, rest);
    for (i = count; i < strlen(result); i++) result[i] = '\0';
    LLNode* temp = (LLNode*) malloc(sizeof(LLNode*));
    LLNode* ptr = head;
    repeat = 0;

    printf("%s + %s\n", result, temp1);

    for(i = 0; i < size; i++) {                       //handles tokens
      //printf("%s -- %s\n", ptr->tok, result);
      if (strcmp(ptr->tok, result) == 0) {
        //printf("incrementing:: %s\n", ptr->tok);
        ptr->fre ++;
        repeat = 1;
        break;
      }
      if (ptr->next == NULL) break;
      ptr = ptr->next;
    }
    if (repeat == 0) {
      //printf("inserting :: %s\n", result);
      head = LLinsert(head, result, size);
      size++;
    }
    //printf("%s, %d\n", head->tok, size);

    ptr = head;
    repeat = 0;
    for(i = 0; i < size; i++) {                     //handles terminating chars
      //printf("%s -- %s\n", ptr->tok, result);
      if (strcmp(temp1, ptr->tok) == 0) {
        //printf("incrementing:: %s\n", ptr->tok);
        ptr->fre ++;
        repeat = 1;
        break;
      }
      if (ptr->next == NULL) break;
      ptr = ptr->next;
    }
    if (repeat == 0) {
      printf("inserting :: '%s'\n", temp1);
      head = LLinsert(head, temp1, size);
      size++;
    }
    //printf("%s, %d\n", head->tok, size);


  }


  int clo = close(file);
  clo = close(counter);
  return size;
}

int create (DIR* directory, int recur, LLNode* head, int size, char* fileName) {
  struct dirent* file;

  char temp[5];
  temp[4] = '\0';

  LLNode* ptr = head;

  int i;

  if (recur == 1) {
    while ((file = readdir(directory))) {
      printf("%s\n", file->d_name);
      if (file->d_type == DT_DIR && strcmp(file->d_name, ".") != 0  && strcmp(file->d_name, "..") != 0) {
        DIR* t = opendir(file->d_name);
        char path[4096];
        strcpy(path, file->d_name);
        strcat(path, "/");
        int change = chdir(path);
        size = create(t, recur, head, size, fileName);
        change = chdir("..");
      }
      if (strlen(file->d_name) > 4) {
        for (i = 0; i < 4; i++) {
          temp[i] = file->d_name[strlen(file->d_name) - 4 + i];
        }
        if (strcmp(temp, ".txt") == 0) {
          size = tokenize(file->d_name, head, size);
        }
      }
    }

  } else {
    printf("%s\n", fileName);
    size = tokenize(fileName, head, size);
  }

  return size;
}

void compressFile(char* fileName, char* codebook, char* compressedName) {
  //fileName is name of file to be, codebook name is huffmancodeboook, compressedname is given to new file.
  //open filename
  struct stat st;
  stat(codebook, &st);//st.st_size = file size
  int qwe = 0;

  char escc;
  char*code = (char*)malloc(st.st_size*sizeof(char*));

  LLCNode* head = (LLCNode*)malloc(sizeof(LLCNode*));
  int fd = open(codebook, O_RDONLY);

  read(fd,head->tok, 1);//gets terminator
  head->bin = "1";
  head->next = NULL;
  LLCNode* ptr = head;

  LLCNode* temp = (LLCNode*)malloc(sizeof(LLCNode*));
  read(fd,escc,1);//tok is holding \n
  temp->next = NULL;

  int size = 0;
  int i;

  while(0<read(fd,escc,1)){//creates LL from codebook
    if(escc == '\n'){
      //if(qwe == 0) LLNode* temp = (LLNode*)malloc(sizeof(LLNode*));
      temp->tok = code;
      ptr->next = temp;
      ptr = ptr->next;
      qwe = 0;
      code = (char*)realloc(code, st.st_size*sizeof(char));
      LLCNode* temp = (LLCNode*)malloc(sizeof(LLCNode*));
      temp->next = NULL;
      size++;
    }else if(escc == '\t'){
      temp->bin = code;
      qwe = 0;

    }else{
      code[qwe] = escc;
      qwe++;

    }
  }
  int fd2 = open(fileName, O_RDONLY);
  struct stat st2;
  stat(fileName, &st2);//st2.st_size = file size

  ptr = head->next;
  int fd3 = open(compressedName, O_RDWR | O_CREAT | O_APPEND, 0744);
  qwe = 0;

  char* spac;
  char* newl;
  char* tabh;

  //malloc this <-----------------------------------

  for (i = 0; i < size; i++) {
    ptr = ptr->next;
    if (strcmp(ptr->tok,"$s") == 0) {
      spac = ptr->bin;//gives huffmancode for space
      ptr = head->next;
      break;
    }
  }
  for (i = 0; i < size; i++) {
    ptr = ptr->next;
    if (strcmp(ptr->tok,"$t") == 0) {
      tabh = ptr->bin;//gives huffmancode for space
      ptr = head->next;
      break;
    }
  }
  for (i = 0; i < size; i++) {
    ptr = ptr->next;
    if (strcmp(ptr->tok,"$n") == 0) {
      newl = ptr->bin;//gives huffmancode for space
      ptr = head->next;
      break;
    }
  }

  printf("%d\n", size);
  ptr = head;
  for (i = 0; i < size; i++) {
    printf("%s\t%s\n", ptr->bin, ptr->tok);
  }

  printf("1\n");
  while(0<read(fd2, escc, 1)){
    if(escc == ' '){
      while(strcmp(ptr->tok,code)!=0){
        ptr = ptr->next;
      }
      write(fd3, ptr->bin, strlen(ptr->bin));
      write(fd3, spac, strlen(spac));

      //write(fd, head->tok, strlen(head->tok));
      code = (char*)realloc(code, st.st_size*sizeof(char));

      qwe = 0;
      ptr = head->next;

    }else if(escc == '\t'){
      write(fd3, tabh, strlen(tabh));

    }else if(escc == '\n'){
      while(strcmp(ptr->tok,code)!=0){
        ptr = ptr->next;
      }
      write(fd3, ptr->bin, strlen(ptr->bin));
      write(fd3, spac, strlen(spac));

      code = (char*)realloc(code, st.st_size*sizeof(char));
      qwe = 0;
      ptr = head->next;
    }else{
      code[qwe]= escc;
      qwe++;
    }
  }
  return;
}

void compress (DIR* directory, char* codebook, int recur, char* fileName) {
  struct dirent* file;
  int i;
  char temp[5];
  temp[4] = '\0';
  char ext[] = {'.', 'h', 'c', 'z'};

  if (recur == 1) {
    while ((file = readdir(directory))) {
      printf("%s\n", file->d_name);
      if (file->d_type == DT_DIR && strcmp(file->d_name, ".") != 0  && strcmp(file->d_name, "..") != 0) {
        DIR* t = opendir(file->d_name);
        char path[4096];
        strcpy(path, file->d_name);
        strcat(path, "/");
        int change = chdir(path);
        compress(t, codebook, recur, fileName);
        change = chdir("..");
      }
      if (strlen(file->d_name) > 4) {
        for (i = 0; i < 4; i++) {
          temp[i] = file->d_name[strlen(file->d_name) - 4 + i];
        }
        if (strcmp(temp, ".txt") == 0) {
          int fd = open(file->d_name, O_RDONLY);
          printf("compressing: %s\n", file->d_name);

          char* name = (char*)malloc(sizeof(char) * (strlen(file->d_name) + 4));
          for (i = 0; i < strlen(file->d_name); i++) {
            name[i] = file->d_name[i];
          }
          for (i = 0; i < 4; i++) {
            name[strlen(file->d_name) + i] = ext[i];
          }
          printf("%s\n", name);
          compressFile(file->d_name, codebook, name);

        }
      }
    }
  } else {
    char* name = (char*)malloc(sizeof(char) * (strlen(fileName) + 4));
    for (i = 0; i < strlen(fileName); i++) {
      name[i] = fileName[i];
    }
    for (i = 0; i < 4; i++) {
      name[strlen(fileName) + i] = ext[i];
    }
    printf("%s\n", name);
    compressFile(fileName, codebook, name);
  }
  return;
}

void decompressFile(char* fileName, LLCNode* codebook, char* decompressedName) {
  int fd = open(fileName, O_RDONLY);
  int decompressed = open(decompressedName, O_RDWR | O_CREAT | O_APPEND);
  struct stat st;
  stat(fd, &st);

  char escc;
  LLCNode* ptr = codebook;
  char*code = (char*)malloc(st.st_size*sizeof(char*));
  int qwe = 0;


  while(0<read(fd,escc,1)){
    code[qwe] = escc;
    qwe++;

    while(ptr){
      if(strcmp(ptr->bin,code) == 0){
        break;
      }
      ptr = ptr->next;
    }

    if(ptr !=NULL){
      if(strcmp(ptr->tok,"$t") ==0){
        write(decompressed, "\t", 1);
      }else if(strcmp(ptr->tok,"$n") == 0){
        write(decompressed, "\n",1);
      }else if(strcmp(ptr->tok,"$t") ==0){
        write(decompressed, "\s", 1);
      }else{
        write(decompressed, ptr->tok, strlen(ptr->tok));
      }

      qwe = 0;
      code = (char*)realloc(code, st.st_size*sizeof(char));
    }
    ptr = codebook;

  }

  return;
}

void decompress (DIR* directory, char* codebook, int recur, char* fileName) {
  struct dirent* file;
  int i;
  char temp[9];
  temp[8] = '\0';

  if (recur == 1) {
    //printf("%s\n", file->d_name);
    while ((file = readdir(directory))) {
      if (file->d_type == DT_DIR && strcmp(file->d_name, ".") != 0  && strcmp(file->d_name, "..") != 0) {
        DIR* t = opendir(file->d_name);
        char path[4096];
        strcpy(path, file->d_name);
        strcat(path, "/");
        int change = chdir(path);
        decompress(t, codebook, recur, fileName);
        change = chdir("..");
      }
      if (strlen(file->d_name) > 8) {
        for (i = 0; i < 8; i++) {
          temp[i] = file->d_name[strlen(file->d_name) - 8 + i];
        }
        if (strcmp(temp, ".txt.hcz") == 0) {
          int fd = open(file->d_name, O_RDONLY);
          char* name = (char*)malloc(sizeof(char) * (strlen(file->d_name) - 4));
          for (i = 0; i < strlen(file->d_name) - 4; i++) {
            name[i] = file->d_name[i];
          }
          printf("%s\n", name);
          decompressFile(file->d_name, codebook, name);
        }
      }
    }
  } else {
    char* name = (char*)malloc(sizeof(char) * (strlen(fileName) - 4));
    for (i = 0; i < strlen(fileName) - 4; i++) {
      name[i] = fileName[i];
    }
    printf("%s\n", name);
    decompressFile(fileName, codebook, name);
  }
  return;
}

int main(int argc, char** argv){

  DIR* directory;
  directory = opendir(argv[3]);

  int i;
  int recur = 0;
  char* flag;
  char* codebook;
  char* fileName;
  if (strcmp(argv[1], "-R") == 0) {
    recur = 1;
    flag = argv[2];
    directory = opendir(argv[3]);
    if (strcmp(flag, "-c") == 0 || strcmp(flag, "-d") == 0) {
      codebook = argv[4];
    }
    if (directory == NULL) {
      printf("Error: could not open directory\n");
      return 0;
    }
  } else {
    flag = argv[1];
    fileName = argv[2];
    if (strcmp(flag, "-c") == 0 || strcmp(flag, "-d") == 0) {
      codebook = argv[3];
    }
  }


  if (strcmp(flag, "-b") == 0) {
    LLNode* head = (LLNode*)malloc(sizeof(LLNode));
    head->tok = NULL;
    int size = 0;
    size = create(directory, recur, head, size, fileName);
    printf("%d\n", size);
    LLNode* ptr = head;
    for (i = 0; i < size; i++) {
      printf("%s -- %d\n", ptr->tok, ptr->fre);
      ptr = ptr->next;
    }
    doHuffman(head, size);

  } else if (strcmp(flag, "-c") == 0) {
    compress(directory, codebook, recur, fileName);
  } else if (strcmp(flag, "-d") == 0) {
    int book = open(codebook, O_RDONLY);
    int counter = open(codebook, O_RDONLY);
    printf("%s\n", codebook);
    char control;
    char c, temp, temp1;
    read(book, &control, 1);
    read(counter, &control, 1);
    read(book, &temp, 1);
    read(counter, &temp, 1);
    int count;
    while (read(book, &temp, 1) == 1) {

      count = 0;
      while (read(counter, &c, 1) == 1) {
        //printf("%c\n", c);
        if (c == '\t') break;
        count++;
      }
      //printf("%d\n", count);
      char* code = (char*)malloc(sizeof(char) * count);
      for (i = 0; i < count; i++) {
        read(book, &temp, 1);
        code[i] = temp;
      }

      count = 0;
      while (read(counter, &c, 1) == 1) {
        if (c == '\n') break;
        count++;
      }
      //printf("%d\n", count);
      char* token = (char*)malloc(sizeof(char) * count);
      for (i = 0; i < count; i++) {
        read(book, &temp, 1);
        token[i] = temp;
      }
      printf("%s-%s\n", code, token);
      printf("\n");


    }
    decompress(directory, codebook, recur, fileName);
  }




  if (recur == 1) {
    int clo = closedir(directory);
  }
  return 0;
}
