#include "JsonFile.h"
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


// ~~~~~~~~~~~~~~~~~~~~~~~ Parsing ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define T0 mTokens[idx]
#define T1 mTokens[idx-1]
#define T2 mTokens[idx-2]
#define T3 mTokens[idx-3]

#define T0_(x) (mTokens[idx].type == x)
#define T1_(x) (mTokens[idx-1].type == x)
#define T2_(x) (mTokens[idx-2].type == x)

#define T0_Value ((mTokens[idx  ].type < jsonToken::isValue))
#define T1_Value ((mTokens[idx-1].type < jsonToken::isValue))
#define T2_Value ((mTokens[idx-2].type < jsonToken::isValue))



	bool JsonFile1::process_Brace0()
	{
		if (mTokens.size() < 1) return false;
		int idx = mTokens.size() - 1;

		if( T0_(jsonToken::Brace0) )
		{
			T0.val_obj = new jsonObject;
			return true;
		}

		return false;
	}

	bool JsonFile1::process_Bracket0()
	{
		if (mTokens.size() < 1) return false;
		int idx = mTokens.size() - 1;

		if( T0_(jsonToken::Bracket0) )
		{
			T0.val_array = new jsonArray;
			return true;
		}

		return false;
	}

	bool JsonFile1::process_KeyValue()
	{
		if (mTokens.size() < 3) return false;
		int idx = mTokens.size() - 1;

		if( T2_(jsonToken::String) &&
			T1_(jsonToken::Colon) &&
			T0_Value )
		{
			jsonKeyValue* kv = new jsonKeyValue;
			kv->setKey(T2);
			kv->setValue(T0);

			mTokens.pop_back();
			mTokens.pop_back();
			mTokens.pop_back();

			jsonToken re;
			re.type = jsonToken::KeyValue;
			re.val_kv = kv;
			mTokens.push_back(re);

			return true;
		}

		return false;
	}

	void JsonFile2::make_KeyValue()
	{
		int idx = mTokens.size() - 1;

		jsonKeyValue kv;
		kv.setKey(T2);
		kv.setValue(T0);

		mTokens.pop_back();
		mTokens.pop_back();
		mTokens.pop_back();

		mTokens.back().val_obj->mKeyValueList.push_back(kv);
	}

	bool JsonFile1::Bracket0_Value_T0()
	{
		int idx = mTokens.size() - 1;

		/* Array는 경우의 수가 많다. 하지만 코드는 동일하다.
		//  [ 값,
		//  [ Array,
		//  [ Object,
		//
		//  mTokens[idx] 에는 , 혹은 ] 이 들어 있다.
		//
		*/
		jsonValue val;
		val.setValue(T1);
		T2.val_array->mValueList.push_back(val);

		mTokens.pop_back();  // Comma 혹은 ] 를 제거 한다.
		mTokens.pop_back();   // 데이타도 제거한다.
		return true;
	}


	bool JsonFile1::Brace0_KeyValue_T0()
	{
		int idx = mTokens.size() - 1;

		// Brace0 다음은 KeyValue 만 가능하다....
		// { 키밸류,
		//
		//  mTokens[idx] 에는 , 혹은 } 이 들어 있다.
		//
		jsonKeyValue* kv = T1.val_kv;
		jsonObject* obj = T2.val_obj;
		obj->mKeyValueList.push_back(*kv);
		delete kv;

		mTokens.pop_back();   // , } 제거
		mTokens.pop_back();   // value 제거

		return true;
	}

	bool JsonFile1::process_Blank_Comma()
	{
		if (mTokens.size() < 2) return false;
		int idx = mTokens.size() - 1;

		//  빈거일때.....에러...
		// { ,
		// [ ,
		//		
		if ( T0_(jsonToken::Comma) &&
			(T1_(jsonToken::Brace0) || T1_(jsonToken::Bracket0) ) 
		) {
			mTokens.pop_back();
			
			//
			//  에러처리해야 함.....빈거는 에러임.
			//
			return true;
		}

		return false;
	}

	bool JsonFile1::process_Brace0_KeyValue_Comma()
	{
		if (mTokens.size() < 3) return false;
		int idx = mTokens.size() - 1;		

		// { KeyValue ,
		//
		if (T0_(jsonToken::Comma) &&
			T1_(jsonToken::KeyValue) &&
			T2_(jsonToken::Brace0) ) return Brace0_KeyValue_T0();

		return false;
	}

	bool JsonFile1::process_Bracket0_Value_Comma()
	{
		if (mTokens.size() < 3) return false;
		int idx = mTokens.size() - 1;

		//  [ 값,
		//  [ Array,
		//  [ Object,
		//
		if (T0_(jsonToken::Comma) &&
			T1_Value &&
			T2_(jsonToken::Bracket0)) return Bracket0_Value_T0();

		return false;
	}

	bool JsonFile1::process_Blank_Bracket1()
	{
		if (mTokens.size() < 2) return false;
		int idx = mTokens.size() - 1;

		//  [ ]
		//
		if ( T0_(jsonToken::Bracket1) &&
			 T1_(jsonToken::Bracket0)
		){
			T1.type = jsonToken::Array;

			mTokens.pop_back();  // jsonToken::Bracket1 날림
			return true;
		}

		return false;
	}

	bool JsonFile1::process_Blank_Brace1()
	{
		if (mTokens.size() < 2) return false;
		int idx = mTokens.size() - 1;

		// 빈거.
		// { }
		//
		if (T0_(jsonToken::Brace1) &&
			T1_(jsonToken::Brace0)
		){
			T1.type = jsonToken::Object;

			mTokens.pop_back();
			return true;
		}

		return false;
	}

	bool JsonFile1::process_Bracket0_Value_Bracket1()
	{
		if (mTokens.size() < 3) return false;
		int idx = mTokens.size() - 1;

		//  [ 값 ]
		//  [ Array ]
		//  [ Object ]
		//
		if ( T0_(jsonToken::Bracket1) &&
			 T1_Value &&
			 T2_(jsonToken::Bracket0)
		) {
			// Bracket0 을 Array로 바꿔준다.
			Bracket0_Value_T0();
			T2.type = jsonToken::Array;
			return true;
		}

		return false;
	}

	bool JsonFile1::process_Brace0_KeyValue_Brace1()
	{
		if (mTokens.size() < 3) return false;
		int idx = mTokens.size() - 1;


		//{ KeyValue }
		//
		if (T0_(jsonToken::Brace1) && 
			T1_(jsonToken::KeyValue) &&
			T2_(jsonToken::Brace0)
		){
			Brace0_KeyValue_T0();
			T2.type = jsonToken::Object;
			return true;

		}

		return false;
	}


	void JsonFile1::Parse(const char* file_name)
	{
		if (!Load(file_name)) return;

		bool bLoop = true;

		while (bLoop)
		{
			jsonToken t0;
			if (!PopToken(t0))
				return;
			mTokens.push_back(t0);

			if (process_Brace0()) continue;
			if (process_Bracket0()) continue;
			if (process_Blank_Comma()) continue; // 에러임...에러처리해야 함.
			if (process_Brace0_KeyValue_Comma()) continue;
			if (process_Bracket0_Value_Comma()) continue;

			if (process_Blank_Bracket1()) {}
			else if (process_Blank_Brace1()) {}
			else if (process_Brace0_KeyValue_Brace1()) {}
			else if (process_Bracket0_Value_Bracket1()) {};
			
			int cnt = 0;
			while (process_KeyValue()) {
				++cnt;
				break;
			}

			// Object 하나만 남아 있으면...
			if (mTokens.size() == 1 &&
				mTokens[0].type == jsonToken::Object)
			{
				mRoot = mTokens[0].val_obj;
				bLoop = false;
			}

		}
	}




#define T0P_(x) (mTokens.back().type == x)

#define POP_TOKET() if (!PopToken(t0)) return false; mTokens.push_back(t0);

	bool JsonFile2::Parse(const char* file_name)
	{
		if (!Load(file_name)) return false;

		mRoot = nullptr;

		jsonToken t0;
		POP_TOKET();

		if (t0.type == jsonToken::Brace0)
		{
			if (stmt_Object() == false)
				return false;
		}


		// Object 하나만 남아 있으면...
		if (mTokens.size() == 1 &&
			mTokens[0].type == jsonToken::Object)
		{
			mRoot = mTokens[0].val_obj;
		}

	}

	bool JsonFile2::stmt_Object()
	{
		mTokens.back().val_obj = new jsonObject;

		jsonToken t0;
		POP_TOKET();

		// 빈 Object ....
		if (T0P_(jsonToken::Brace1)) {
			mTokens.pop_back();
			mTokens.back().type = jsonToken::Object;
			return true;
		}
		
		bool bLoop = false;
		do {
			bLoop = false;

			if (!T0P_(jsonToken::String)) {
				printf("Object의 Key를 받던 중 에러 발생 !! \n");
				return false;
			}

			POP_TOKET();
			if (!T0P_(jsonToken::Colon)) {
				printf("Object의 Colon 자리에 에러 발생 !! \n");
				return false;
			}

			if (!stmt_Value()) {
				printf("Object의 Value 자리에 에러 발생 !! \n");
				return false;
			}

			make_KeyValue();

			POP_TOKET();
			if (T0P_(jsonToken::Comma)) {
				mTokens.pop_back();
				POP_TOKET();
				bLoop = true;

				if (T0P_(jsonToken::Brace1)) {
					mTokens.pop_back();
					mTokens.back().type = jsonToken::Object;
					return true;
				}
			}
			else if (T0P_(jsonToken::Brace1)) {
				mTokens.pop_back();
				mTokens.back().type = jsonToken::Object;
				return true;
			}
		} while (bLoop);

		return true;
	}


	bool JsonFile2::stmt_Array()
	{
		jsonArray* arr = new jsonArray;
		mTokens.back().val_array = arr;

		jsonToken t0;
		bool bLoop = true;

		do {
			if (stmt_Value()) {
				jsonValue val;
				val.setValue(mTokens.back());
				mTokens.pop_back(); // 마지막 데이타 토큰 제거
				arr->mValueList.push_back(val);
			}
			else {
				if (T0P_(jsonToken::Bracket1)) {
					mTokens.pop_back();
					mTokens.back().type = jsonToken::Array;
					return true;
				}

				printf("Array 만들던 중 에러 발생 !!!!\n");
				return false;
			}

			POP_TOKET();
			if (T0P_(jsonToken::Comma)) {
				mTokens.pop_back();
				bLoop = true;
			}
			else if (T0P_(jsonToken::Bracket1)) {
				mTokens.pop_back();
				mTokens.back().type = jsonToken::Array;
				return true;
			}

		} while (bLoop);

		return true;
	}

	bool JsonFile2::stmt_Value()
	{
		jsonToken t0;
		POP_TOKET();

		if ((t0.type == jsonToken::Null)) return true;
		if ((t0.type == jsonToken::True)) return true;
		if ((t0.type == jsonToken::False)) return true;
		if ((t0.type == jsonToken::Number)) return true;
		if ((t0.type == jsonToken::Float)) return true;
		if ((t0.type == jsonToken::String)) return true;

		if ((t0.type == jsonToken::Brace0)) return stmt_Object();
		if ((t0.type == jsonToken::Bracket0)) return stmt_Array();

		return false;
	}








} // namespace dw1
