/**
 * @file        settings.cpp
 * 
 * @brief       Implements fx3d::Settings.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-07-14
 */
#include <fx3d/settings.hpp>


fx3d::VelocitySet fx3d::Settings::m_VSet        = fx3d::VelocitySet::D3Q19;
unsigned int fx3d::Settings::m_VSetSize         = 19u;
unsigned int fx3d::Settings::m_VSetDims         = 3u;
unsigned int fx3d::Settings::m_VSetTransfer     = 5u;
fx3d::CollisionType fx3d::Settings::m_CollType  = fx3d::CollisionType::SRT;
// fx3d::DDFCompression fx3d::Settings::m_Compr    = fx3d::DDFCompression::FP16S;
// fx3d::VelocitySet fx3d::Settings::m_VSet        = (fx3d::VelocitySet)0;
// fx3d::CollisionType fx3d::Settings::m_CollType         = (fx3d::CollisionType)0;
fx3d::DDFCompression fx3d::Settings::m_Compr    = (fx3d::DDFCompression)0;
fx3d::Feature fx3d::Settings::m_Features        = (fx3d::Feature)0;



fx3d::VelocitySet fx3d::Settings::GetVelocitySet() { return m_VSet; }
fx3d::CollisionType fx3d::Settings::GetCollisionType() { return m_CollType; }
fx3d::DDFCompression fx3d::Settings::GetDDFCompression() { return m_Compr; }
unsigned int fx3d::Settings::GetVSetSize() { return m_VSetSize; }
unsigned int fx3d::Settings::GetVSetDims() { return m_VSetDims; }
unsigned int fx3d::Settings::GetVSetTransfer() { return m_VSetTransfer; }

void fx3d::Settings::SetCollisionType(fx3d::CollisionType CType) { m_CollType = CType; }
void fx3d::Settings::SetDDFCompression(fx3d::DDFCompression Compr) { m_Compr = Compr; }
void fx3d::Settings::SetVelocitySet(fx3d::VelocitySet VSet)
{ 
    m_VSet = VSet;
    if (m_VSet == fx3d::VelocitySet::D2Q9)
    {
        m_VSetSize = 9u;
        m_VSetDims = 2u;
        m_VSetTransfer = 3u;
    }
    else if (m_VSet == fx3d::VelocitySet::D3Q15)
    {
        m_VSetSize = 15u;
        m_VSetDims = 3u;
        m_VSetTransfer = 5u;
    }
    else if (m_VSet == fx3d::VelocitySet::D3Q19)
    {   
        m_VSetSize = 19u;
        m_VSetDims = 3u;
        m_VSetTransfer = 5u;
    }
    else if (m_VSet == fx3d::VelocitySet::D3Q27)
    {
        m_VSetSize = 27u;
        m_VSetDims = 3u;
        m_VSetTransfer = 9u;
    }
}


// #ifdef SURFACE // (rho, u) need to be updated exactly every LBM step
// #define UPDATE_FIELDS // update (rho, u, T) in every LBM step
// #endif // SURFACE

// #ifdef PARTICLES // (rho, u) need to be updated exactly every LBM step
// #define UPDATE_FIELDS // update (rho, u, T) in every LBM step
// #endif // PARTICLES

// #ifdef TEMPERATURE
// #define VOLUME_FORCE
// #endif // TEMPERATURE
void fx3d::Settings::EnableFeature(fx3d::Feature Feat)
{
    m_Features = (fx3d::Feature)((int)m_Features | (int)Feat);
    if (((int)Feat & ((int)fx3d::Feature::SURFACE | (int)fx3d::Feature::PARTICLES)) != 0)
        EnableFeature(fx3d::Feature::UPDATE_FIELDS);
    if (((int)Feat & (int)fx3d::Feature::TEMPERATURE) != 0)
        EnableFeature(VOLUME_FORCE);
}

void fx3d::Settings::DisableFeature(fx3d::Feature Feat)
{
    m_Features = (fx3d::Feature)((int)m_Features & (~((int)Feat)));
    if (((int)Feat & (int)fx3d::Feature::VOLUME_FORCE) != 0)
        DisableFeature(fx3d::Feature::TEMPERATURE);
    if (((int)Feat & (int)fx3d::Feature::UPDATE_FIELDS) != 0)
    {
        DisableFeature(fx3d::Feature::SURFACE);
        DisableFeature(fx3d::Feature::PARTICLES);
    }
}

bool fx3d::Settings::IsFeatureEnabled(fx3d::Feature Feat)
{
    return ((int)m_Features & (int)Feat) != 0;
}