#pragma once

#include "CommonTemplates.hpp"

#include <functional>


template<typename... TUniforms>
struct ShaderUniformsList
{
	using TupleType = std::tuple<TUniforms...>;
	static constexpr size_t Count = sizeof...(TUniforms);
};


template<StringLiteral NameLiteral, typename GLMType>
struct GLMShaderUniform
{
	using ValueType = GLMType;

	static constexpr auto Name = NameLiteral;
	static constexpr GLenum EnumType = GLMUniformTypeTrait<ValueType>::Type;

	GLint Location = -1;

	void Set(const ValueType& Value) const
	{
		if (Location == -1)
		{
			return;
		}

		GLMUniformTypeTrait<ValueType>::Set(Location, Value);
	}

	constexpr const char* GetName() const
	{
		return Name.String;
	}
};


template<typename TUniformsListType>
class ShaderUniformsDescriptor
{
	using UniformUpdatingFuncSign = std::function<void()>;
	using UniformTupleType = typename TUniformsListType::TupleType;

	UniformTupleType Uniforms;
	GLuint ShaderProgramID = 0;

	UniformUpdatingFuncSign GraphicalSetupFunction;


	public:
		// Initializes the descriptor.
		void SetShaderProgramID(const GLuint NewShaderProgramID)
		{
			ShaderProgramID = NewShaderProgramID;
			FindUniforms();
		}

		template<typename UniformType>
		void SetUniform(const typename UniformType::ValueType& Value)
		{
			auto& Uniform = std::get<UniformType>(Uniforms);
			Uniform.Set(Value);
		}

		void SetGraphicsUpdatingFunction(const UniformUpdatingFuncSign NewUniformUpdatingFunction)
		{
			GraphicalSetupFunction = NewUniformUpdatingFunction;
		}

		void UpdateGraphics()
		{
			GraphicalSetupFunction();
		}


	private:
		template<std::size_t Index = 0>
		void FindUniforms()
		{
			if constexpr (Index < std::tuple_size_v<UniformTupleType>)
			{
				auto& Uniform = std::get<Index>(Uniforms);
				const char* Name = Uniform.GetName();
				Log::println("Uniform: {}", Name);
				Uniform.Location = glGetUniformLocation(ShaderProgramID, Name);
				FindUniforms<Index + 1>();
			}
		}
};