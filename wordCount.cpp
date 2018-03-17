#include<iostream>
#include<cstring>
#include<vector>
using namespace std;

#define	MAX_COM_LENGTH	50
#define	MAX_PATH_LENGTH	80

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
	~SourceFile(){
		delete next;
	}
};

void mainLoop();
void analyseCommand(char commandStr[], Command &command);
void getFileName(char filePath[], SourceFile *head);
void wordCount(SourceFile *head, char stopPath[]);
void wordCount(SourceFile *sourceFile, vector<string> &stopWords);
void outPut(SourceFile *head, Command &command);

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
		
		SourceFile *head=new SourceFile();
		
		Command command;
		analyseCommand(commandStr,command);
		
		getFileName(command.filePath, head);
		
		SourceFile *p=head->next;
		while(p!=NULL){
			cout<<p->fileName<<endl;
			cout<<p->filePath<<endl<<endl;
			p=p->next;
		}
		
		delete head;
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
	char   buffer[MAX_PATH_LENGTH];   
	getcwd(buffer, MAX_PATH_LENGTH); 
	string path;
	path.assign(buffer).append("\\");
	//��ȡ��·���µ������ļ�  
	getFiles(path,"", head,filePath);
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
                        while (*src && (!MATCH_CHAR(*src,*pattern,ignore_case)))
                              src++;
                        
                        /* ����Ҳ�������ƥ��ʧ�� */        
                        if (!*src) return NOT_MATCH;

                        /* ����ҵ��ˣ�ƥ��ʣ�µ��ַ���*/
                        result = WildCharMatch (src, pattern, ignore_case);
                        /* ���ʣ�µ��ַ���ƥ�䲻�ϣ���src��һ���ַ�����pattern��'*"���һ���ַ� */
                        /* srcǰ��һλ������ƥ�� */
                        while ( (!result) && (*(src+1)) && MATCH_CHAR(*(src+1),*pattern,ignore_case) )
                           result = WildCharMatch (++src, pattern, ignore_case);

                        return result;

                    }
                else
                    {
                     /* ���pattern�е�ǰ�ַ����� '*' */
                     /* ƥ�䵱ǰ�ַ�*/
                        if ( MATCH_CHAR(*src,*pattern,ignore_case) || ('?' == *pattern))
                          { 
                            /* src,pattern�ֱ�ǰ��һλ������ƥ�� */
                            return WildCharMatch (++src, ++pattern, ignore_case);
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
           if ( (*pattern=='*') && (*(pattern+1)==0) ) /* ���pattern�����һλ�ַ�����'*' */
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
    long   hFile   =   0;  
    //�ļ���Ϣ  
    struct _finddata_t fileinfo;  
    string p,p2;  
    if((hFile = _findfirst(p.assign(path).append(path2).append("*").c_str(),&fileinfo)) !=  -1)  
    {  
        do  
        {  
            //�����Ŀ¼,����֮  
            //�������,�����б�  
            if((fileinfo.attrib &  _A_SUBDIR))  
            {  
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
                    getFiles( p.assign(path).append("\\"),p2.assign(fileinfo.name).append("\\"), head,pattern);  
            }  
            else  
            {
				char fileName[MAX_PATH_LENGTH];
				strcpy(fileName,p.assign(fileinfo.name).c_str());
				char filePath[MAX_PATH_LENGTH];
				strcpy(filePath,p.assign(path2).append(fileinfo.name).c_str());
				
				if(WildCharMatch(fileName, pattern, 1)||WildCharMatch(filePath, pattern, 1)){
					SourceFile *pF = new SourceFile();
					pF->next=head->next;
					head->next=pF;
					strcpy(pF->fileName,fileName);
					strcpy(pF->filePath,filePath);
				}
				
            }  
        }while(_findnext(hFile, &fileinfo)  == 0);  
        _findclose(hFile);  
    }  
}
