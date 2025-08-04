#pragma once

#include "GlmTypeTraits_VertexAttribute.hpp"
#include <tuple>

template<typename... TAttributes>
struct VertexAttributesList
{
	using TupleType = std::tuple<TAttributes...>;
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
	using TypeTrait = GLMTypeTrait<TGlmType>;
	using Type = typename TypeTrait::ComponentType;
	using Type2 = TGlmType;

	static constexpr GLint Location = AttrLocation;
	static constexpr GLint Size = TypeTrait::Size;
	static constexpr GLint SizeInBytes = Size * sizeof(Type);
	static constexpr GLenum EnumType = TypeTrait::EnumType;
	static constexpr GLboolean Normalized = TypeNormalized;
};


/*
 * VertexArrayObject Class that auto generates all Attributes for usage in Shaders.
 * It also does compile-time checks if the passed structure is compatible with declared attributes.
 */
template<typename VertexDataStruct, typename TAttributeListType>
struct VertexArrayObjectHandler
{
	public:
		using AttributesTupleType = typename TAttributeListType::TupleType;
		using VertexDataType = VertexDataStruct;

		AttributesTupleType Attributes;

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
			return (sizeof(typename std::tuple_element_t<Indexes, Tuple>::Type2) + ...);
		}

		explicit VertexArrayObjectHandler()
		{
			static_assert(
				sizeof(VertexDataStruct) == CalculateAttributesSize<AttributesTupleType>(),
				"VertexDataStruct size must match sum of attribute sizes"
			);
			glGenVertexArrays(1, &ID);
		}


		explicit VertexArrayObjectHandler(const GLuint NewVAO) : ID(NewVAO) {}


		void operator=(const GLuint NewVAO)
		{
			ID = NewVAO;
		}


		~VertexArrayObjectHandler()
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
					CurrentAttribute::EnumType,
					CurrentAttribute::Normalized,
					Stride,
					reinterpret_cast<void*>(CurrentOffset)
				);
				glEnableVertexAttribArray(CurrentAttribute::Location);

				// Calculate size of current attribute for next offset
				SetupVertexAttributesImpl<I + 1>(Stride, CurrentOffset + CurrentAttribute::SizeInBytes);
			}
		}

		// -------------------------------------------------------------------------------

	private:
		GLuint ID;
};