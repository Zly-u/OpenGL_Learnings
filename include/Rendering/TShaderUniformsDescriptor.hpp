#pragma once

#include "CommonTemplates.hpp"
#include "TypeTraits_Uniform.hpp"

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

	static constexpr GLenum EnumType = UniformTypeTrait<ValueType>::Type;

	GLint Location = -1;

	void Set(const ValueType& Value) const
	{
		if (Location == -1)
		{
			return;
		}

		UniformTypeTrait<ValueType>::Set(Location, Value);
	}

	[[nodiscard]] constexpr const char* GetName() const
	{
		return NameLiteral.String;
	}
};


template<typename TUniformsListType>
class TShaderUniformsDescriptor
{
	using UniformTupleType = typename TUniformsListType::TupleType;

	UniformTupleType Uniforms;
	GLuint ShaderProgramID = 0;


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


	private:
		void FindUniforms()
		{
			std::apply(
				[this](auto&... Uniforms)
				{
					((Uniforms.Location = glGetUniformLocation(ShaderProgramID, Uniforms.GetName())), ...);
				},
				Uniforms
			);
		}
};