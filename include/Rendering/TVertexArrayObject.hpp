#pragma once

#include "GlmTypeTraits.hpp"
#include <tuple>

template<typename... TAttributes>
struct VertexAttributesList
{
	using Type = std::tuple<TAttributes...>;
	static constexpr size_t Count = sizeof...(TAttributes);
};


namespace std {
	template<typename... TAttributes>
	struct tuple_size<VertexAttributesList<TAttributes...>> : integral_constant<size_t, sizeof...(TAttributes)> {};

	template<size_t I, typename... TAttributes>
	struct tuple_element<I, VertexAttributesList<TAttributes...>>
	{
		using type = tuple_element_t<I, tuple<TAttributes...>>;
	};
}


/*
 * For generic types.
 */
template<GLuint AttrLocation, GLint TypeSize, GLenum AttrType, GLboolean TypeNormalized = GL_FALSE>
struct VertexAttribute
{
	static constexpr GLint Location = AttrLocation;
	static constexpr GLint Size = TypeSize;
	static constexpr GLenum Type = AttrType;
	static constexpr GLboolean Normalized = TypeNormalized;
};


/*
 * For GLM types, such as glm::vec2, glm::vec3, glm::mat4, etc.
 */
template<GLuint AttrLocation, typename TGlmType, GLboolean TypeNormalized = GL_FALSE>
struct GLMVertexAttribute
{
	static constexpr GLint Location = AttrLocation;
	static constexpr GLint Size = GLMTypeTrait<TGlmType>::Size;
	static constexpr GLenum Type = GLMTypeTrait<TGlmType>::Type;
	static constexpr GLboolean Normalized = TypeNormalized;
	using GlmType = TGlmType;
};


/*
 * VertexArrayObject Class that auto generates all Attributes for usage in Shaders.
 */
template<typename VertexDataStruct, typename TAttributeListType>
struct VertexArrayObject
{
	public:
		using VertexDataType = VertexDataStruct;

		// -------------------------------------------------------------------------------

	public:
		template<typename Tuple>
		static constexpr std::size_t CalculateAttributesSize()
		{
			return CalculateAttributesSizeImpl<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
		}

		template<typename Tuple, std::size_t... Indexes>
		static constexpr std::size_t CalculateAttributesSizeImpl(std::index_sequence<Indexes...>)
		{
			return (sizeof(typename std::tuple_element_t<Indexes, Tuple>::GlmType) + ...);
		}

		explicit VertexArrayObject()
		{
			static_assert(
				sizeof(VertexDataStruct) == CalculateAttributesSize<typename TAttributeListType::Type>(),
				"VertexDataStruct size must match sum of attribute sizes"
			);
			glGenVertexArrays(1, &ID);
		}


		explicit VertexArrayObject(const GLuint NewVAO) : ID(NewVAO) {}


		void operator=(const GLuint NewVAO)
		{
			ID = NewVAO;
		}


		~VertexArrayObject()
		{
			glDeleteVertexArrays(1, &ID);
		}


		// -------------------------------------------------------------------------------


		void SetupVertexAttributes() {
			Bind();
			const GLsizei Stride = CalculateTotalStride();
			SetupVertexAttributesImpl(Stride);
		}


		void Bind() const
		{
			glBindVertexArray(ID);
		}


		static void Unbind()
		{
			glBindVertexArray(0);
		}

		// -------------------------------------------------------------------------------

	private:
		constexpr GLsizei CalculateTotalStride() const
		{
			return sizeof(VertexDataType);
		}


		template<typename AttributeType>
		constexpr GLuint GetAttributeLocation() const {
			return AttributeType::Location;
		}


		template<std::size_t I = 0>
		void SetupVertexAttributesImpl(const GLsizei Stride, const size_t CurrentOffset = 0) {
			if constexpr (I < std::tuple_size_v<TAttributeListType>)
			{
				using CurrentAttribute = std::tuple_element_t<I, TAttributeListType>;

				glVertexAttribPointer(
					CurrentAttribute::Location,
					CurrentAttribute::Size,
					CurrentAttribute::Type,
					CurrentAttribute::Normalized,
					Stride,
					reinterpret_cast<void*>(CurrentOffset)
				);
				glEnableVertexAttribArray(CurrentAttribute::Location);

				// Calculate size of current attribute for next offset
				const size_t AttributeSize = GetAttributeSize<CurrentAttribute>();
				SetupVertexAttributesImpl<I + 1>(Stride, CurrentOffset + AttributeSize);
			}
		}

		template<typename Attribute>
		constexpr size_t GetAttributeSize() const {
			size_t TypeSize = 0;

			// clang-format: off
			switch (Attribute::Type) {
				case GL_FLOAT:			TypeSize = sizeof(GLfloat); break;
				case GL_DOUBLE:			TypeSize = sizeof(GLdouble); break;
				case GL_BYTE:			TypeSize = sizeof(GLbyte); break;
				case GL_UNSIGNED_BYTE:	TypeSize = sizeof(GLubyte); break;
				case GL_SHORT:			TypeSize = sizeof(GLshort); break;
				case GL_UNSIGNED_SHORT: TypeSize = sizeof(GLushort); break;
				case GL_INT:			TypeSize = sizeof(GLint); break;
				case GL_UNSIGNED_INT:	TypeSize = sizeof(GLuint); break;
				default:				TypeSize = sizeof(GLfloat); break;
			}
			// clang-format: on
			return TypeSize * Attribute::Size;
		}

		// -------------------------------------------------------------------------------

	private:
		GLuint ID;
};