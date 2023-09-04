/**
 * @file        settings.hpp
 * 
 * @brief       A static class for defining global settings of the project.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-07-14
 */
#pragma once

#include <utils/utilities.hpp>


class Camera;

namespace fx3d
{
    
enum VelocitySet
{
    // choose D2Q9 velocity set for 2D; allocates 53 (FP32) or 35 (FP16) Bytes/node
    D2Q9,
    // choose D3Q15 velocity set for 3D; allocates 77 (FP32) or 47 (FP16) Bytes/node
    D3Q15,
    // choose D3Q19 velocity set for 3D; allocates 93 (FP32) or 55 (FP16) Bytes/node; (default)
    D3Q19,
    // choose D3Q27 velocity set for 3D; allocates 125 (FP32) or 71 (FP16) Bytes/node
    D3Q27
};

enum CollisionType
{
    // choose single-relaxation-time LBM collision operator; (default)
    SRT,
    // choose two-relaxation-time LBM collision operator
    TRT
};

enum DDFCompression
{
    // // compress LBM DDFs to range-shifted IEEE-754 FP16; number conversion is done in hardware; all arithmetic is still done in FP32
    // FP16S,
    // // compress LBM DDFs to more accurate custom FP16C format; number conversion is emulated in software; all arithmetic is still done in FP32
    // FP16C,
    // // no compression
    // NONE
};

enum Feature
{
    // enables global force per volume in one direction (equivalent to a pressure gradient); specified in the LBM class constructor; the force can be changed on-the-fly between time steps at no performance cost
    VOLUME_FORCE = 1,
    // enables computing the forces on solid boundaries with lbm.calculate_force_on_boundaries(); and enables setting the force for each lattice point independently (enable VOLUME_FORCE too); allocates an extra 12 Bytes/node
    FORCE_FIELD = 2,
    // enables fixing the velocity/density by marking nodes with TYPE_E; can be used for inflow/outflow; does not reflect shock waves
    EQUILIBRIUM_BOUNDARIES = 4,
    // enables moving solids: set solid nodes to TYPE_S and set their velocity u unequal to zero
    MOVING_BOUNDARIES = 8,
    // enables free surface LBM: mark fluid nodes with TYPE_F; at initialization the TYPE_I interface and TYPE_G gas domains will automatically be completed; allocates an extra 12 Bytes/node
    SURFACE = 16,
    // enables temperature extension; set fixed-temperature nodes with TYPE_T (similar to EQUILIBRIUM_BOUNDARIES); allocates an extra 32 (FP32) or 18 (FP16) Bytes/node
    TEMPERATURE = 32,
    // enables Smagorinsky-Lilly subgrid turbulence LES model to keep simulations with very large Reynolds number stable
    SUBGRID = 64,
    // enables particles with immersed-boundary method (for 2-way coupling also activate VOLUME_FORCE and FORCE_FIELD; only supported in single-GPU)
    PARTICLES = 128,
    // update (rho, u, T) in every LBM step
    UPDATE_FIELDS = 256
};



/**
 * @brief       Static class containing all the simulation settings.
 * 
 * @details     This static class groups and defines all the settings that could be used by the simulation.
 */
class Settings
{
private:
    static VelocitySet m_VSet;      // fx3d::VelocitySet::D3Q19
    static CollisionType m_CollType;       // fx3d::CollisionType::SRT
    static DDFCompression m_Compr;  // fx3d::DDFCompression::FP16S
    static Feature m_Features;

    static unsigned int m_VSetSize;
    static unsigned int m_VSetDims;
    static unsigned int m_VSetTransfer;


    Settings() = delete;
    ~Settings() = delete;
public:
    static VelocitySet GetVelocitySet();
    static unsigned int GetVSetSize();
    static unsigned int GetVSetDims();
    static unsigned int GetVSetTransfer();
    static void SetVelocitySet(VelocitySet VSet);

    static CollisionType GetCollisionType();
    static void SetCollisionType(CollisionType CType);

    static DDFCompression GetDDFCompression();
    static void SetDDFCompression(DDFCompression Compression);
    
    static void EnableFeature(Feature Feat);
    static void DisableFeature(Feature Feat);
    static bool IsFeatureEnabled(Feature Feat);
};


/**
 * @brief       Static class containing the graphics settings
 * 
 * @details     This static class contains and defines all the settings that could be used by the renderer.
 */
class GraphicsSettings
{
private:
    static int m_Width;
    static int m_Height;
    static int m_BGColor;
    static float m_Alpha;
    static bool m_AlphaEnabled;
    static float m_UMax;
    static float m_QCrit;
    static float m_FMax;
    static int m_SLineSparse;
    static int m_SLineLength;
    static float m_RTTrans;
    static int m_RTColor;
    static std::string m_SkyboxPath;

    static Camera* m_Cam;

    GraphicsSettings() = delete;
    ~GraphicsSettings() = delete;
public:
    static int GetWidth();
    static int GetHeight();

    static int GetBackgroundColor();
    static int GetBackgroundRed();
    static int GetBackgroundGreen();
    static int GetBackgroundBlue();

    static float GetAlpha();
    static bool IsAlphaEnabled();

    static float GetUMax();
    static float GetQCriterion();
    static float GetFMax();
    
    static int GetStreamlineSparse();
    static int GetStreamlineLength();

    static float GetRaytracingTransmittance();

    static int GetRaytracingColor();
    static int GetRaytracingRed();
    static int GetRaytracingGreen();
    static int GetRaytracingBlue();

    static std::string GetSkyboxPath();

    static Camera& GetCamera();



    static void SetWidth(int W);
    static void SetHeight(int H);

    static void SetBackgroundColor(int Color);
    static void SetBackgroundRed(char Red);
    static void SetBackgroundGreen(char Green);
    static void SetBackgroundBlue(char Blue);

    static void SetAlpha(float Alpha);
    static void UseAlpha(bool Activate);

    static void SetUMax(float UMax);
    static void SetQCriterion(float QCrit);
    static void SetFMax(float FMax);
    
    static void SetStreamlineSparse(int SLS);
    static void SetStreamlineLength(int SLL);

    static void SetRaytracingTransmittance(float RTTrans);

    static void SetRaytracingColor(int Color);
    static void SetRaytracingRed(char Red);
    static void SetRaytracingGreen(char Green);
    static void SetRaytracingBlue(char Blue);

    static void SetSkyboxPath(std::string& SkyboxPath);
};


} // namespace fx3d
