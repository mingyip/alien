#pragma once

#include "device_functions.h"
#include "sm_60_atomic_functions.h"

#include "ModelBasic/ElementaryTypes.h"

#include "CudaAccessTOs.cuh"
#include "CudaConstants.cuh"
#include "Base.cuh"
#include "Physics.cuh"
#include "Map.cuh"
#include "EntityFactory.cuh"

class ParticleProcessorOnOrigData
{
public:
    __inline__ __device__ void init(SimulationData& data);

    __inline__ __device__ void processingMovement();
    __inline__ __device__ void processingCollision();
    __inline__ __device__ void processingTransformation();

private:
    SimulationData* _data;
    Map<Particle> _origParticleMap;
    int _startParticleIndex;
    int _endParticleIndex;
};


/************************************************************************/
/* Implementation                                                       */
/************************************************************************/
__inline__ __device__ void ParticleProcessorOnOrigData::init(SimulationData & data)
{
    _data = &data;
    _origParticleMap.init(data.size, data.particleMap);

    int indexResource = threadIdx.x + blockIdx.x * blockDim.x;
    int numEntities = data.particlesAC1.getNumEntries();
    calcPartition(numEntities, indexResource, blockDim.x * gridDim.x, _startParticleIndex, _endParticleIndex);

    __syncthreads();
}

__inline__ __device__ void ParticleProcessorOnOrigData::processingMovement()
{
    for (int particleIndex = _startParticleIndex; particleIndex <= _endParticleIndex; ++particleIndex) {
        Particle* particle = &_data->particlesAC1.getEntireArray()[particleIndex];
        particle->pos = add(particle->pos, particle->vel);
        _origParticleMap.mapPosCorrection(particle->pos);
        _origParticleMap.set(particle->pos, particle);
    }
}

__inline__ __device__ void ParticleProcessorOnOrigData::processingCollision()
{
    for (int particleIndex = _startParticleIndex; particleIndex <= _endParticleIndex; ++particleIndex) {
        Particle* particle = &_data->particlesAC1.getEntireArray()[particleIndex];
        Particle* otherParticle = _origParticleMap.get(particle->pos);
        if (otherParticle && otherParticle != particle) {
            if (particle->alive && otherParticle->alive) {

                DoubleLock lock;
                lock.init(&particle->locked, &otherParticle->locked);
                lock.tryLock();
                if (!lock.isLocked()) {
                    continue;
                }

                float factor1 = particle->energy / (particle->energy + otherParticle->energy);
                float factor2 = 1.0f - factor1;
                particle->vel = add(mul(particle->vel, factor1), mul(otherParticle->vel, factor2));
                atomicAdd(&particle->energy, otherParticle->energy);
                atomicAdd(&otherParticle->energy, -otherParticle->energy);
                otherParticle->alive = false;

                lock.releaseLock();
            }
        }
    }
}

__inline__ __device__ void ParticleProcessorOnOrigData::processingTransformation()
{
    for (int particleIndex = _startParticleIndex; particleIndex <= _endParticleIndex; ++particleIndex) {
        Particle* particle = &_data->particlesAC1.getEntireArray()[particleIndex];
        auto innerEnergy = particle->energy - Physics::linearKineticEnergy(cudaSimulationParameters.cellMass, particle->vel);
        if (innerEnergy >= cudaSimulationParameters.cellMinEnergy) {
            if (_data->numberGen.random() < cudaSimulationParameters.cellTransformationProb) {
                EntityFactory factory;
                factory.init(_data);
                factory.createClusterWithRandomCell(innerEnergy, particle->pos, particle->vel);
                particle->alive = false;
            }
        }
    }
}