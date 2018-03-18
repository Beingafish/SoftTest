#include<iostream>
#include<cstring>
#include<vector>
#include <fstream>
#include<sstream>
using namespace std;

#define	MAX_COM_LENGTH			150
#define MAX_WORD_LENGTH			100
#define	MAX_PATH_LENGTH			80
#define MAX_STOPWORD_LENGTH		20

struct Command {
	bool _c;		//�Ƿ�ͳ���ַ���
	bool _w;		//��ͳ�Ƶ�������
	bool _l;		//�Ƿ�ͳ��������
	bool _o;		//�Ƿ񽫽�������ָ���ļ�
	bool _s;		//�Ƿ�ݹ鴦��Ŀ¼�·��������������ļ�
	bool _a;		//�Ƿ�ͳ�ƴ�����/����/ע����
	bool _e;		//�Ƿ���ͣ�ôʱ�
	char filePath[MAX_PATH_LENGTH];		//�ļ�·�� 
	char outFile[MAX_PATH_LENGTH];		//������·�� 
	char stopFile[MAX_PATH_LENGTH];		//ͣ�ô�·�� 
	Command() {
		_c = false;
		_w = false;
		_l = false;
		_o = false;
		_s = false;
		_a = false;
		_e = false;
		strcpy(filePath, "");
		strcpy(outFile, "");
		strcpy(stopFile, "");
	}
};
struct SourceFile {
	char filePath[MAX_PATH_LENGTH];
	char fileName[MAX_PATH_LENGTH];
	int charNum;
	int wordNum;
	int lineNum;
	int blankLineNum;
	int codeLineNum;
	int noteLineNum;
	SourceFile *next;
	SourceFile() {
		strcpy(filePath, "");		//·������Ѱ���ļ�����������ļ��� 
		strcpy(fileName, "");		//�ļ������ڽ���ͨ���ƥ�� 
		charNum = 0;
		wordNum = 0;
		lineNum = 1;
		blankLineNum = 0;
		codeLineNum = 0;
		noteLineNum = 0;
		next = NULL;
	}
	~SourceFile() {
		delete next;
	}
};
struct StopWord {
	char word[MAX_STOPWORD_LENGTH];
	StopWord *next;
	StopWord() {
		strcpy(word, "");
		next = NULL;
	}
	~StopWord() {
		delete next;
	}
};

void mainLoop();											//������ѭ�� 
void analyseCommand(char commandStr[], Command &command);	//�����û�ָ�� 
void getFileName(char path[], SourceFile *head);			//�ݹ�õ�Ŀ¼�������ļ� 
void wordCount(SourceFile *head, char stopPath[]);			//����ͳ�Ƶ�Ԥ������ 
void wordCount(SourceFile *sourceFile, StopWord *head);		//����ͳ�� 
void outPut(SourceFile *head, Command &command);			//���ı���� 

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����Ϊ�ݹ����Ŀ¼���� 
#include<io.h>
void getFiles(string path, string path2, SourceFile *head, char* pattern);
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����Ϊ���õ��ַ���ƥ�䣨��ͨ��������� 
#include <ctype.h>
/** Defines and Macros */
#define MATCH      1
#define NOT_MATCH  0
/* ƥ��һ���ַ��ĺ� */
#define MATCH_CHAR(c1,c2,ignore_case)  ( (c1==c2) || ((ignore_case==1) &&(tolower(c1)==tolower(c2))) )
int WildCharMatch(char *src, char *pattern, int ignore_case);
////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
	mainLoop();
	return 0;
}

void mainLoop() {
	/*ʹ��whileѭ������ִ��cin.
	cin֮������û�ָ������ݽ����û�ָ��Ľ��ִ�к������ܡ�
	���cin�õ����ǿ��ַ��������˳���ѭ����*/
	char commandStr[MAX_COM_LENGTH] = "";
	while (true) {											//ѭ�������û����� 
		gets(commandStr);									//����û�ָ�� 
		if (strcmp(commandStr, "") == 0) break;					//����ǿ�ָ�������� 

		Command command;
		analyseCommand(commandStr, command);					//�����û�ָ�� 

		SourceFile *head = new SourceFile();
		if (command._s) getFileName(command.filePath, head);	//�ݹ�Ѱ��Ŀ¼�µ��ļ� 
		else {												//����ֱ���������·�������ļ� 
			SourceFile *p = new SourceFile();
			p->next = head->next;
			head->next = p;
			strcpy(p->fileName, command.filePath);
			strcpy(p->filePath, command.filePath);
		}

		wordCount(head, command.stopFile);					//ͳ�Ƶ����� 

		outPut(head, command);								//���������ļ� 

		delete head;
	}

}

void analyseCommand(char commandStr[], Command &command) {
	/*����һ���ַ�����һ��ָ��ṹ������á�
	�ڸýṹ���ڴ洢���������*/
	for (int i = 0; true; i++) {
		char c = commandStr[i];
		if (c == 0) return;
		if (c == '-') {	//����'-'֮��������ȡ��һ���ַ���Ȼ���ж��Ǻ���ָ��ж��Ƿ���Ҫ������ȡ�ַ��� 
			i++;
			c = commandStr[i];
			if (c == 'c') { command._c = true; continue; }
			else if (c == 'w') { command._w = true; continue; }
			else if (c == 'l') { command._l = true; continue; }
			else if (c == 's') { command._s = true; continue; }
			else if (c == 'a') { command._a = true; continue; }
			else if (c == 'e') command._e = true;
			else if (c == 'o') command._o = true;
			else continue;
			i += 2;	//����һ���ո��Ժ�ʼ��ȡ�ļ�·��
					//���i++���ֻ��-e��-o��ִ�е� 
		}
		if (c == ' ') continue;

		char path[MAX_PATH_LENGTH] = "";
		for (int j = 0; true; j++) {
			char ch = commandStr[i];
			if (ch == 0) { i--; break; }
			/*���ڴ�ѭ��ÿ��i++�����ﵽ������ĩβ
			����Ҫ��i--�����ó����жϳ�����������ĩβ*/
			if (ch == ' ') break;
			path[j] = ch;
			i++;
		}
		if (c == 'e') strcpy(command.stopFile, path);
		else if (c == 'o') strcpy(command.outFile, path);
		else strcpy(command.filePath, path);
	}
	return;
}

void getFileName(char path[], SourceFile *head) {
	//�����ļ�·����ͷָ�룬�õ�ָ��·�������з����������ļ�����
	char filePath[MAX_PATH_LENGTH] = "";
	char fileName[MAX_PATH_LENGTH] = "";
	strcpy(filePath, path);
	for (int i = MAX_PATH_LENGTH - 1; i >= 0; i--) {
		if (filePath[i] == '\\') {	//�Ӻ���ǰѰ�ҵ�һ��'\\'�Էָ��ļ�Ŀ¼���ļ��� 
			i++;
			int j = 0;
			for (; filePath[i + j] != 0; j++) {
				fileName[j] = filePath[i + j];
			}
			fileName[j] = 0;
			filePath[i] = 0;
			break;
		}
	}
	string p;
	p.assign(filePath);
	//��ȡ��·���µ������ļ���������SourceFile������ 
	getFiles(p, "", head, fileName);
	return;
}

void wordCount(SourceFile *head, char stopPath[]) {
	/*���ȱ���ͣ�ôʱ�����ͣ�ôʽṹ�塣
	����ʹ��ÿһ��Դ�ļ��ṹ���ͣ�ô�������о����ͳ�ơ�*/
	StopWord *sHead = new StopWord();

	ifstream in(stopPath);
	string line, word;
	if (in)
	{
		while (getline(in, line))
		{   //��ȡͣ�ô�ʱ���������ж�ȡ���𵥴ʶ�ȡ�ķ��� 
			istringstream istrm(line);
			while (istrm >> word) {
				StopWord *pS = new StopWord();
				pS->next = sHead->next;
				strcpy(pS->word, word.c_str());
				sHead->next = pS;
			}
		}
	}
	in.close();

	SourceFile *p = head->next;
	while (p != NULL) {
		wordCount(p, sHead);
		p = p->next;
	}

	delete sHead;
}

void wordCount(SourceFile *sourceFile, StopWord *head) {
	//�����ļ����ݣ�����ͳ�ƣ��������������sourceFile��
	char c;
	ifstream in;
	in.open(sourceFile->filePath);

	bool wordFlag = false;					//�Ƿ��ڵ����ڲ� 
	bool longNote = false;					//�Ƿ��ڳ���ע�� 
	int state = 1;							//��ǰ�е�״̬ 
	bool hasPassState2 = false;				//��ǰ���Ѳ����ܳ�Ϊ���� 
	char currentWord[MAX_WORD_LENGTH] = "";	//��¼��ǰ�ĵ��� 
	int wordPosition = 0;						//��¼��ǰ�α괦�ڵ��ʵĵڼ�����ĸ 


	while (true)
	{
		c = in.get();
		if (c == EOF) {
			//���ļ���β������Ҫ�Ե������������Ƚ������Ľ��� 
			if (wordFlag) {
				sourceFile->wordNum++;
			}
			if (state == 1) {
				if (hasPassState2) sourceFile->noteLineNum++;
				else sourceFile->blankLineNum++;
			}
			if (state == 2) {
				if (hasPassState2) sourceFile->noteLineNum++;
				else sourceFile->blankLineNum++;
			}
			if (state == 3) sourceFile->codeLineNum++;
			if (state == 5) {
				if (hasPassState2) sourceFile->codeLineNum++;
				else sourceFile->blankLineNum++;
			}
			if (state == 6 || state == 7 || state == 8) sourceFile->noteLineNum++;

			if (strcmp(currentWord, "") != 0) {
				StopWord *pH = head->next;
				while (pH != NULL) {
					if (strcmp(currentWord, pH->word) == 0) {
						sourceFile->wordNum--;
						break;
					}
					pH = pH->next;
				}
			}
			break;
		}

		//�ַ��� 
		if (c != '\n') sourceFile->charNum++;
		//������ 
		bool separator = (c == ' ' || c == ',' || c == '\n' || c == '\t');	//���������ʾ��ǰ�ַ��Ƿ����ڷָ��� 
		if (wordFlag&&separator) sourceFile->wordNum++;		//����ַ��ɵ������ַ���Ϊ�ָ�����˵��������+1 
		wordFlag = !separator;
		if (wordFlag) {
			currentWord[wordPosition] = c;
			wordPosition++;
			currentWord[wordPosition] = 0;					//��ʱ�����ַ���ֹͣ��־ 
		}
		if (!wordFlag) {										//��һ�����ʶ����Ժ�Ҫ�ж�ͣ�ô� 
			wordPosition = 0;
			if (strcmp(currentWord, "") != 0) {
				StopWord *pH = head->next;
				while (pH != NULL) {							//��һ����ͣ�ôʱ�����ж� 
					if (strcmp(currentWord, pH->word) == 0) {
						sourceFile->wordNum--;				//�������ͣ�ôʣ���������Ҫ-1 
						break;
					}
					pH = pH->next;
				}
			}
			currentWord[wordPosition] = 0;					//��������Ϊ�˷�ֹ�ظ��ж�ͣ�ô� 
		}

		//�����������������С�ע���С����� 
		//����״̬Ǩ��ģ�ͣ���δ���̫���ˣ����ܻ���ʵ���� 
		if (state == 1) {
			if (c == '\n') {
				sourceFile->lineNum++;
				if (hasPassState2) sourceFile->noteLineNum++;
				else sourceFile->blankLineNum++;
				hasPassState2 = false;
			}
			else if (c == '/') state = 5;
			else if (c != ' '&&c != '\t') { state = 2; }
		}
		else if (state == 2) {
			if (c == '\n') {
				state = 1;
				sourceFile->lineNum++;
				if (hasPassState2) sourceFile->noteLineNum++;
				else sourceFile->blankLineNum++;
				hasPassState2 = false;
			}
			else if (c == '/') {
				state = 5;
				hasPassState2 = true;
			}
			else if (c != ' '&&c != '\t') {
				state = 3;
				hasPassState2 = true;
			}
		}
		else if (state == 3) {		//������ʵ��һЩ���⣬�Ժ��ٸ� 
			if (c == '\n') {
				state = 1;
				sourceFile->lineNum++;
				sourceFile->codeLineNum++;
				hasPassState2 = false;
			}
		}
		else if (state == 5) {
			if (c == '\n') {
				state = 1;
				sourceFile->lineNum++;
				if (hasPassState2) sourceFile->codeLineNum++;
				else sourceFile->blankLineNum++;
				hasPassState2 = false;
			}
			else if (c == '/')state = 6;
			else if (c == '*') {
				state = 7;
				longNote = true;
			}
			else state = 3;
		}
		else if (state == 6) {
			if (c == '\n') {
				state = 1;
				sourceFile->lineNum++;
				sourceFile->noteLineNum++;
				hasPassState2 = false;
			}
		}
		else if (state == 7) {
			if (c == '\n') {
				state = 7;
				sourceFile->lineNum++;
				sourceFile->noteLineNum++;
				hasPassState2 = false;
			}
			else if (c == '*') state = 8;
		}
		else if (state == 8) {
			if (c == '\n') {
				state = 7;
				sourceFile->lineNum++;
				sourceFile->noteLineNum++;
				hasPassState2 = false;
			}
			else if (c == '*') state = 8;
			else if (c == '/') {
				state = 1;
				longNote = false;
				hasPassState2 = true;
			}
			else state = 7;
		}
	}
	in.close();
	return;
}

void outPut(SourceFile *head, Command &command) {
	/*�����ļ��ṹ���ͷָ���ָ��ṹ������ã����ΰ���ʽ��������
	�����ַ�'����'����'��������/������/ע���е�˳�����η�����ʾ��*/

	ofstream outf(command.outFile);
	streambuf *default_buf = cout.rdbuf();
	if(command._o) cout.rdbuf(outf.rdbuf());		//����ض���

	SourceFile *p = head->next;
	while (p != NULL) {
		//�ϸ������������˳�������� 
		if (command._c)
			cout << p->filePath << ", �ַ���: " << p->charNum << endl;
		if (command._w)
			cout << p->filePath << ", ������: " << p->wordNum << endl;
		if (command._l)
			cout << p->filePath << ", ����: " << p->lineNum << endl;
		if (command._a) {
			cout << p->filePath << ", ������/����/ע����: ";
			cout << p->codeLineNum << '/';
			cout << p->blankLineNum << '/';
			cout << p->noteLineNum << endl;
		}
		p = p->next;
	}

	//�ָ�coutĬ����� 
	cout.rdbuf(default_buf);
}

int WildCharMatch(char *src, char *pattern, int ignore_case)
{
	int result;

	while (*src)
	{
		if (*pattern == '*')
		{   /* ��� pattern �ĵ�ǰ�ַ��� '*' */
		 /* ��������ж�� '*', ���� */
			while ((*pattern == '*') || (*pattern == '?'))
				pattern++;

			/* ��� '*" ��û���ַ��ˣ�����ȷƥ�� */
			if (!*pattern) return MATCH;

			/* �� src �в���һ���� pattern��'*"���һ���ַ���ͬ���ַ�*/
			while (*src && (!MATCH_CHAR(*src, *pattern, ignore_case)))
				src++;

			/* ����Ҳ�������ƥ��ʧ�� */
			if (!*src) return NOT_MATCH;

			/* ����ҵ��ˣ�ƥ��ʣ�µ��ַ���*/
			result = WildCharMatch(src, pattern, ignore_case);
			/* ���ʣ�µ��ַ���ƥ�䲻�ϣ���src��һ���ַ�����pattern��'*"���һ���ַ� */
			/* srcǰ��һλ������ƥ�� */
			while ((!result) && (*(src + 1)) && MATCH_CHAR(*(src + 1), *pattern, ignore_case))
				result = WildCharMatch(++src, pattern, ignore_case);

			return result;

		}
		else
		{
			/* ���pattern�е�ǰ�ַ����� '*' */
			/* ƥ�䵱ǰ�ַ�*/
			if (MATCH_CHAR(*src, *pattern, ignore_case) || ('?' == *pattern))
			{
				/* src,pattern�ֱ�ǰ��һλ������ƥ�� */
				return WildCharMatch(++src, ++pattern, ignore_case);
			}
			else
			{
				return NOT_MATCH;
			}
		}
	}


	/* ���src�����ˣ���pattern�з����*/
	if (*pattern)
	{
		/* patternû�н���*/
		if ((*pattern == '*') && (*(pattern + 1) == 0)) /* ���pattern�����һλ�ַ�����'*' */
			return MATCH;
		else
			return NOT_MATCH;
	}
	else
		return MATCH;
}

void getFiles(string path, string path2, SourceFile *head, char* pattern)
{
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p, p2;
	if ((hFile = _findfirst(p.assign(path).append(path2).append("*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮  
			//�������,�����б�  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\"), p2.assign(fileinfo.name).append("\\"), head, pattern);
			}
			else
			{
				char fileName[MAX_PATH_LENGTH];
				strcpy(fileName, p.assign(fileinfo.name).c_str());
				char filePath[MAX_PATH_LENGTH];
				strcpy(filePath, p.assign(path2).append(fileinfo.name).c_str());

				if (WildCharMatch(filePath, pattern, 1)) {
					SourceFile *pF = new SourceFile();
					pF->next = head->next;
					head->next = pF;
					strcpy(pF->fileName, fileName);
					strcpy(pF->filePath, filePath);
				}

			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
