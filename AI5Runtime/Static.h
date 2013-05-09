#pragma once

#include <vector>

typedef unsigned char STATIC_TYPE;

const STATIC_TYPE STATIC_STRING = 1;
const STATIC_TYPE STATIC_BINARY = 2;
const STATIC_TYPE STATIC_NAME = 3;


struct STATIC_DATA
{
	STATIC_TYPE type;
	size_t data_length;
	void* data;
};



void LoadStatics(unsigned char* data,size_t length,std::vector<STATIC_DATA*>* out);

