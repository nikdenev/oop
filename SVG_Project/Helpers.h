#pragma once

#include "Figure.h"

Figure* getFigureFromSVG_String(char* buffer);

Figure* getFigureFromConsoleString(char* buffer);

const char* ignoreCharacter(const char* buffer, char ch);

const char* ignoreWhiteCharacters(const char* buffer);

bool findAndZeroingCharacter(char* buffer, char*& afterCharacter, char ch);

bool findAndZeroingWhiteCharacter(char* buffer, char*& afterCharacter);

char* strStr(const char* Where, const char* what, int& lengthToSVG_Tag);

int getNumberOfCharachters(const char* str, char ch);

void takeWordsFromString(char* buffer, char**& data, int& count);

void takeAttributeFromString(char* buffer, char*& value);

const char* colorToString(Color clr);

Color stringToColor(const char* str);

int stringToNumber(const char* str);

void numberToString(char* const& str,int number);

void stringIsValidSVG_File(const char* fileString);

void onlyWhiteCharacters(const char* str, int oldLinesCount);

bool isSVG_CloseTag(const char* tag, const char* afterTag);

bool isCommentTag(const char* tag);

bool isUnknownTag(char*&beforeTag, char* tag, char* afterTag, int& linesCount);