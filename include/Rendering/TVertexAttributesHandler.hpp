#pragma once

#include "TypeTraits_VertexAttribute.hpp"
#include <tuple>

/*
 * A list that simply holds different VertexAttributes
 */
template<typename... TVertexAttributes>
struct VertexAttributesList
{
	using TupleType = std::tuple<TVertexAttributes...>;
	static constexpr size_t Count = sizeof...(TVertexAttributes);
};


namespace std {
	template<typename... TVertexAttributes>
	struct tuple_size<VertexAttributesList<TVertexAttributes...>> : integral_constant<size_t, sizeof...(TVertexAttributes)> {};

	template<size_t Index, typename... TAttributes>
	struct tuple_element<Index, VertexAttributesList<TAttributes...>>
	{
		using type = tuple_element_t<Index, tuple<TAttributes...>>;
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
	using TypeTrait = VertexAttributeTypeTrait<TGlmType>;
	using Type = typename TypeTrait::ComponentType;
	using GlmType = TGlmType;

	static constexpr GLint Location = AttrLocation;
	static constexpr GLint Size = TypeTrait::Size;
	static constexpr GLint SizeInBytes = Size * sizeof(Type);
	static constexpr GLenum EnumType = TypeTrait::EnumType;
	static constexpr GLboolean Normalized = TypeNormalized;

	GLMVertexAttribute() = delete; // We don't want an accidental possibility of the attribute creation on its own.
	GLMVertexAttribute(const GlmType& NewData) : Data(NewData) {}
	GLMVertexAttribute(std::initializer_list<Type> NewList)
	{
		assert(NewList.size() == Size && "Wrong number of elements for GLM Type.");

		std::array<Type, Size> ArgsArray{};
		std::copy(NewList.begin(), NewList.end(), ArgsArray.begin());

		Data = [&]<std::size_t... Indexes>(std::index_sequence<Indexes...>)
		{
			return GlmType{ArgsArray[Indexes]...};
		}(std::make_index_sequence<Size>{});
	}

	void operator=(const GlmType& NewValue)
	{
		Data = NewValue;
	}

	GlmType Data;
};


/*
 * VertexArrayObject Class that auto generates all Attributes for usage in Shaders.
 * It also does compile-time checks if the passed structure is compatible with declared attributes.
 */
template<typename VertexDataStruct, typename TAttributeListType>
struct TVertexAttributesHandler
{
	using AttributesTupleType = typename TAttributeListType::TupleType;
	using VertexDataStructType = VertexDataStruct;

	// -------------------------------------------------------------------------------

	public:
		explicit TVertexAttributesHandler()
		{
			static_assert(
				sizeof(VertexDataStructType) == CalculateAttributesSize<AttributesTupleType>(),
				"VertexDataStruct size must match sum of attribute sizes"
			);

			CheckOrder();

			glGenVertexArrays(1, &VAO_ID);
		}


		~TVertexAttributesHandler()
		{
			glDeleteVertexArrays(1, &VAO_ID);
		}


		// -------------------------------------------------------------------------------

		void SetupVertexAttributes()
		{
			Bind();
			const GLsizei Stride = CalculateTotalStride();
			SetupVertexAttributes(Stride);
		}

		void SetAssociatedVBO(const GLuint NewVBO)
		{
			VBO_ID = NewVBO;
		}

		void Bind() const
		{
			// TODO: Optimize binding, check if its already bind.
			glBindVertexArray(VAO_ID);
			glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
		}


		static void Unbind()
		{
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}


		// -------------------------------------------------------------------------------


		template<typename TVertexAttribute>
		void SetAttributeBufferData(const std::array<typename TVertexAttribute::GlmType, 4>& VertexData)
		{
			constexpr size_t AttributeOffset = CalculateAttributeOffset<TVertexAttribute>();
			constexpr size_t AttributeSize = sizeof(typename TVertexAttribute::GlmType);

			for (int i = 0; i < 4; i++) {
				size_t VertexOffset = i * sizeof(VertexDataStructType) + AttributeOffset;
				glBufferSubData(GL_ARRAY_BUFFER, VertexOffset, AttributeSize, &VertexData[i]);
			}
		}

		// -------------------------------------------------------------------------------


	private:
		template<size_t Index = 0>
		static constexpr void CheckOrder()
		{
			if constexpr (Index < std::tuple_size_v<AttributesTupleType>)
			{
				using CurrentAttribute = std::tuple_element_t<Index, AttributesTupleType>;
				static_assert(CurrentAttribute::Location == Index, "Incorrect Attributes order");
				CheckOrder<Index + 1>();
			}
		}

		template<typename TargetAttribute, size_t Index = 0>
		static constexpr size_t CalculateAttributeOffset()
		{
			static_assert(Index < std::tuple_size_v<AttributesTupleType>, "Attribute not found in list.");

			using CurrentAttribute = std::tuple_element_t<Index, AttributesTupleType>;

			if constexpr (std::is_same_v<CurrentAttribute, TargetAttribute>)
			{
				return 0;
			}
			else
			{
				return sizeof(typename CurrentAttribute::GlmType) + CalculateAttributeOffset<TargetAttribute, Index + 1>();
			}
		}

		// -------------------------------------------------------------------------------

		template<typename Tuple>
		static constexpr std::size_t CalculateAttributesSize()
		{
			return TVertexAttributesHandler::CalculateAttributesSizeImpl<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
		}


		template<typename Tuple, std::size_t... Indexes>
		static constexpr std::size_t CalculateAttributesSizeImpl(std::index_sequence<Indexes...>)
		{
			return (sizeof(typename std::tuple_element_t<Indexes, Tuple>::GlmType) + ...);
		}

		// -------------------------------------------------------------------------------


	private:
		constexpr GLsizei CalculateTotalStride() const
		{
			return sizeof(VertexDataStructType);
		}


		template<typename AttributeType>
		constexpr GLuint GetAttributeLocation() const {
			return AttributeType::Location;
		}


		template<std::size_t Index = 0>
		void SetupVertexAttributes(const GLsizei Stride, const size_t CurrentOffset = 0) {
			if constexpr (Index < std::tuple_size_v<TAttributeListType>)
			{
				using CurrentAttribute = std::tuple_element_t<Index, TAttributeListType>;

				glVertexAttribPointer(
					CurrentAttribute::Location,
					CurrentAttribute::Size,
					CurrentAttribute::EnumType,
					CurrentAttribute::Normalized,
					Stride,
					reinterpret_cast<void*>(CurrentOffset)
				);
				glEnableVertexAttribArray(CurrentAttribute::Location);

				this->SetupVertexAttributes<Index + 1>(Stride, CurrentOffset + CurrentAttribute::SizeInBytes);
			}
		}

		// -------------------------------------------------------------------------------

	private:
		GLuint VAO_ID;
		GLuint VBO_ID = 0;
};