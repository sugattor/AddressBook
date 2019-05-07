#define _CRT_SECURE_NO_WARNINGS
#define TXTFILE "datum.txt"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct record {
	int id; // ID
	char lname[64]; // Last Name
	char fname[64]; // First Name
	char email[64]; // Email Address
	struct record* next; // Next Record
} RECORD;

// アドレス帳の情報をテキストファイルから読み込む
RECORD* ReadData(void) {
	int i, num;
	FILE* fp;
	RECORD* data = NULL;
	RECORD* top = NULL;
	RECORD* prev = NULL;

	if ((fp = fopen(TXTFILE, "r")) == NULL) {
		fprintf(stderr, "File not found\n");
		exit(EXIT_FAILURE);
	}

	// 番兵
	data = malloc(sizeof(RECORD));
	data->id = 0;
	data->next = NULL;
	top = data; // 先頭ノードであることを示す

	fscanf(fp, "%d", &num);
	if (num == 0) return top;

	// ノード作成
	prev = data;
	for (i = 1; i <= num; i++) {
		// 構造体1個分のメモリ確保
		data = malloc(sizeof(RECORD));
		prev->next = data;

		data->id = i;
		fscanf(fp, "%s %s %s", data->lname, data->fname, data->email);
		prev = data;
		data->next = NULL;
	}
	fclose(fp);

	// 先頭ノードのポインタを返す
	return top;
}

// アドレス帳の情報をテキストファイルに上書き
void WriteData(RECORD* record) {
	int i, count = 0;
	FILE* fp;
	RECORD* top = record;

	if ((fp = fopen(TXTFILE, "w")) == NULL) {
		fprintf(stderr, "File open error(w)\n");
		exit(EXIT_FAILURE);
	}

	if (record->next == NULL) {
		printf("No Data\n");
		fprintf(fp, "0\n");
		fclose(fp);
		exit(EXIT_SUCCESS);
	}

	// データの個数を数える
	while (record->next != NULL) {
		count++;
		record = record->next;
	}
	fprintf(fp, "%d\n", count);

	record = top->next;
	for (i = 0; i < count; i++) {
		fprintf(fp, "%s %s %s\n", record->lname, record->fname, record->email);
		record = record->next;
	}
	fclose(fp);
}

// 新しい情報を最後尾に一つ追加する
void Add(RECORD* record, char* lname, char* fname, char* email) {
	RECORD* prev, * data;
	int max = 0;// IDの最大値

	// 末尾に行く
	while (record->next != NULL) {
		record = record->next;
		if (max < record->id) max = record->id;
	}
	prev = record;

	// ノードを1つ作成
	data = malloc(sizeof(RECORD));
	prev->next = data;
	data->id = max + 1;
	strcpy(data->lname, lname);
	strcpy(data->fname, fname);
	strcpy(data->email, email);
	data->next = NULL;
}

// 既存の情報をidで指定して削除
void Delete(RECORD* record, int id) {
	RECORD* prev = record;
	record = record->next;

	// 一致するIDを見つける
	while (1) {
		if (record->id == id) break;
		if (record->next == NULL) return; // 見つからない場合は何もしない
		prev = record;
		record = record->next;
	}

	// 前のノードを次のノードかNULLに繋げる
	if (record->next == NULL) {
		prev->next = NULL;
	}
	else {
		prev->next = record->next;
	}
	free(record);
}

// 情報を1つ表示
void Display(RECORD * record) {
	printf("%d\t%s %s\t%s\n", record->id, record->lname, record->fname, record->email);
}

// 情報を全て表示
void DisplayAll(RECORD * head) {
	RECORD* current = head;

	while (current->next != NULL) {
		current = current->next;
		Display(current);
	}
}

// strcmp 車輪の再発明
int myStrcmp(char* word1, char* word2) {
	for (int i = 0; word1[i] != '\0' || word2[i] != '\0'; i++) {
		if (word1[i] == word2[i]) continue;
		if (word1[i] > word2[i]) return 1;
		if (word1[i] < word2[i]) return -1;
	}
	return 0;
}

// 姓,名,メールアドレスのどれかが完全一致した人の全ての情報を表示
void Find(RECORD * record, char* word) {
	// どれかにヒットしたら表示
	while (record->next != NULL) {
		record = record->next;
		if (myStrcmp(record->lname, word) == 0) Display(record);
		if (myStrcmp(record->fname, word) == 0) Display(record);
		if (myStrcmp(record->email, word) == 0) Display(record);
	}
}

// リストの順番をnextと交換
void Swap(RECORD * record, RECORD * prev) {
	RECORD* tmp;

	if (record == NULL) {
		fprintf(stderr, "Cannot swap(current null)\n");
		exit(EXIT_FAILURE);
	}

	if (record->next == NULL) {
		fprintf(stderr, "Cannot swap(next null)\n");
		exit(EXIT_FAILURE);
	}

	tmp = record->next->next;
	prev->next = record->next;
	record->next->next = record;
	record->next = tmp;
}

// 姓,名,メールアドレスのどれかでリストをソートして、全ての情報を表示
// item : 0:姓 1:名 2:メールアドレス
// isAsc : 0:降順 1:昇順
void Sort(RECORD * head
	, int item // 0:姓 1:名 2:メールアドレス
	, int isAsc // 0:降順 1:昇順
) {
	int count = 0; // リストの個数
	int swap = 0;
	int i;
	RECORD* current = head;
	RECORD* prev = NULL;
	RECORD* tmp;

	if (head->next == NULL) return;
	if (head->next->next == NULL) {
		DisplayAll(head);
		return;
	}

	// リストの個数を数える
	count = 0;
	while (current->next != NULL) {
		current = current->next;
		count++;
	}

	// バブルソート

	while (count > 1) {
		current = head->next;
		prev = head;
		count--;

		for (i = 0; i < count; i++) {
			swap = 0;

			if (isAsc) { // 昇順
				switch (item)
				{
				case 0: // 姓
					if (strcmp(current->lname, current->next->lname) > 0) swap = 1;
					break;
				case 1: // 名
					if (strcmp(current->fname, current->next->fname) > 0) swap = 1;
					break;
				case 2: // メールアドレス
					if (strcmp(current->email, current->next->email) > 0) swap = 1;
					break;
				}
			}
			else { // 降順
				switch (item)
				{
				case 0: // 姓
					if (strcmp(current->lname, current->next->lname) < 0) swap = 1;
					break;
				case 1: // 名
					if (strcmp(current->fname, current->next->fname) < 0) swap = 1;
					break;
				case 2: // メールアドレス
					if (strcmp(current->email, current->next->email) < 0) swap = 1;
					break;
				}
			}

			// 交換
			if (swap) {
				tmp = current->next;
				Swap(current, prev);
				prev = tmp;
			}
			else {
				prev = current;
				current = current->next;
			}
		}
	}

	// 表示部
	DisplayAll(head);
}

void AddDialog(RECORD * record) {
	char lname[64] = "";
	char fname[64] = "";
	char email[64] = "";

	printf("情報を追加します\n姓を入力:");
	scanf("%63[^\n]%*[^\n]", lname); getchar();

	printf("名を入力:");
	scanf("%63[^\n]%*[^\n]", fname); getchar();

	printf("メールアドレスを入力:");
	scanf("%63s%*[^\n]", email); getchar();

	Add(record, lname, fname, email);
	printf("%s %s : %s\nを追加しました\n\n", lname, fname, email);
}

void DeleteDialog(RECORD * record) {
	int id = 0;
	printf("情報を削除します\nIDを入力:");
	scanf("%d%*[^\n]", &id); getchar();

	Delete(record, id);
	puts("処理結果");
	DisplayAll(record);
	puts("");
}

void FindDialog(RECORD * record) {
	char word[64];
	printf("情報を検索します(完全一致)\nキーワード:");
	scanf("%63[^\n]%*[^\n]", word); getchar();

	Find(record, word);
	puts("");
}

void SortDialog(RECORD * record) {
	int item = 0, isAsc = 1;
	printf("情報をソートして表示します\n");
	printf("ソートする項目を入力\n0:姓 1:名 2:メールアドレス  :");
	scanf("%d%*[^\n]", &item); getchar();

	printf("0:降順 1:昇順  :");
	scanf("%d%*[^\n]", &isAsc); getchar();

	Sort(record, item, isAsc);
	puts("");
}

int main(void) {
	RECORD* record = ReadData();
	int mode = 4;

	printf("簡易アドレス帳\n");
	while (mode != 0) {
		printf("1:追加  2:削除  3:検索  4:ソートして表示  0:保存して終了\n");
		scanf("%d%*[^\n]", &mode); getchar();

		switch (mode)
		{
		case 1:
			AddDialog(record);
			break;
		case 2:
			DeleteDialog(record);
			break;
		case 3:
			FindDialog(record);
			break;
		case 4:
			SortDialog(record);
			break;
		default:
			break;
		}
	}
	WriteData(record);
	return 0;
}
