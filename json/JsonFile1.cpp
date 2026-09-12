#include "JsonFile1.h"
#include <stdlib.h>
#include <stdio.h>



namespace dw1
{


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









} // namespace dw1
