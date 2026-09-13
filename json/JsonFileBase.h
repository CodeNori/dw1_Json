#pragma once
#include <string>
#include <deque>

namespace dw1
{

	struct jsonToken
	{
		enum Type
		{
			Null,
			True,
			False,
			Number,
			Float,
			String,

			Array,
			Object,

			isValue,

			WhiteSpace,
			Colon,
			Comma,
			Brace0,
			Brace1,
			Bracket0,
			Bracket1,

			KeyValue
		};
		Type type;
		char* start;
		int size;

		union
		{
			struct jsonArray* val_array;
			struct jsonObject* val_obj;
			struct jsonKeyValue* val_kv;
		};

		void Print();
	};

	struct jsonLexer
	{
		char* tok;
		char* tok_end;


		bool skipWhiteSpace(jsonToken& token);
		bool parsingString(jsonToken& token);
		bool parsingNumber(jsonToken& token);
		bool getToken(jsonToken& token);
	};


	struct jsonBaseObj
	{
		jsonToken::Type type;

		jsonBaseObj() :type(jsonToken::Null) {}
		virtual ~jsonBaseObj() { }
	};

	struct jsonValue : public jsonBaseObj
	{
		union {
			bool value_b;
			int value_i;
			float value_f;
			char* value_string;
			struct jsonArray* value_array;
			struct jsonObject* value_obj;
			void* data;
		};
		jsonValue() { data = nullptr; }
		bool setValue(const jsonToken& t0);
		void Print1(int level);
	};

	class jsonKeyValue : public jsonValue
	{
	public:
		char* key;

		static int debug_created_count;

		jsonKeyValue() { ++debug_created_count; }
		~jsonKeyValue() { --debug_created_count; }
		void setKey(const jsonToken& t0);
		void Print(int level);
	};
	
	struct jsonObject : public jsonBaseObj
	{
		std::deque< jsonKeyValue> mKeyValueList;

		static int debug_created_count;

		jsonObject();
		~jsonObject();
		void Print(int level);
		jsonKeyValue* Find(const char* name);
	};

	struct jsonArray : public jsonBaseObj
	{
		std::deque<jsonValue> mValueList;

		static int debug_created_count;

		jsonArray();
		~jsonArray();
		void Print(int level);
	};


	class JsonFileBase
	{
	protected:
		char* mFileData = nullptr;
		int mFileSize = 0;

		jsonLexer mPtr;
		jsonObject* mRoot = nullptr;

		std::deque<jsonToken> mTokens;

		int Load(const char* file_name);
		bool PopToken(jsonToken& t);

	public:
		JsonFileBase();
		~JsonFileBase();

		void Print() { mRoot->Print(1); }
		jsonKeyValue* Find(const char* name) { return mRoot->Find(name); }
	};


}

