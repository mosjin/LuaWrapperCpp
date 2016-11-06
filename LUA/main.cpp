
#define MAIN_MAIN
#ifdef MAIN_MAIN

#include "./LUA/LuaWrapperCpp.h"


#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <array>
#include <tuple>
#include <vector>

#include "Benchmarks.h"

//=================================================================================================
//=================================================================================================
//=================================================================================================

//https://github.com/SteveKChiu/lua-intf



//=================================================================================================
//=================================================================================================
//=================================================================================================
#include "./TestClass.h"

//=================================================================================================
//=================================================================================================
//=================================================================================================

void HelloMethod()
{
	printf("Hello method\n");
}

void HelloMethodParam(int v)
{
	printf("Hello method Param: %i\n", v);
}

int HelloMethodParamReturn(int v)
{
	//printf("Hello method Param: %i\n", v);
	return v + 1;
}






//==================================================================================================
//==================================================================================================
//==================================================================================================




template<typename T, typename ... Args>
using get_print_type = decltype(std::declval<T>().Print0(std::declval<Args>() ...)) (T::*)(Args ...);


Lua::LuaScript * Create(LuaString name)
{
	//http://loadcode.blogspot.cz/2007/02/wrapping-c-classes-in-lua.html
	//https://john.nachtimwald.com/2014/07/12/wrapping-a-c-library-in-lua/

	Lua::LuaScript *ls = Lua::LuaWrapper::GetInstance()->AddScript(name, name);

	//using bar_t = TypeOverload<Account, &Account::Print5, int>::type;

	/*
	using bar_t = TypeOverload<Foo, void(Foo::*)(int, int), &Foo::bar, int, int>::type;
	static_assert(std::is_same<bar_t, void(Foo::*)(int, int)>::value, "y");

	using baz_t = TypeOverload<Foo, int(Foo::*)(double), &Foo::baz, double>::type;
	static_assert(std::is_same<baz_t, int(Foo::*)(double)>::value, "x");
	*/

	

	//ls->RegisterFunction("Print_fce", NULL);
	
	Lua::LuaClassBind<Account> cb("Account");
	cb.SetDefaultCtor<double>();
	cb.AddCtor<double, double>("Account_2");
	//cb.AddMethod("Print0", LuaCallbacks::function<decltype(std::declval<Account>().Print0()) (Account::*)(), &Account::Print0>);
	cb.AddMethod("Print0", CLASS_METHOD_OVERLOAD(Account, Print0));
	cb.AddMethod("Print0L", CLASS_METHOD_OVERLOAD(Account, Print0, double, double));
	//cb.AddMethod("Print0L", LuaCallbacks::function<TMP(Account, Print0, double, double), &Account::Print0>);
	
	//cb.AddMethod("Print0L", LuaCallbacks::function<TypeOverload<Account, &Account::Print0, double, double>::type, &Account::Print0>);
	cb.AddMethod("Print1", CLASS_METHOD(Account, Print1));
	cb.AddMethod("Print2", CLASS_METHOD(Account, Print2));
	cb.AddMethod("Print3", CLASS_METHOD(Account, Print3));
	cb.AddMethod("Print4", CLASS_METHOD(Account, Print4));
	cb.AddMethod("PrintConst", CLASS_METHOD(Account, PrintConst));
	//cb.AddMethod("PrintConst", LuaCallbacks::function2<decltype(&Account::PrintConst), &Account::PrintConst>);	
	cb.AddMethod("Print5", CLASS_METHOD(Account, Print5));
	cb.AddMethod("Print6", CLASS_METHOD(Account, Print6));
	cb.AddMethod("Print7", CLASS_METHOD(Account, Print7));
	cb.AddMethod("deposit", CLASS_METHOD(Account, deposit));
	cb.AddMethod("balance", CLASS_METHOD(Account, balance));
	/*
	cb.ctor = [](Lua::LuaScript * script) {
		return new Account(script->GetFnInput<double>());
	};
	*/

	cb.AddAttribute("vv", CLASS_ATTRIBUTE(Account, val));
	cb.AddAttribute("xx", CLASS_ATTRIBUTE(Account, xx));
	cb.AddAttribute("cc", CLASS_ATTRIBUTE(Account, cc));

	/*
	cb.toString = MyFunction<std::string, Account *>([](Account * a) -> std::string {
		return "string...";
	});
	*/
	
	cb.toString = [](void * a) -> std::string {
		Account * aa = (Account *)a;

		std::string str = "str";
		str += std::to_string(aa->val);

		return str;
	};
	

	ls->RegisterClass(cb);




	ls->RegisterFunction("Print_fce", METHOD(HelloMethod));
	ls->RegisterFunction("Print_fce2", METHOD(HelloMethodParam));


	
	Lua::LuaClassBind<Account2> cb2("Account2");	
	cb.SetDefaultCtor<double>();
	cb2.AddMethod("Print0", CLASS_METHOD(Account2, Print0));
	

	//ls->RegisterClass<Account2>(cb2);
	

	return ls;
}

struct MyClass
{
	int i;
	char c;
};


template<typename T, class C, T C::*MemPtr>
void doSomething(MyClass * cl)
{
	(cl->*MemPtr)++;
}

template<typename T, T t>
void doSomething2()
{
	//(cl->*MemPtr)++;
}




int main(int argc, char * argv[])
{
	/*
	MyClass cl;
	cl.i = 7;
	//arg_info<decltype(MyClass::i)> o();
	doSomething2<&MyClass::i>();
	doSomething<decltype(MyClass::i), MyClass, &MyClass::i>(&cl);


	return 0;
	*/
	Lua::LuaWrapper::Initialize([](const LuaString & str) -> LuaString {
		return Lua::LuaUtils::LoadFromFile(str);
	});


	//RunBenchmark();
	//return 1;



	
	


	
	Lua::LuaScript *ls = Create("t2.lua");
	

	
	//lua_boxpointer(L, aa);
	//(*(void **)(lua_newuserdata(L, sizeof(void *))) = (aa));
	
	//create data in "lua" memory
	//Account *foo = new (lua_newuserdata(L, sizeof(Account))) Account(450);

	//void * luaMem = lua_newuserdata(L, sizeof(void *));
	//luaMem = aa;

	
	Account* ee = new Account(600);	
	ls->SetGlobalVarClass("ee", ee);

	Account* cc = new Account(900);
	//ls->SetGlobalVarLight("cc", cc);

	ls->Run();
	printf("BBB %f BBB\n", cc->balance());
	printf("BBB %f BBB\n", ee->balance());
	printf("---> %f\n", ee->val);
	return 0;
	/*
	//ls->RegisterFunction("LUA_Test", LUA_Test);		
	ls->RegisterFunction("LUA_Test", LUA_TestClass);
	START;
	ls->Run();
	END;
	PRINT_TIME;
	


	MyUtils::LuaScript *ls2 = MyUtils::LuaWrapper::GetInstance()->AddScript("t.lua", "t.lua");
	
	MyFunction<int, TestClass *, int> * ff = new MyFunction<int, TestClass *, int>();
	ff->f = &HelloMethodParamReturn2;

	f = ff;
	//ls2->RegisterFunction("LUA_Test", LUACallback<int, TestClass *, int>);
	ls2->RegisterFunction("LUA_Test", ff);
	
	START;
	ls2->Run();
	END;
	PRINT_TIME;
	*/
	
	//ANTI_OPTIMALIZATION;

}

#endif