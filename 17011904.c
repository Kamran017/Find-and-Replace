/*
	KAMRAN BALAYEV 17011904
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>//get empty spaces with scanf
#include <time.h>//for time calculation

struct timespec begin;/* These structs will be used in time calculation*/
struct timespec end;	

void shifttable(char *);
int horspool(char *);
void replaceTextFunc(char*, char *, int);

int table[1000];//table of character shift values
char wholeText[1000];//this array stores the string read from file
int main()
{
	char findText[300], replaceText[300], textName[200], ch, i, flag[300];
	int cs, pos, counter = 0;//control for the case sensitive operation
	//get inputs from user
	printf("Please enter the text you want to find: ");
	scanf("%[^\n]s", findText);
	printf("\nPlease enter the text you want to replace with original one: ");
	getchar();//catch the new line 
	scanf("%[^\n]s", replaceText);
	printf("\nPlease enter the name of text file with .txt extension you want to open (please write .txt extension too): ");
	getchar();//catch the new line 
	gets(textName);
	printf("\nIf you want to search case sensitively please enter 1, otherwise enter 0: ");
	scanf("%d", &cs);
	//open and read file
	FILE *input = fopen(textName, "r");
	fgets(wholeText, 1000, input);//assign the string to the wholeText variable
	printf("The default version of text: \n");
	printf("%s\n", wholeText);//print the default version of string 
	//if input is null it will ask new file name from user till the file is available
	if (input == NULL)
	{
		printf("File is not available \n");
		while (input == NULL)
		{
			printf("\nPlease enter the name of text file with .txt extension you want to open (please write .txt extension too): ");
			gets(textName);
			input = fopen(textName, "r");
		}
	}
	//timer starts
	clock_gettime(CLOCK_MONOTONIC, &begin);
	//call the shift table function for calculating the table
	shifttable(findText);
	//call horspool function for finding the string value
	pos = horspool(findText);
	//if the option is case sensitive and pos value is equal to the -1 then string is not available in the text
	if ((cs == 1) && (pos == -1)) {
		printf("\nThis string is not available in this text!\n");
	}
	//if the option is not case sensitive do these operations:
	else if (cs == 0) {
		//copy the value of findText string in order to use it in capital letter case 
		strcpy(flag, findText);
		
		//if string is uppercase convert it to the lowercase
		for (i = 0; findText[i] != '\0'; i++) {
			if (findText[i] >= 'A' && findText[i] <= 'Z')
				findText[i] = findText[i] + 32;
		}
		//create the shift table
		shifttable(findText);
		//call horspool  function and find the string
		pos = horspool(findText);
		//replace all string values
		while (pos != -1) {
			replaceTextFunc(findText, replaceText, pos);
			shifttable(findText);
			pos = horspool(findText);
			counter++;
		}
		//if string is lowercase convert it to the uppercase
		for (i = 0; findText[i] != '\0'; i++) {
			if (findText[i] >= 'a' && findText[i] <= 'z')
				findText[i] = findText[i] - 32;
		}
		//create the shift table
		shifttable(findText);
		//call horspool  function and find the string
		pos = horspool(findText);
		//replace all string values
		while (pos != -1) {
			replaceTextFunc(findText, replaceText, pos);
			shifttable(findText);
			pos = horspool(findText);
			counter++;
		}

		//use the previous value of findText which was stored in flag
		strcpy(findText, flag);
		//control the capital case letter with converting first character to the capital
		if (findText[0] >= 'A' && findText[0] <= 'Z') {
			findText[0] = findText[0] + 32;
		}
		//convert the first character of string to the lowercase
		else if (findText[0] >= 'a' && findText[0] <= 'z') {
			findText[0] = findText[0] - 32;
		}
		//create the shift table
		shifttable(findText);
		//call horspool  function and find the string
		pos = horspool(findText);
		//replace all string values
		while (pos != -1) {
			replaceTextFunc(findText, replaceText, pos);
			shifttable(findText);
			pos = horspool(findText);
			counter++;
		}
		//print the replaced version of text
		printf("\nThe replaced version of text:");
		printf("\n%s\n", wholeText);
		
		printf("\nFounded and Replaced: %d", counter);
	}
	else {
		//this section is for case sensitive option
		//replace all values
		while (pos != -1) {
			replaceTextFunc(findText, replaceText, pos);
			shifttable(findText);
			pos = horspool(findText);
			counter++;
		}
		//print replaced version of text
		printf("\nThe replaced version of text:");
		printf("\n%s\n", wholeText);
		//print the counter
		printf("\nFounded and Replaced: %d", counter);
	}
	
	//timer ends
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("\nTime in nanosecond: %ld", (long int)(end.tv_sec-begin.tv_sec)*1000000000 + (end.tv_nsec-begin.tv_nsec) );
	
	fclose(input);//close the file which was opened for reading

	FILE *output = fopen(textName, "w+");//open file for writing the replaced version of text
	fputs(wholeText,output);//put string to the file

	fclose(output);//close the file which was opened for writing
	return 0;
}
/*
	Purpose of this function is replacing the finded text with the
	text which user wants (which is called replaceText). 
	Function has 3 parameters:
	First one is string wanted to be find , 
	second one is string which will be replaced with finded string 
	and the last one is the start position of finded string.
*/
void replaceTextFunc(char* findedText, char * replaceText, int pos) {
	int i, index, j, sub;
	int findLenght = strlen(findedText);//calculate the lenght of finded string
	int replaceLenght = strlen(replaceText);//calculate the lenght of replace string
	int wholeTextLength = strlen(wholeText);//calculate the whole text lenght
	//if the length of string which will be replaced with finded one is equal to the lenght of finded text string
	if (replaceLenght == findLenght) {
		//change the string 
		for (i = pos, j = 0; i < replaceLenght + pos; i++, j++) {
			wholeText[i] = replaceText[j];
		};
	}
	//if the lenght of replace text is higher than the lenght of finded text
	else if (replaceLenght > findLenght) {
		index = pos + replaceLenght;//calculate the index for shifting array 
		sub = replaceLenght - findLenght;//array will be shifted 'sub' times
		//shift array
		for (i = wholeTextLength + sub; i >= index; i--) {
			wholeText[i] = wholeText[i - sub];
		};
		//change the string
		for (i = pos, j = 0; i < replaceLenght + pos; i++, j++) {
			wholeText[i] = replaceText[j];
		};
	}
	//if the lenght of finded text is higher than the lenght of replace text
	else if (findLenght > replaceLenght) {
		index = pos + replaceLenght;//calculate the index for shifting array 
		sub = findLenght - replaceLenght;//array will be shifted 'sub' times
		//shift array
		for (i = index; i < wholeTextLength - 1; i++) {
			wholeText[i] = wholeText[i + sub];
		};
		wholeText[wholeTextLength - 1] = '\0';//assign the null value to the end of string
		//change string
		for (i = pos, j = 0; i < replaceLenght + pos; i++, j++) {
			wholeText[i] = replaceText[j];
		};
	}


}
/*
	Purpose of this function is creating 
	shift table for the characters.
	calculation formula of shift table is: length-index-1.
	It has one parameter which is finded text
*/
void shifttable(char * findText) {
	int i, j, lnth;
	lnth = strlen(findText);
	for (i = 0; i < 1000; i++)
		table[i] = lnth;
	for (j = 0; j < lnth - 1; j++)
		table[findText[j]] = lnth - 1 - j;
}
/*
	Purpose of this function is finding the string which user wants.
	It has one parameter which is findedtext. It returns -1 if the string 
	is not available in the text. Otherwise, return the start position of string.
*/
int horspool(char *findText) {
	int i, j, k, m, n;
	n = strlen(wholeText);
	m = strlen(findText);
	i = m - 1;
	while (i < n) {
		k = 0;
		while ((k < m) && (findText[m - 1 - k] == wholeText[i - k]))
			k++;
		if (k == m)
			return(i - m + 1);
		else
			i += table[wholeText[i]];
	}
	return -1;
}


