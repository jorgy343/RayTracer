export module Alignment;

import "Common.h";

namespace Yart
{
    // Originally from: https://stackoverflow.com/a/70994249/1078268
    export template<typename ElementType, std::size_t ALIGNMENT_IN_BYTES>
    class AlignedAllocator
    {
    private:
        static_assert(
            ALIGNMENT_IN_BYTES >= alignof(ElementType),
            "Beware that types like int have minimum alignment requirements or access will result in crashes.");

    public:
        using value_type = ElementType;

        static constexpr std::align_val_t ALIGNMENT{ALIGNMENT_IN_BYTES};

        constexpr AlignedAllocator() noexcept
        {

        }

        template<typename ElementType2, std::size_t ALIGNMENT_IN_BYTES2>
        constexpr AlignedAllocator(const AlignedAllocator<ElementType2, ALIGNMENT_IN_BYTES2>&) noexcept
        {

        }

        // This is only necessary because AlignedAllocator has a second template
        // argument for the alignment that will make the default
        // std::allocator_traits implementation fail during compilation.
        // @see https://stackoverflow.com/a/48062758/2191065
        template<class OtherElementType>
        struct rebind
        {
            using other = AlignedAllocator<OtherElementType, ALIGNMENT_IN_BYTES>;
        };

        [[nodiscard]]
        ElementType* allocate(std::size_t nElementsToAllocate)
        {
            if (nElementsToAllocate > std::numeric_limits<std::size_t>::max() / sizeof(ElementType))
            {
                throw std::bad_array_new_length();
            }

            auto const nBytesToAllocate = nElementsToAllocate * sizeof(ElementType);
            return reinterpret_cast<ElementType*>(::operator new[](nBytesToAllocate, ALIGNMENT));
        }

        void deallocate(
            ElementType* allocatedPointer,
            [[maybe_unused]] std::size_t nBytesAllocated)
        {
            ::operator delete[](allocatedPointer, ALIGNMENT);
        }
    };

    export template<typename T, std::size_t ALIGNMENT_IN_BYTES>
    using ConstexprAwareAllocator = std::conditional<std::is_constant_evaluated(), std::allocator<T>, AlignedAllocator<T, ALIGNMENT_IN_BYTES>>;

    export template<typename ElementType, std::size_t ALIGNMENT_IN_BYTES>
    using AlignedVector = std::vector<ElementType, ConstexprAwareAllocator<ElementType, ALIGNMENT_IN_BYTES>>;
}