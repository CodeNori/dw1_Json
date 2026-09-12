#include "JsonFileBase.h"
#include <stdlib.h>
#include <stdio.h>

#define PRINT_tab(level) for (int i = 0; i < level; ++i)	printf("    ");

namespace dw1
{

	int token_copy_int(char* src, int cnt)
	{
		char old = src[cnt];
		src[cnt] = '\0';
		int re = atoi(src);
		src[cnt] = old;
		return re;
	}

	float token_copy_float(char* src, int cnt)
	{
		char old = src[cnt];
		src[cnt] = '\0';
		float re = (float)atof(src);
		src[cnt] = old;
		return re;
	}

	char* token_copy_string(char* src, int cnt)
	{
		src[cnt] = '\0';
		return src;
	}

	bool jsonLexer::skipWhiteSpace(jsonToken& token)
	{
		token.type = jsonToken::WhiteSpace;
		token.start = tok;
		token.size = 1;

		while (*tok == ' ' ||
			*tok == '\n' ||
			*tok == '\r' ||
			*tok == '\t')
		{
			++tok;
			if (tok >= tok_end)
				return false;
		}

		token.size = tok - token.start;
		return true;
	}

	bool jsonLexer::parsingString(jsonToken& token)
	{
		if (*tok == '"') {
			++tok;
			token.type = jsonToken::String;
			token.start = tok;
			while (*tok != '"') {
				++tok;
				if (tok >= tok_end) return false;
			}
			token.size = tok - token.start;
			*tok = '\0';

			++tok;
			return true;
		}
		return false;
	}

	bool jsonLexer::parsingNumber(jsonToken& token)
	{
		token.type = jsonToken::Number;
		token.start = tok;
		token.size = 1;

		if (*tok == '+' || *tok == '-') ++tok;

		while (*tok >= '0' && *tok <= '9') {
			++tok;
			if (tok >= tok_end) return false;
		}
		if (*tok == '.') {
			++tok;
			if (tok >= tok_end) return false;

			while ((*tok >= '0' && *tok <= '9')) {
				++tok;
				if (tok >= tok_end) return false;
			}
			if (*tok == 'f') ++tok;

			token.type = jsonToken::Float;
			token.size = tok - token.start;

			// char old = *tok;
			// *tok = '\0';
			// token.val_float = (float)atof(token.start);
			// *tok = old;
		}
		else {
			token.size = tok - token.start;

			// char old = *tok;
			// *tok = '\0';
			// token.val_int = atoi(token.start);
			// *tok = old;
		}

		return true;
	}

	bool jsonLexer::getToken(jsonToken& token)
	{
		if (tok >= tok_end) return false;

		token.type = jsonToken::Null;
		token.start = nullptr;
		token.size = 0;
		token.val_obj = nullptr;

		switch (*tok)
		{
		case '[': {
			token.type = jsonToken::Bracket0;
			token.start = tok;
			token.size = 1;
			++tok;
		}return true;

		case ']': {
			token.type = jsonToken::Bracket1;
			token.start = tok;
			token.size = 1;
			++tok;
		}return true;

		case '{': {
			token.type = jsonToken::Brace0;
			token.start = tok;
			token.size = 1;
			++tok;
		}return true;

		case '}': {
			token.type = jsonToken::Brace1;
			token.start = tok;
			token.size = 1;
			++tok;
		}return true;

		case ':': {
			token.type = jsonToken::Colon;
			token.start = tok;
			token.size = 1;
			++tok;
		}return true;

		case ',': {
			token.type = jsonToken::Comma;
			token.start = tok;
			token.size = 1;
			++tok;
		}return true;

		case 'n': {
			if (tok[1] == 'u' && tok[2] == 'l' && tok[3] == 'l') {
				token.type = jsonToken::Null;
				token.start = tok;
				token.size = 4;
				// token.val_int = 0;
				tok += 4;
				return true;
			}
		}break;
		case 't': {
			if (tok[1] == 'r' && tok[2] == 'u' && tok[3] == 'e') {
				token.type = jsonToken::True;
				token.start = tok;
				token.size = 4;
				// token.val_bool = true;
				tok += 4;
				return true;
			}
		}break;
		case 'f': {
			if (tok[1] == 'a' && tok[2] == 'l' && tok[3] == 's' && tok[4] == 'e') {
				token.type = jsonToken::False;
				token.start = tok;
				token.size = 5;
				// token.val_bool = false;

				tok += 5;
				return true;
			}
		}break;

		case '+':
		case '-':
			if (tok[1] >= '0' && tok[1] <= '9')
				return parsingNumber(token);
			else
				return false;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return parsingNumber(token);

		case '"':
			return parsingString(token);

		case ' ':
		case '\n':
		case '\r':
		case '\t':
			return skipWhiteSpace(token);
		}

		return false;
	}

	void jsonToken::Print()
	{
		switch (type)
		{
		case jsonToken::String: printf("(string) %.*s\n", size, start);
			break;
		case jsonToken::Number: printf("(number) %.*s,\n", size, start);
			break;
		case jsonToken::Float: printf("(number) %.*s,\n", size, start);
			break;
		case jsonToken::Null: printf("(null)\n");
			break;
		case jsonToken::True: printf("(true)\n");
			break;
		case jsonToken::False: printf("(false)\n");
			break;

		case jsonToken::WhiteSpace: printf("(space)\n");
			break;
		case jsonToken::Colon: printf("  :\n");
			break;
		case jsonToken::Comma: printf("  ,\n");
			break;
		case jsonToken::Brace0: printf("  {\n");
			break;
		case jsonToken::Brace1: printf("  }\n");
			break;
		case jsonToken::Bracket0: printf("  [\n");
			break;
		case jsonToken::Bracket1: printf("  ]\n");
			break;
		}
	}

	jsonObject::~jsonObject()
	{
		for (auto& kv : mKeyValueList) {
			if (kv.type == jsonToken::Object) delete kv.value_obj;
			if (kv.type == jsonToken::Array) delete kv.value_array;
		}
	}

	void jsonObject::Print(int level)
	{
		PRINT_tab(level);
		printf("{\n");

		for (auto& kv : mKeyValueList) {
			kv.Print(level+1);
		}

		PRINT_tab(level);
		printf("}");

	}

	jsonKeyValue* jsonObject::Find(const char* name)
	{
		for (auto& kv : mKeyValueList) {
			if (strcmp(kv.key, name) == 0)
				return &kv;
			if (kv.type == jsonToken::Object) {
				jsonKeyValue* v = kv.value_obj->Find(name);
				if (v) return v;
			}
		}

		return nullptr;
	}

	jsonArray::~jsonArray()
	{
		for (auto& kv : mValueList) {
			if (kv.type == jsonToken::Object) delete kv.value_obj;
			if (kv.type == jsonToken::Array) delete kv.value_array;
		}	
	}

	void jsonArray::Print(int level)
	{
		PRINT_tab(level);
		printf("[\n");

		for (auto& kv : mValueList) {
			PRINT_tab(level+1);
			kv.Print1(level + 1);
		}

		PRINT_tab(level);
		printf("]");

	}


	bool jsonValue::setValue(const jsonToken& t0)
	{
		type = t0.type;

		switch (t0.type)
		{
		case jsonToken::True: value_b = true; return true;
		case jsonToken::False: value_b = false; return true;
		case jsonToken::Null: value_string = 0; return true;
		case jsonToken::Number: value_i = token_copy_int(t0.start, t0.size); return true;
		case jsonToken::Float: value_f = token_copy_float(t0.start, t0.size); return true;
		case jsonToken::Array: value_array = t0.val_array; return true;
		case jsonToken::Object: value_obj = t0.val_obj; return true;
		case jsonToken::String: value_string = token_copy_string(t0.start, t0.size); return true;
		}

		return false;
	}

	void jsonValue::Print1(int level)
	{
		switch (type)
		{
		case jsonToken::String: printf("%s ,\n", value_string);
			break;
		case jsonToken::Number: printf("%d ,\n", value_i);
			break;
		case jsonToken::Float: printf("%f ,\n", value_f);
			break;
		case jsonToken::Null: printf("null ,\n");
			break;
		case jsonToken::True: printf("true ,\n");
			break;
		case jsonToken::False: printf("false ,\n");
			break;

		case jsonToken::Object: printf("\n"); value_obj->Print(level + 1); printf(" ,\n");
			break;
		case jsonToken::Array: printf("\n"); value_array->Print(level + 1); printf(" ,\n");
			break;
		}

	}

	void jsonKeyValue::setKey(const jsonToken& t0)
	{
		key = token_copy_string(t0.start, t0.size);
	}

	void jsonKeyValue::Print(int level)
	{
		PRINT_tab(level);

		printf("%s : ", key);

		jsonValue::Print1(level);
	}

	JsonFileBase::JsonFileBase()
	{ 
		mRoot = nullptr; 
		mFileData = nullptr;
	}

	JsonFileBase::~JsonFileBase()
	{
		if (mRoot) delete mRoot;
		if (mFileData) ::free( mFileData );
	}

	int JsonFileBase::Load(const char* file_name)
	{
		FILE* f = nullptr;
		fopen_s(&f, file_name, "rb");
		if (f == NULL)
			return 0;
		fseek(f, 0, SEEK_END);
		size_t file_size = (size_t)ftell(f);
		if (file_size == -1)
			return 0;
		fseek(f, 0, SEEK_SET);
		mFileData = (char*)::malloc(file_size + 2);
		fread(mFileData, 1, file_size, f);
		mFileData[file_size] = '\0';
		mFileData[file_size + 1] = '\0';
		mFileSize = (int)file_size;

		mPtr.tok = mFileData;
		mPtr.tok_end = mFileData + mFileSize;
		return mFileSize;
	}

	bool JsonFileBase::PopToken(jsonToken& t)
	{
		if (mPtr.getToken(t)) {
			if (t.type == jsonToken::WhiteSpace) return mPtr.getToken(t);
			return true;
		}

		return false;
	}








} // namespace dw1
