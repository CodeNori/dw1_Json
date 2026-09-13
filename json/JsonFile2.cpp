#include "pch.h"
#include "JsonFile2.h"
#include <stdlib.h>
#include <stdio.h>


namespace dw1
{



#define T0 mTokens[idx]
#define T1 mTokens[idx-1]
#define T2 mTokens[idx-2]
#define T3 mTokens[idx-3]

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


	void JsonFile2::make_KeyValue()
	{
		int idx = mTokens.size() - 1;

		jsonKeyValue kv;
		kv.setKey(T2);
		kv.setValue(T0);

		mTokens[idx-3].val_obj->mKeyValueList.push_back(kv);
		mTokens.pop_back();
		mTokens.pop_back();
		mTokens.pop_back();

	}






} // namespace dw1
