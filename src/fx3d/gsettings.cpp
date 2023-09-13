/**
 * @file        gsettings.cpp
 * 
 * @brief       Implements fx3d::GraphicsSettings.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-07-14
 */
#include <fx3d/settings.hpp>
#include <utils/graphics.hpp>


int fx3d::GraphicsSettings::m_Width                 = 1920;
int fx3d::GraphicsSettings::m_Height                = 1080;

int fx3d::GraphicsSettings::m_BGColor               = 0x000000;

float fx3d::GraphicsSettings::m_Alpha               = 0.7f;
bool fx3d::GraphicsSettings::m_AlphaEnabled         = false;

float fx3d::GraphicsSettings::m_UMax                = 0.25f;
float fx3d::GraphicsSettings::m_QCrit               = 0.0001f;
float fx3d::GraphicsSettings::m_FMax                = 0.002f;

int fx3d::GraphicsSettings::m_SLineSparse           = 4;
int fx3d::GraphicsSettings::m_SLineLength           = 128;

float fx3d::GraphicsSettings::m_RTTrans             = 0.25f;
int fx3d::GraphicsSettings::m_RTColor               = 0x005F7F;

int fx3d::GraphicsSettings::m_FluidMaterial         = -1;

Camera* fx3d::GraphicsSettings::m_Cam               = new Camera();

std::string fx3d::GraphicsSettings::m_SkyboxPath    = "";


Camera& fx3d::GraphicsSettings::GetCamera() { return *m_Cam; }


int fx3d::GraphicsSettings::GetWidth() { return m_Width; }
int fx3d::GraphicsSettings::GetHeight() { return m_Height; }
void fx3d::GraphicsSettings::SetWidth(int W) 
{ 
    m_Width = W;
    m_Cam->width = m_Width;
    m_Cam->update_matrix();
    m_Cam->set_zoom(m_Cam->zrad);
}
void fx3d::GraphicsSettings::SetHeight(int H)
{ 
    m_Height = H; 
    m_Cam->height = m_Height;
    m_Cam->update_matrix();
    m_Cam->set_zoom(m_Cam->zrad);
}


float fx3d::GraphicsSettings::GetAlpha() { return m_Alpha; }
bool fx3d::GraphicsSettings::IsAlphaEnabled() { return m_AlphaEnabled; }
void fx3d::GraphicsSettings::SetAlpha(float Alpha) { m_Alpha = Alpha; }
void fx3d::GraphicsSettings::UseAlpha(bool Activate) { m_AlphaEnabled = Activate; }


float fx3d::GraphicsSettings::GetUMax() { return m_UMax; }
float fx3d::GraphicsSettings::GetQCriterion() { return m_QCrit; }
float fx3d::GraphicsSettings::GetFMax() { return m_FMax; }
void fx3d::GraphicsSettings::SetUMax(float UMax) { m_UMax = UMax; }
void fx3d::GraphicsSettings::SetQCriterion(float QCrit) { m_QCrit = QCrit; }
void fx3d::GraphicsSettings::SetFMax(float FMax) { m_FMax = FMax; }


int fx3d::GraphicsSettings::GetStreamlineSparse() { return m_SLineSparse; }
int fx3d::GraphicsSettings::GetStreamlineLength() { return m_SLineLength; }
void fx3d::GraphicsSettings::SetStreamlineSparse(int SLS) { m_SLineSparse = SLS; }
void fx3d::GraphicsSettings::SetStreamlineLength(int SLL) { m_SLineLength = SLL; }


float fx3d::GraphicsSettings::GetRaytracingTransmittance() { return m_RTTrans; }
void fx3d::GraphicsSettings::SetRaytracingTransmittance(float RTTrans) { m_RTTrans = RTTrans; }


int fx3d::GraphicsSettings::GetBackgroundColor() { return m_BGColor; }
int fx3d::GraphicsSettings::GetBackgroundRed()
{
    return (m_BGColor & 0x00ff0000) >> 16;
}
int fx3d::GraphicsSettings::GetBackgroundGreen()
{
    return (m_BGColor & 0x0000ff00) >> 8;
}
int fx3d::GraphicsSettings::GetBackgroundBlue()
{
    return m_BGColor & 0x000000ff;
}

void fx3d::GraphicsSettings::SetBackgroundColor(int BGColor) { m_BGColor = BGColor; }
void fx3d::GraphicsSettings::SetBackgroundRed(char Red)
{
    m_BGColor = m_BGColor & 0xff00ffff;
    m_BGColor = m_BGColor | (((int)Red) << 16);
}
void fx3d::GraphicsSettings::SetBackgroundGreen(char Green)
{
    m_BGColor = m_BGColor & 0xffff00ff;
    m_BGColor = m_BGColor | (((int)Green) << 8);
}
void fx3d::GraphicsSettings::SetBackgroundBlue(char Blue)
{
    m_BGColor = m_BGColor & 0xffffff00;
    m_BGColor = m_BGColor | ((int)Blue);
}


int fx3d::GraphicsSettings::GetRaytracingColor() { return m_RTColor; }
int fx3d::GraphicsSettings::GetRaytracingRed()
{
    return (m_RTColor & 0x00ff0000) >> 16;
}
int fx3d::GraphicsSettings::GetRaytracingGreen()
{
    return (m_RTColor & 0x0000ff00) >> 8;
}
int fx3d::GraphicsSettings::GetRaytracingBlue()
{
    return m_RTColor & 0x000000ff;
}
int fx3d::GraphicsSettings::GetFluidMaterial() {
    return m_FluidMaterial;
}

void fx3d::GraphicsSettings::SetRaytracingColor(int RTColor) { m_RTColor = RTColor; }
void fx3d::GraphicsSettings::SetRaytracingRed(char Red)
{
    m_RTColor = m_RTColor & 0xff00ffff;
    m_RTColor = m_RTColor | (((int)Red) << 16);
}
void fx3d::GraphicsSettings::SetRaytracingGreen(char Green)
{
    m_RTColor = m_RTColor & 0xffff00ff;
    m_RTColor = m_RTColor | (((int)Green) << 8);
}
void fx3d::GraphicsSettings::SetRaytracingBlue(char Blue)
{
    m_RTColor = m_RTColor & 0xffffff00;
    m_RTColor = m_RTColor | ((int)Blue);
}
void fx3d::GraphicsSettings::SetFluidMaterial(int Mat) {
    m_FluidMaterial = Mat;
}



std::string fx3d::GraphicsSettings::GetSkyboxPath() {
    return m_SkyboxPath;
}

void fx3d::GraphicsSettings::SetSkyboxPath(std::string& SkyboxPath) {
    m_SkyboxPath = SkyboxPath;
}