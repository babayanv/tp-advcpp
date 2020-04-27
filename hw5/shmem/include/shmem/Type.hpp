#ifndef SHMEM_TYPE_HPP
#define SHMEM_TYPE_HPP

#include "shmem/allocator/LinearAllocator.hpp"

#include <string>


namespace shmem
{

using StringAllocator = shmem::allocator::LinearAllocator<char>;
using String = std::basic_string<char, std::char_traits<char>, StringAllocator>;

} // namespace shmem

#endif // SHMEM_TYPE_HPP
