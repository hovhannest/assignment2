
#include <stdio.h>
#include <stdlib.h> //for malloc and calloc

char *buildToken(char token[]);
char *storeToken(const char *source);
char *copyToken(char *destination, const char *source);
int numberOfTokens(char *tokens, char delim);

char ** mytoc(char *str,char delim){
	int currTokens = 0;
	char **tokenVec = NULL; //vector of pointers to be returnedy
	int numOfTokens = numberOfTokens(str,delim); //total number of tokens
	tokenVec = malloc(sizeof(char*) * numOfTokens+1);//allocate memory for tokens
	int startOfString = 0;
	char character; //character of tokens currently being read will be stored in character
	int i = 0;
	character = *(str + i);//assign what is pointed to the tokens first or 0 element to character
	char *token = NULL;

	while( character != '\0'){
		//check if whole word has been read
		if( character == delim || *(str + (i+1)) == '\0'){

			int tokenRear = i+1;//set limit reached of a full word and +1 for EOF
			char temp[tokenRear];
			temp[tokenRear] = '\0'; //set last character equal to null character
			int tempI =0;  //for inserting char(s) in temp array
			int j = startOfString;//for retrieving chars from tokens string

			while(j < tokenRear){
				character = *(str+j);//store word being read
				//store character in the temp variable
				if(character != delim){
					temp[tempI] = character;
					tempI++;
				}
				j++;
				startOfString = j;
			}
			temp[tempI] ='\0';
			token = buildToken(temp);
			tokenVec[currTokens] = storeToken(token);
			currTokens++;
		}
		//increment pointer
		i++;
		character = *(str+ i);
	}
	tokenVec[currTokens] = '\0';

	return tokenVec;
}

char *buildToken(char token[]){
	char *word = NULL;
	int i = 0;
	int numOfChars = 0;
	//get total chars to allocate
	while( token[numOfChars] != '\0'){
		numOfChars++;
	}

	word = (char*)calloc(numOfChars+1, sizeof(char));
	while(token[i] != '\0'){
		*(word + i) = token[i];
		i++;
	}

	*(word + (i+1)) = '\0';//store null character
	return word;
}

//returned stored token
char *storeToken(const char *source) {
	int tokenLength = tokenLen(source);//get the lenght of the token
	//allocate memory
	char *destination = malloc ( tokenLength + 1);
	if (destination == NULL){
		return NULL;
	}
	copyToken (destination,source); //copy token onto destination

	return destination;
}

//return lenght of token
int tokenLen(const char *token){
	if(token == NULL){
		return 0;
	}
	int length = 0;
	char character;
	for(int i = 0; (character =  *(token + i)) != '\0'; i++){
		length++;
	}
	return length;
}

//copy token to be stored on vector
char *copyToken(char *destination, const char *source){
	char *temp = destination; //char destination d store in temp
	while (*source) //while there is stuff in source copy it to the destination
	{
		*destination++ = *source++;
	}

	*destination = '\0';//store null character
	return temp;
}

//return total number of tokens
int numberOfTokens(char *str, char delim){
	if (str == NULL)
		return 0;

	int numOfTokens = 0; //num of tokens contained in the input string
	for(;*str != '\0';str++) //for-loop will stop once the null character is reached
	{
		if(*str == delim)//check wheter the delim character has been found or the last token has been reached
			numOfTokens++;
	}
	return numOfTokens+1;
}
