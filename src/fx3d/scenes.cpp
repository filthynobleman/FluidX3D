/**
 * @file        scenes.cpp
 * 
 * @brief       
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-07-14
 */
#include <fx3d/scenes.hpp>
#include <utils/shapes.hpp>

fx3d::LBMInitializer fx3d::DetermineScene(const std::string& Name)
{
     if (Name == "dam_break")
        return fx3d::DamBreakInit;
    else if (Name == "colliding_droplets")
        return fx3d::CollidingDropletsInit;
    
    std::cerr << "Unknown scene \"" << Name << "\"." << std::endl;
    return nullptr;
}


fx3d::LBM* fx3d::DamBreakInit(const nlohmann::json& json)
{
    fx3d::Settings::EnableFeature(Feature::VOLUME_FORCE);
    fx3d::Settings::EnableFeature(Feature::SURFACE);

    // ################################################################## define simulation box size, viscosity and volume force ###################################################################
	fx3d::LBM* lbm = new fx3d::LBM(128u, 256u, 256u, 0.005f, 0.0f, 0.0f, -0.0002f, 0.0001f);
	// ###################################################################################### define geometry ######################################################################################
	const uint Nx=lbm->get_Nx(), Ny=lbm->get_Ny(), Nz=lbm->get_Nz(); 
    for(ulong n=0ull; n < lbm->get_N(); n++) 
    { 
        uint x=0u, y=0u, z=0u; lbm->coordinates(n, x, y, z);
		if(z<Nz*6u/8u && y<Ny/8u) 
            lbm->flags[n] = TYPE_F;
		if(x==0u||x==Nx-1u||y==0u||y==Ny-1u||z==0u||z==Nz-1u) 
            lbm->flags[n] = TYPE_S; // all non periodic
	} // ######################################################################### run simulation, export images and data ##########################################################################
	lbm->graphics.visualization_modes = lbm->get_D()==1u ? VIS_PHI_RAYTRACE : VIS_PHI_RASTERIZE;

    return lbm;
}


fx3d::LBM* fx3d::CollidingDropletsInit(const nlohmann::json& json)
{
    fx3d::Settings::EnableFeature(Feature::VOLUME_FORCE);
    fx3d::Settings::EnableFeature(Feature::SURFACE);
    fx3d::Settings::EnableFeature(Feature::FORCE_FIELD);

    // ################################################################## define simulation box size, viscosity and volume force ###################################################################
	fx3d::LBM* lbm = new fx3d::LBM(256u, 256u, 128u, 0.014f, 0.0f, 0.0f, 0.0f, 0.0001f);
	// ###################################################################################### define geometry ######################################################################################
	const uint Nx=lbm->get_Nx(), Ny=lbm->get_Ny(), Nz=lbm->get_Nz(); 
    for(ulong n=0ull; n<lbm->get_N(); n++) 
    { 
        uint x=0u, y=0u, z=0u; lbm->coordinates(n, x, y, z);
		if(sphere(x, y, z, lbm->center()-float3(0u, 10u, 0u), 32.0f)) 
        {
			lbm->flags[n] = TYPE_F;
			lbm->u.y[n] = 0.025f;
		}
		if(sphere(x, y, z, lbm->center()+float3(30u, 40u, 0u), 12.0f)) 
        {
			lbm->flags[n] = TYPE_F;
			lbm->u.y[n] = -0.2f;
		}
		lbm->F.x[n] = -0.001f*lbm->relative_position(n).x;
		lbm->F.y[n] = -0.001f*lbm->relative_position(n).y;
		lbm->F.z[n] = -0.0005f*lbm->relative_position(n).z;
		if(x==0u||x==Nx-1u||y==0u||y==Ny-1u||z==0u||z==Nz-1u) 
            lbm->flags[n] = TYPE_S; // all non periodic
	} // ######################################################################### run simulation, export images and data ##########################################################################
	lbm->graphics.visualization_modes = lbm->get_D()==1u ? VIS_PHI_RAYTRACE : VIS_PHI_RASTERIZE;

    return lbm;
}



















/*void main_setup() { // dam break; required extensions in defines.hpp: FP16S, VOLUME_FORCE, SURFACE, INTERACTIVE_GRAPHICS
	// ################################################################## define simulation box size, viscosity and volume force ###################################################################
	LBM lbm(128u, 256u, 256u, 0.005f, 0.0f, 0.0f, -0.0002f, 0.0001f);
	// ###################################################################################### define geometry ######################################################################################
	const uint Nx=lbm->get_Nx(), Ny=lbm->get_Ny(), Nz=lbm->get_Nz(); for(ulong n=0ull; n<lbm->get_N(); n++) { uint x=0u, y=0u, z=0u; lbm->coordinates(n, x, y, z);
		if(z<Nz*6u/8u && y<Ny/8u) lbm->flags[n] = TYPE_F;
		if(x==0u||x==Nx-1u||y==0u||y==Ny-1u||z==0u||z==Nz-1u) lbm->flags[n] = TYPE_S; // all non periodic
	} // ######################################################################### run simulation, export images and data ##########################################################################
	lbm->graphics.visualization_modes = lbm->get_D()==1u ? VIS_PHI_RAYTRACE : VIS_PHI_RASTERIZE;
	lbm->run();
} /**/

/*void main_setup() { // two colliding droplets in force field; required extensions in defines.hpp: FP16S, VOLUME_FORCE, FORCE_FIELD, SURFACE, INTERACTIVE_GRAPHICS
	// ################################################################## define simulation box size, viscosity and volume force ###################################################################
	LBM lbm(256u, 256u, 128u, 0.014f, 0.0f, 0.0f, 0.0f, 0.0001f);
	// ###################################################################################### define geometry ######################################################################################
	const uint Nx=lbm->get_Nx(), Ny=lbm->get_Ny(), Nz=lbm->get_Nz(); for(ulong n=0ull; n<lbm->get_N(); n++) { uint x=0u, y=0u, z=0u; lbm->coordinates(n, x, y, z);
		if(sphere(x, y, z, lbm->center()-float3(0u, 10u, 0u), 32.0f)) {
			lbm->flags[n] = TYPE_F;
			lbm->u.y[n] = 0.025f;
		}
		if(sphere(x, y, z, lbm->center()+float3(30u, 40u, 0u), 12.0f)) {
			lbm->flags[n] = TYPE_F;
			lbm->u.y[n] = -0.2f;
		}
		lbm->F.x[n] = -0.001f*lbm->relative_position(n).x;
		lbm->F.y[n] = -0.001f*lbm->relative_position(n).y;
		lbm->F.z[n] = -0.0005f*lbm->relative_position(n).z;
		if(x==0u||x==Nx-1u||y==0u||y==Ny-1u||z==0u||z==Nz-1u) lbm->flags[n] = TYPE_S; // all non periodic
	} // ######################################################################### run simulation, export images and data ##########################################################################
	lbm->graphics.visualization_modes = lbm->get_D()==1u ? VIS_PHI_RAYTRACE : VIS_PHI_RASTERIZE;
	lbm->run();
} /**/