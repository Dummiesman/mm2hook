#pragma once

namespace MM2
{
    // Forward declarations
    class datParser;
    class datParserRecord;
    class datBaseTokenizer;
    class datAsciiTokenizer;
    class datBinTokenizer;
    class datTokenizer;
    class datMultiTokenizer;

    // External declarations
    extern class Stream;
    extern class Vector2;
    extern class Vector3;
    extern class Vector4;

    // Class definitions

    namespace $
    {
        namespace datParser
        {
            declhook(0x4A78E0, _MemberFunc<void>, $$ctor);
            declhook(0x4A7920, _MemberFunc<void>, $$dtor);

            declhook(0x4A7980, _MemberFunc<MM2::datParser *>, AddParser);
            declhook(0x4A79D0, _MemberFunc<MM2::datParserRecord &>, AddRecord);

            declhook(0x4A7A90, _MemberFunc<bool>, Load$1);
            declhook(0x4A7AF0, _MemberFunc<bool>, Load$2);
            declhook(0x4A7B40, _MemberFunc<bool>, Load$3);

            declhook(0x4A7B90, _MemberFunc<bool>, Save$1);
            declhook(0x4A7C00, _MemberFunc<bool>, Save$2);
            declhook(0x4A7C50, _MemberFunc<bool>, Save$3);
        }
    }

    class datParserRecord {
    private:
        /*
            This class is NOT meant to be manipulated outside of datParser
            Hopefully by making this private we'll minimize any possible bugs
        */
        datParserRecord() {};
    public:
        int type;

        short dataCount;
        short dataLength;

        char name[64];

        void *dataPtr;
        void *callback; // unused

        // simple yet effective linked list:
        // parser
        // > record
        //  > record
        //   > record
        //    > (etc.)
        // parser will loop through each record and its children
        // once the destructor is called, any children below it are destroyed as well
        datParserRecord *child;
    };

    class datParser {
    private:
        char name[64];

        int numRecords;
        int _unk_44; // doesn't get initialized nor used?

        datParserRecord *record;

        int _unk_4c; // set to zero but never used?
        int indentLevel;

        /*
            Record types:

            0 - RawToken (no tabs/spaces allowed -- e.g. "MyProperty token_value")
            1 - Bool
            2 - Byte
            3 - Short
            4 - Int
            5 - Float
            6 - Vector2
            7 - Vector3
            8 - Vector4
            9 - Parser (datParser)
        */
        AGE_API datParserRecord & AddRecord(int type, LPCSTR name, void *dataPtr, int count) {
            // last parameter is the 'callback' which is never actually used
            return $::datParser::AddRecord(this, type, name, dataPtr, count, NULL);
        };

        inline datParserRecord & AddRecord(int type, LPCSTR name, void *dataPtr) {
            return AddRecord(type, name, dataPtr, 1);
        };
    public:
        AGE_API datParser(const char *filename) {
            $::datParser::$$ctor(this, filename);
        };

        AGE_API ~datParser() {
            $::datParser::$$dtor(this);
        };

        AGE_API datParser * AddParser(LPCSTR name) {
            // 'callback' unused (see AddRecord above)
            return $::datParser::AddParser(this, name, NULL);
        };

        void AddToken(LPCSTR name, char *tokenPtr, int tokenLength) {
            auto &record = AddRecord(0, name, tokenPtr);

            // MM2 doesn't have a way of adding this kind of record
            // so we need to manually set it here
            // (otherwise it won't work)
            record.dataLength = tokenLength;
        };

        template <int N>
        inline void AddString(LPCSTR name, char (*tokenPtr)[N]) {
            AddToken(name, tokenPtr[0], N);
        }

        /*
            Support for adding arrays of values
        */

        AGE_API void AddValue(LPCSTR name, bool *values, int count)     { AddRecord(1, name, values, count); };
        AGE_API void AddValue(LPCSTR name, char *values, int count)     { AddRecord(2, name, values, count); };
        AGE_API void AddValue(LPCSTR name, short *values, int count)    { AddRecord(3, name, values, count); };
        AGE_API void AddValue(LPCSTR name, int *values, int count)      { AddRecord(4, name, values, count); };
        AGE_API void AddValue(LPCSTR name, float *values, int count)    { AddRecord(5, name, values, count); };
        AGE_API void AddValue(LPCSTR name, Vector2 *values, int count)  { AddRecord(6, name, values, count); };
        AGE_API void AddValue(LPCSTR name, Vector3 *values, int count)  { AddRecord(7, name, values, count); };
        AGE_API void AddValue(LPCSTR name, Vector4 *values, int count)  { AddRecord(8, name, values, count); };

        /*
            Inline methods for adding a single value instead of an array
        */

        inline void AddValue(LPCSTR name, bool *value)      { AddValue(name, value, 1); };
        inline void AddValue(LPCSTR name, char *value)      { AddValue(name, value, 1); };
        inline void AddValue(LPCSTR name, short *value)     { AddValue(name, value, 1); };
        inline void AddValue(LPCSTR name, int *value)       { AddValue(name, value, 1); };
        inline void AddValue(LPCSTR name, float *value)     { AddValue(name, value, 1); };
        inline void AddValue(LPCSTR name, Vector2 *value)   { AddValue(name, value, 1); };
        inline void AddValue(LPCSTR name, Vector3 *value)   { AddValue(name, value, 1); };
        inline void AddValue(LPCSTR name, Vector4 *value)   { AddValue(name, value, 1); };

        AGE_API bool Load(Stream *stream, LPCSTR filename) {
            return $::datParser::Load$1(this, stream, filename);
        };

        AGE_API bool Load(LPCSTR directory, LPCSTR filename) {
            return $::datParser::Load$2(this, directory, filename);
        };

        AGE_API bool Load(LPCSTR directory, LPCSTR filename, LPCSTR extension) {
            return $::datParser::Load$3(this, directory, filename, extension);
        };

        /*
            Binary saving is indefinitely disabled since it's bugged
        */

        AGE_API bool Save(Stream *stream, LPCSTR filename) {
            return $::datParser::Save$1(this, stream, filename, false);
        };

        AGE_API bool Save(LPCSTR directory, LPCSTR filename) {
            return $::datParser::Save$2(this, directory, filename, false);
        };

        AGE_API bool Save(LPCSTR directory, LPCSTR filename, LPCSTR extension) {
            return $::datParser::Save$3(this, directory, filename, extension, false);
        };
    };

    class datBaseTokenizer
    {
    private:
        char buffer[0x54];
    private:
        // Lua methods
        Vector2 MatchVector2(const char* key)
        {
            Vector2 vec;
            MatchVector(key, vec);
            return vec;
        }

        Vector3 MatchVector3(const char* key)
        {
            Vector3 vec;
            MatchVector(key, vec);
            return vec;
        }

        Vector4 MatchVector4(const char* key)
        {
            Vector4 vec;
            MatchVector(key, vec);
            return vec;
        }

        Vector2 GetVector2()
        {
            Vector2 vec;
            GetVector(vec);
            return vec;
        }

        Vector3 GetVector3()
        {
            Vector3 vec;
            GetVector(vec);
            return vec;
        }

        Vector4 GetVector4()
        {
            Vector4 vec;
            GetVector(vec);
            return vec;
        }

        std::string LuaGetToken()
        {
            char buf[256];
            int len = this->GetToken(buf, 256);

            std::string str(buf);
            return str;
        }

        void LuaPutString(const char* str)
        {
            this->Put(str, static_cast<unsigned int>(strlen(str)));
        }
    public:
        void Init(const char* name, Stream* stream)                { hook::Thunk<0x4C8210>::Call<void>(this, name, stream); }
        int GetToken(char* buf, int bufsize)                       { return hook::Thunk<0x4C82F0>::Call<int>(this, buf, bufsize); }
        int GetBlock(char* buf, int bufsize)                       { return hook::Thunk<0x4C8440>::Call<int>(this, buf, bufsize); }
        bool CheckToken(const char* token, bool consumeIfFound)    { return hook::Thunk<0x4C8500>::Call<bool>(this, token, consumeIfFound); }
        void IgnoreToken()                                         { hook::Thunk<0x4C8670>::Call<void>(this); }
        void MatchToken(const char* token)                         { hook::Thunk<0x4C8480>::Call<void>(this, token); }

        virtual int GetInt()                                       PURE;
        virtual float GetFloat()                                   PURE;
        virtual void GetVector(Vector4& vec)                       PURE;
        virtual void GetVector(Vector3& vec)                       PURE;
        virtual void GetVector(Vector2& vec)                       PURE;
        virtual void GetDelimiter(const char* str)                 PURE;
        virtual int MatchInt(const char* key)                      PURE;
        virtual float MatchFloat(const char* key)                  PURE;
        virtual void MatchVector(const char* key, Vector4& vec)    PURE;
        virtual void MatchVector(const char* key, Vector3& vec)    PURE;
        virtual void MatchVector(const char* key, Vector2& vec)    PURE;
        virtual bool Put(Vector4 const& vec)                       PURE;
        virtual bool Put(Vector3 const& vec)                       PURE;
        virtual bool Put(Vector2 const& vec)                       PURE;
        virtual bool Put(float val)                                PURE;
        virtual bool Put(int val)                                  PURE;
        virtual bool Put(char val)                                 { return hook::Thunk<0x4C86D0>::Call<bool>(this, val); }
        virtual bool Put(const char* val, unsigned int length)     PURE;
        virtual bool PutDelimiter(const char* val)                 PURE;

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<datBaseTokenizer>("datBaseTokenizer")
                .addFunction("Init", &Init)
                .addFunction("IgnoreToken", &IgnoreToken)
                .addFunction("CheckToken", &CheckToken)
                .addFunction("MatchToken", &MatchToken)
                .addFunction("GetToken", &LuaGetToken)
                .addFunction("GetInt", &GetInt)
                .addFunction("GetFloat", &GetFloat)
                .addFunction("GetVector2", &GetVector2)
                .addFunction("GetVector3", &GetVector3)
                .addFunction("GetVector4", &GetVector4)
                .addFunction("MatchInt", &MatchInt)
                .addFunction("MatchFloat", &MatchFloat)
                .addFunction("MatchVector2", &MatchVector2)
                .addFunction("MatchVector3", &MatchVector3)
                .addFunction("MatchVector4", &MatchVector4)
                .addFunction("PutInt", static_cast<bool(datBaseTokenizer::*)(int)>(&datBaseTokenizer::Put))
                .addFunction("PutFloat", static_cast<bool(datBaseTokenizer::*)(float)>(&datBaseTokenizer::Put))
                .addFunction("PutCh", static_cast<bool(datBaseTokenizer::*)(char)>(&datBaseTokenizer::Put))
                .addFunction("Put", &LuaPutString)
                .endClass();
        }
    };
    ASSERT_SIZEOF(datBaseTokenizer, 0x58);


    class datAsciiTokenizer : public datBaseTokenizer {
    public:
        virtual int GetInt() override                                      { return hook::Thunk<0x4C8770>::Call<int>(this); }
        virtual float GetFloat() override                                  { return hook::Thunk<0x4C87D0>::Call<float>(this); }
        virtual void GetVector(Vector4& vec) override                      { hook::Thunk<0x4C8890>::Call<void>(this, &vec); }
        virtual void GetVector(Vector3& vec) override                      { hook::Thunk<0x4C8860>::Call<void>(this, &vec); }
        virtual void GetVector(Vector2& vec) override                      { hook::Thunk<0x4C8830>::Call<void>(this, &vec); }
        virtual void GetDelimiter(const char* str) override                { hook::Thunk<0x4C88D0>::Call<void>(this, str); }
        virtual int MatchInt(const char* key) override                     { return hook::Thunk<0x4C88E0>::Call<int>(this, key); }
        virtual float MatchFloat(const char* key) override                 { return hook::Thunk<0x4C8900>::Call<float>(this, key); }
        virtual void MatchVector(const char* key, Vector4& vec) override   { hook::Thunk<0x4C8960>::Call<void>(this, key, &vec); }
        virtual void MatchVector(const char* key, Vector3& vec) override   { hook::Thunk<0x4C8940>::Call<void>(this, key, &vec); }
        virtual void MatchVector(const char* key, Vector2& vec) override   { hook::Thunk<0x4C8920>::Call<void>(this, key, &vec); }
        virtual bool Put(Vector4 const& vec) override                      { return hook::Thunk<0x4C8B50>::Call<bool>(this, &vec); }
        virtual bool Put(Vector3 const& vec) override                      { return hook::Thunk<0x4C8AF0>::Call<bool>(this, &vec); }
        virtual bool Put(Vector2 const& vec) override                      { return hook::Thunk<0x4C8A90>::Call<bool>(this, &vec); }
        virtual bool Put(float val) override                               { return hook::Thunk<0x4C8A40>::Call<bool>(this, val); }
        virtual bool Put(int val) override                                 { return hook::Thunk<0x4C8A00>::Call<bool>(this, val); }
        virtual bool Put(const char* val, unsigned int length) override    { return hook::Thunk<0x4C8980>::Call<bool>(this, val, length); }
        virtual bool PutDelimiter(const char* val) override                { return hook::Thunk<0x4C8BC0>::Call<bool>(this, val); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<datAsciiTokenizer, datBaseTokenizer>("datAsciiTokenizer")
                .endClass();
        }
    };

    class datBinTokenizer : public datBaseTokenizer {
    public:
        virtual int GetInt() override                                      { return hook::Thunk<0x4C8BE0>::Call<int>(this); }
        virtual float GetFloat() override                                  { return hook::Thunk<0x4C8C20>::Call<float>(this); }
        virtual void GetVector(Vector4& vec) override                      { hook::Thunk<0x4C8CF0>::Call<void>(this, &vec); }
        virtual void GetVector(Vector3& vec) override                      { hook::Thunk<0x4C8CB0>::Call<void>(this, &vec); }
        virtual void GetVector(Vector2& vec) override                      { hook::Thunk<0x4C8C70>::Call<void>(this, &vec); }
        virtual void GetDelimiter(const char* str) override                { hook::Thunk<0x4C8D30>::Call<void>(this, str); }
        virtual int MatchInt(const char* key) override                     { return hook::Thunk<0x4C8D40>::Call<int>(this, key); }
        virtual float MatchFloat(const char* key) override                 { return hook::Thunk<0x4C8D50>::Call<float>(this, key); }
        virtual void MatchVector(const char* key, Vector4& vec) override   { hook::Thunk<0x4C8D80>::Call<void>(this, key, &vec); }
        virtual void MatchVector(const char* key, Vector3& vec) override   { hook::Thunk<0x4C8D70>::Call<void>(this, key, &vec); }
        virtual void MatchVector(const char* key, Vector2& vec) override   { hook::Thunk<0x4C8D60>::Call<void>(this, key, &vec); }
        virtual bool Put(Vector4 const& vec) override                      { return hook::Thunk<0x4C8E60>::Call<bool>(this, &vec); }
        virtual bool Put(Vector3 const& vec) override                      { return hook::Thunk<0x4C8E40>::Call<bool>(this, &vec); }
        virtual bool Put(Vector2 const& vec) override                      { return hook::Thunk<0x4C8E20>::Call<bool>(this, &vec); }
        virtual bool Put(float val) override                               { return hook::Thunk<0x4C8E00>::Call<bool>(this, val); }
        virtual bool Put(int val) override                                 { return hook::Thunk<0x4C8DE0>::Call<bool>(this, val); }
        virtual bool Put(const char* val, unsigned int length) override    { return hook::Thunk<0x4C8D90>::Call<bool>(this, val, length); }
        virtual bool PutDelimiter(const char* val) override                { return hook::Thunk<0x4C8E80>::Call<bool>(this, val); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<datBinTokenizer, datBaseTokenizer>("datBinTokenizer")
                .endClass();
        }
    };

    class datTokenizer : public datAsciiTokenizer
    {
    public:
        datTokenizer(const char* name, Stream* stream)
        {
            Init(name, stream);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<datTokenizer, datAsciiTokenizer>("datTokenizer")
                .addConstructor(LUA_ARGS(LPCSTR, Stream*))
                .endClass();
        }
    };

    class datMultiTokenizer
    {
    private:
        datAsciiTokenizer m_tokenizerA;
        datBinTokenizer m_tokenizerB;
    public:
        datMultiTokenizer()
        {
            hook::Thunk<0x4C8E90>::Call<void>(this);
        }

        datBaseTokenizer& GetReadTokenizer(const char* name, Stream* stream, const char* typeAHeader, const char* typeBHeader)
        {
            return hook::Thunk<0x4C8EA0>::Call<datBaseTokenizer&>(this, name, stream, typeAHeader, typeBHeader);
        }

        datBaseTokenizer& GetWriteTokenizer(const char* name, Stream* stream, char binary, const char* header)
        {
            return hook::Thunk<0x4C8FB0>::Call<datBaseTokenizer&>(this, name, stream, binary, header);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<datMultiTokenizer>("datMultiTokenizer")
                .addConstructor(LUA_ARGS())
                .addFunction("GetReadTokenizer", &GetReadTokenizer)
                .addFunction("GetWriteTokenizer", &GetWriteTokenizer)
                .endClass();
        }
    };
    ASSERT_SIZEOF(datMultiTokenizer, 0xB0);

    // Lua initialization

}