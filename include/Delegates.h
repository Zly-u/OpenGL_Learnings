#pragma once

#include <utility>
#include <vector>

template <typename T>
constexpr bool TIsFunctionPointerV =
	std::is_pointer_v<T>
	&& !std::is_function_v<std::remove_pointer_t<T>>;


template<typename TSignature>
concept TIsLambdaV = requires(TSignature t) {
	&TSignature::operator();
}
	&& !TIsFunctionPointerV<TSignature>
	&& !std::is_function_v<TSignature>;


template<typename... Args>
class Delegate final
{
	using StaticFuncPtrType = void(*)(Args...);

	template<typename Object>
	using RawFuncPtrType = void(Object::*)(Args...);

	// -----------------------------------------------------------------------------------------------------------------

	class AbstractDelegate
	{
		public:
			virtual ~AbstractDelegate() = default;

			virtual void Call(Args... args) = 0;
	};

	// -----------------------------------------------------------------------------------------------------------------

	template<typename Object>
	class RawDelegateObject final : public AbstractDelegate
	{
		public:
			RawDelegateObject(Object* PassedObject, RawFuncPtrType<Object> PassedFunc)
				: FuncPtr(PassedFunc), ObjectPtr(PassedObject) { }

			void Call(Args... args) override
			{
				if (!ObjectPtr)
				{
					return;
				}

				(ObjectPtr->*FuncPtr)(std::forward<Args>(args)...);
			}

		private:
			RawFuncPtrType<Object> FuncPtr = nullptr;
			Object* ObjectPtr = nullptr;
	};

	// -----------------------------------------------------------------------------------------------------------------

	template<typename TLambda>
	class LambdaDelegate : public AbstractDelegate
	{
		private:
			TLambda LambdaPtr = nullptr;

		public:
			explicit LambdaDelegate(TLambda&& PassedLambda)
				: LambdaPtr(std::forward<TLambda>(PassedLambda))
			{ }

			void Call(Args... args) override
			{
				(LambdaPtr)(std::forward<Args>(args)...);
			}
	};

	// -----------------------------------------------------------------------------------------------------------------

	public:

		template<typename TCallable>
		void Bind(TCallable&& PassedFunc)
		{
			using CallableType = std::decay_t<TCallable>;

			if constexpr (TIsLambdaV<CallableType>)
			{
				BindLambda(std::forward<TCallable>(PassedFunc));
			}
			else
			{
				BindStatic(std::forward<TCallable>(PassedFunc));
			}
		}

		void BindStatic(StaticFuncPtrType PassedFunc)
		{
			StaticFunctions.push_back(PassedFunc);
		}

		template<typename Object>
		void BindRaw(Object* PassedObject, RawFuncPtrType<Object> PassedFunc)
		{
			RawDelegates.push_back(new RawDelegateObject<Object>(PassedObject, PassedFunc));
		}

		template<typename TLambda>
		void BindLambda(TLambda&& PassedLambda)
		{
			using LambdaType = std::decay_t<TLambda>;
			RawDelegates.push_back(
				new LambdaDelegate<TLambda>(
					std::forward<LambdaType>(PassedLambda)
				)
			);
		}


		void Broadcast(Args... PassedArgs)
		{
			// Static binds
			for (StaticFuncPtrType BindFunction : StaticFunctions)
			{
				BindFunction(std::forward<Args>(PassedArgs)...);
			}

			// Raw binds
			for (auto RawDelegate : RawDelegates)
			{
				RawDelegate->Call(std::forward<Args>(PassedArgs)...);
			}
		}

		~Delegate()
		{
			for (const AbstractDelegate* RawDelegate : RawDelegates)
			{
				delete RawDelegate;
			}
		}

	private:
		std::vector<AbstractDelegate*> RawDelegates;
		std::vector<StaticFuncPtrType> StaticFunctions;
};














