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
fx3d::RelaxTime fx3d::Settings::m_RTime         = fx3d::RelaxTime::SRT;
fx3d::DDFCompression fx3d::Settings::m_Compr    = fx3d::DDFCompression::FP16S;
fx3d::Feature fx3d::Settings::m_Features        = (fx3d::Feature)0;



fx3d::VelocitySet fx3d::Settings::GetVelocitySet() { return m_VSet; }
fx3d::RelaxTime fx3d::Settings::GetRelaxTime() { return m_RTime; }
fx3d::DDFCompression fx3d::Settings::GetDDFCompression() { return m_Compr; }

void fx3d::Settings::SetVelocitySet(fx3d::VelocitySet VSet) { m_VSet = VSet; }
void fx3d::Settings::SetRelaxTime(fx3d::RelaxTime RTime) { m_RTime = RTime; }
void fx3d::Settings::SetDDFCompression(fx3d::DDFCompression Compr) { m_Compr = Compr; }


void fx3d::Settings::EnableFeature(fx3d::Feature Feat)
{
    m_Features = (fx3d::Feature)((int)m_Features | (int)Feat);
}

void fx3d::Settings::DisableFeature(fx3d::Feature Feat)
{
    m_Features = (fx3d::Feature)((int)m_Features & (~((int)Feat)));
}

bool fx3d::Settings::IsFeatureEnabled(fx3d::Feature Feat)
{
    return ((int)m_Features & (int)Feat) != 0;
}