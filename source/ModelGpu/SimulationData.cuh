#pragma once

#include "Base.cuh"
#include "Definitions.cuh"
#include "Entities.cuh"
#include "CellFunctionData.cuh"

struct SimulationData
{
    int2 size;
    int timestep;

    CellMap cellMap;
    ParticleMap particleMap;
    CellFunctionData cellFunctionData;

    Entities entities;
    Entities entitiesForCleanup;

    DynamicMemory dynamicMemory;

    CudaNumberGenerator numberGen;
    int numImageBytes;
    unsigned int* rawImageData;
    unsigned int* finalImageData;

    void init(int2 const& universeSize, int2 const& displaySize, CudaConstants const& cudaConstants, int timestep_)
    {
        size = universeSize;
        timestep = timestep_;

        entities.init(cudaConstants);
        entitiesForCleanup.init(cudaConstants);
        cellFunctionData.init(universeSize);
        cellMap.init(size, cudaConstants.MAX_CELLPOINTERS, entities.cellPointers.getArrayForHost());
        particleMap.init(size, cudaConstants.MAX_PARTICLEPOINTERS);
        dynamicMemory.init(cudaConstants.DYNAMIC_MEMORY_SIZE);
        numberGen.init(40312357);

        numImageBytes = max(displaySize.x * displaySize.y, size.x * size.y);
        CudaMemoryManager::getInstance().acquireMemory<unsigned int>(numImageBytes, rawImageData);
        CudaMemoryManager::getInstance().acquireMemory<unsigned int>(numImageBytes, finalImageData);
    }

    void resizeImage(int2 const& newSize)
    {
        CudaMemoryManager::getInstance().freeMemory(rawImageData);
        CudaMemoryManager::getInstance().freeMemory(finalImageData);
        CudaMemoryManager::getInstance().acquireMemory<unsigned int>(max(newSize.x * newSize.y, size.x * size.y), rawImageData);
        CudaMemoryManager::getInstance().acquireMemory<unsigned int>(max(newSize.x * newSize.y, size.x * size.y), finalImageData);
        numImageBytes = newSize.x * newSize.y;
    }

    void free()
    {
        entities.free();
        entitiesForCleanup.free();
        cellFunctionData.free();
        cellMap.free();
        particleMap.free();
        numberGen.free();
        dynamicMemory.free();

        CudaMemoryManager::getInstance().freeMemory(rawImageData);
        CudaMemoryManager::getInstance().freeMemory(finalImageData);
    }
};

