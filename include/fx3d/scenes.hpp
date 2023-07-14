/**
 * @file        scenes.hpp
 * 
 * @brief       Declaration of the functions for the initialization of the LBM objects.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-07-14
 */
#pragma once

#include <fx3d/lbm.hpp>
#include <nlohmann/json.hpp>

namespace fx3d
{
    
typedef fx3d::LBM*(*LBMInitializer)(const nlohmann::json& json);

LBMInitializer DetermineScene(const std::string& Name);

fx3d::LBM* DamBreakInit(const nlohmann::json& json);
fx3d::LBM* CollidingDropletsInit(const nlohmann::json& json);


} // namespace fx3d
