#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable: 4996)

typedef struct record {
	int id;
	char lname[64];
	char fname[64];
	char email[64];
	struct record* next;
} RECORD;

RECORD* ReadData(void) {
	int i,num;
	FILE* fp;
	RECORD* data = NULL;
	RECORD* top = NULL;
	RECORD* prev = NULL;

	if ((fp = fopen("datum.txt", "r")) == NULL) {
		fprintf(stderr, "File not found\n");
		exit(1);
	}
	fscanf(fp, "%d", &num);
	for (i = 0; i < num; i++) {
		// 構造体1個分のメモリ確保
		data = malloc(sizeof(RECORD));
		if (i == 0) top = data;
		if(i > 0) prev->next = data;
		
		data->id = i;
		fscanf(fp,"%s %s %s", data->lname, data->fname, data->email);
		prev = data;
		data->next = NULL;
	}
	fclose(fp);

	// 先頭ノードのポインタを返す
	return top;
}

int main(void) {
	RECORD* record;
	record = ReadData();

	while (1) {
		printf("%s\n", record->lname);
		if (record->next == NULL) break;
        record = record->next;
	}
	return 0;
}
