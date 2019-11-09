#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAXCHAR 10000
#define MAXLINE 1000
void calLPS(char* pat, int M, int* lps); 
int count;
char fullText[10000];
int KMPStringMatch(char* pat, char* txt) 
{ 
    int M = strlen(pat); 
    int N = strlen(txt); 
  
    int lps[M]; 
  
    
    calLPS(pat, M, lps); 
  
    int i = 0;  
    int j = 0;  
    while (i < N) { 
        if (pat[j] == txt[i]) { 
            j++; 
            i++; 
        } 
        if (j == M) { 
            return 1;
        } 
        else if (i < N && pat[j] != txt[i]) { 
            if (j != 0) 
                j = lps[j - 1]; 
            else
                i = i + 1; 
        } 
    }
    return 0; 
} 
void calLPS(char* pat, int M, int* lps) 
{ 
    int len = 0; 
    lps[0] = 0; 
    int i = 1; 
    while (i < M) { 
        if (pat[i] == pat[len]) { 
            len++; 
            lps[i] = len; 
            i++; 
        } 
        else 
        {  
            if (len != 0) { 
                len = lps[len - 1]; 
            } 
            else 
            { 
                lps[i] = 0; 
                i++; 
            } 
        } 
    } 
} 
void readInputText(){
    printf("Welcome to the plagarism detector\n");
    printf("Enter the location of the file to be checked\n");
    char location[100];
    scanf("%[^\n]%*c",location);
    FILE* fp = fopen(location, "r");
    if(fp != NULL){
            fseek(fp, 0, SEEK_END);
            long fsize = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            fread(fullText, 1, fsize, fp);
            //printf("%s",fullText);
    }
    else{
        printf("Enter valid file location");
    }
}
char* readString(int start,int end,char* sourceText){
    char* pat = (char*)malloc(sizeof(char)*MAXCHAR);
    int k=0;
    for(int i=start;i<end;i++){
        pat[k] = sourceText[i];
        k+=1;
    }
    pat[k] = '\0';
    return pat;
}
double KMPscore(FILE* fp,char* sourceText){
    double score = 0;
    int length = strlen(sourceText);
    int lineNo = 1;
    int lineNoArray[MAXLINE];
    char* pat;
    int k=0;
    for(int i=0;i<length;i++){
        if(sourceText[i] == '\n'){
            lineNo += 1;
            lineNoArray[k] = i;
            k+=1;
        }
        else if(i==length-1){
            lineNoArray[k] = i+1;
            k+=1;
        }
        
    }
    int oldAmount;
    int offval ;
    int res;
    for(int i =0;i<k;i++){
        oldAmount = 0;
        if(i==0){
            offval = 0;
        }
        else{
            offval = lineNoArray[i-1]+1;
            oldAmount = lineNoArray[i-1]+1;
        }
        for(int j=i;j<k;j++){
            pat = readString(offval,lineNoArray[j],sourceText);
            res = KMPStringMatch(pat,fullText);
            if(res){
                score += pow(2,j-i+1);
            }
        }
    }
    
    return score;
   
}
int plagarismCheck(FILE* fp,char* sourceText){
    double threshold = 200;
    double score;
    score = KMPscore(fp,sourceText);
    //printf("The score is %f\n",score);
    if(score > threshold){
        return 1;
    }
    else{
        return 0;
    }
    
}
void printPlagerDoc(int* plagerDoc,int n){
	int k=0;
    printf("The sources from which it was plagarised are : \n");
    for(int i=0;i<n;i++){
        if(plagerDoc[i] == 1){
            printf("Source %d\n",i+1);
            k=1;
        }
    }
    if(k==0)
    {
    	printf("No such files found\n");
    	printf("Document not plagiarised\n");
    }
}
void readSource(int n){
    
    int choice;
    char sourceLocation[1000];
    int res;
    int *plagerDoc =(int*)calloc(sizeof(int),n);
    for(int i=0;i<n;i++){
        printf("Enter location of source %d\n",i+1);
        scanf(" %[^\n]%*c",sourceLocation);
        FILE* fp = fopen(sourceLocation, "r");
        char sourceText[MAXCHAR];
        fseek(fp, 0, SEEK_END);
        long fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        fread(sourceText, 1, fsize, fp);
        fseek(fp, 0, SEEK_SET);
        res = plagarismCheck(fp,sourceText);
        plagerDoc[i] = res;
    }
    printPlagerDoc(plagerDoc,n);
}
int main() {
    readInputText();
    printf("Enter the number of source doucments \n");
    int n;
    scanf("%d",&n);
    readSource(n);
    return 0;
}