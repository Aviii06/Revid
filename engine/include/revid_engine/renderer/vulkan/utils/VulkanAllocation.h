#pragma once

namespace Revid::Allocation
{
    VKAPI_ATTR void* VKAPI_CALL myAlloc(
        void* pUserData,
        size_t size,
        size_t alignment,
        VkSystemAllocationScope allocationScope)
    {
        return aligned_alloc(alignment, size);
    }

    VKAPI_ATTR void* VKAPI_CALL myRealloc(
        void* pUserData,
        void* pOriginal,
        size_t size,
        size_t alignment,
        VkSystemAllocationScope allocationScope)
    {
        free(pOriginal);
        return aligned_alloc(alignment, size);
    }

    VKAPI_ATTR void VKAPI_CALL myFree(
        void* pUserData,
        void* pMemory)
    {
        free(pMemory);
    }

    VKAPI_ATTR void VKAPI_CALL myInternalAllocNotify(
        void* pUserData,
        size_t size,
        VkInternalAllocationType allocationType,
        VkSystemAllocationScope allocationScope)
    {
        Logger::Log(LogLevel::DEBUG, "Internal Alloc: " + std::to_string(size));
    }

    VKAPI_ATTR void VKAPI_CALL myInternalFreeNotify(
        void* pUserData,
        size_t size,
        VkInternalAllocationType allocationType,
        VkSystemAllocationScope allocationScope)
    {
        Logger::Log(LogLevel::DEBUG, "Internal free: " + std::to_string(size));
    }

}
