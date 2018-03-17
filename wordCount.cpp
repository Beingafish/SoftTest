#include<iostream>
#include<cstring>
#include<vector>
using namespace std;

#define	MAX_COM_LENGTH	50
#define	MAX_PATH_LENGTH	50

struct Command{
	bool _c;		//�Ƿ�ͳ���ַ���
	bool _w;		//��ͳ�Ƶ�������
	bool _l;		//�Ƿ�ͳ��������
	bool _o;		//�Ƿ񽫽�������ָ���ļ�
	bool _s;		//�Ƿ�ݹ鴦��Ŀ¼�·��������������ļ�
	bool _a;		//�Ƿ�ͳ�ƴ�����/����/ע����
	bool _e;		//�Ƿ���ͣ�ôʱ�
	char filePath[MAX_PATH_LENGTH];
	char outFile[MAX_PATH_LENGTH];
	char stopFile[MAX_PATH_LENGTH];
	Command(){
		_c=false;
		_w=false;
		_l=false;
		_o=false;
		_s=false;
		_a=false;
		_e=false;
		strcpy(filePath,"");
		strcpy(outFile,"");
		strcpy(stopFile,"");
	}
};
struct SourceFile{
	char filePath[MAX_PATH_LENGTH];
	char fileName[MAX_PATH_LENGTH];
	int charNum;
	int wordNum;
	int lineNum;
	SourceFile *next;
	SourceFile(){
		strcpy(filePath,"");		//·������Ѱ���ļ�����������ļ��� 
		strcpy(fileName,"");		//�ļ������ڽ���ͨ���ƥ�� 
		charNum=0;
		wordNum=0;
		lineNum=0;
		next=NULL;
	}
};

void mainLoop();
void analyseCommand(char commandStr[], Command &command);
void getFileName(char filePath[], SourceFile *head);
void wordCount(SourceFile *head, char stopPath[]);
void wordCount(SourceFile *sourceFile, vector<string> &stopWords);
void outPut(SourceFile *head, Command &command);

int main(){
	mainLoop();
	return 0;	
}

void mainLoop(){
	/*ʹ��whileѭ������ִ��cin.
	cin֮������û�ָ������ݽ����û�ָ��Ľ��ִ�к������ܡ�
	���cin�õ����ǿ��ַ��������˳���ѭ����*/
	char commandStr[MAX_COM_LENGTH] = "";
	while(true){
		gets(commandStr);
		if(strcmp(commandStr,"")==0) break;
		Command command;
		analyseCommand(commandStr,command);
		
	}
	
}

void analyseCommand(char commandStr[], Command &command){
	/*����һ���ַ�����һ��ָ��ṹ������á� 
	�ڸýṹ���ڴ洢���������*/
	for(int i=0;true;i++){
		char c=commandStr[i];
		if(c==0) return;
		if(c=='-'){
			i++;
			c=commandStr[i];
			if(c=='c') {command._c=true; continue;}
			else if(c=='w') {command._w=true; continue;}
			else if(c=='l') {command._l=true; continue;}
			else if(c=='s') {command._s=true; continue;}
			else if(c=='a') {command._a=true; continue;}
			else if(c=='e') command._e=true;
			else if(c=='o') command._o=true;
			else continue;	
			i+=2;	//����һ���ո��Ժ�ʼ��ȡ�ļ�·��
					//���i++���ֻ��-e��-o��ִ�е� 
		}
		if(c==' ') continue;
		
		char path[MAX_PATH_LENGTH]="";
		for(int j=0;true;j++){
			char ch=commandStr[i];
			if(ch==0){i--; break;}
			/*���ڴ�ѭ��ÿ��i++�����ﵽ������ĩβ
			����Ҫ��i--�����ó����жϳ�����������ĩβ*/ 
			if(ch==' ') break;
			path[j]=ch;
			i++;
		}
		if(c=='e') strcpy(command.stopFile, path);
		else if(c=='o') strcpy(command.outFile, path);
		else strcpy(command.filePath, path);
	}
	return;
}

void getFileName(char filePath[], SourceFile *head){
	//�����ļ�·����ͷָ�룬�õ�ָ��·�������з����������ļ����� 
	return;
}

void wordCount(SourceFile *head, char stopPath[]){
	/*���ȱ���ͣ�ôʱ�����ͣ�ôʽṹ�塣 
	����ʹ��ÿһ��Դ�ļ��ṹ���ͣ�ô��������о����ͳ�ơ�*/
}

void wordCount(SourceFile *sourceFile, vector<string> &stopWords){
	//�����ļ����ݣ�����ͳ�ƣ��������������sourceFile�� 
	return;
}

void outPut(SourceFile *head, Command &command){
	/*�����ļ��ṹ���ͷָ���ָ��ṹ������ã����ΰ���ʽ��������
	�����ַ�'����'����'��������/������/ע���е�˳�����η�����ʾ��*/
}

