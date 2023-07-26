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



bool fx3d::Scene::is_boundary(uint x, uint y, uint z) {
	return x==0u || x==lbm->get_Nx()-1u || y==0u || y==lbm->get_Ny()-1u || z==0u || z==lbm->get_Nz()-1u;
}

void fx3d::Scene::instantiate() {

}

fx3d::Scene::Scene(const nlohmann::json& config) {
	uint Nx = 1, Ny = 1, Nz = 1;
	float nu = 1.0f/6.0f, sigma=0.0f, alpha=0.0f, beta=0.0f;
	float fx = 0.0f, fy=0.0f, fz = 0.0f;
	uint particles_N = 0u;
	float particles_rho = 0.0f;
	
	if (config.contains("sim_params")) {
		nlohmann::json sim_config = config["sim_params"];
		Nx = sim_config.contains("Nx") ? sim_config["Nx"] : 1u;
		Ny = sim_config.contains("Ny") ? sim_config["Ny"] : 1u;
		Nz = sim_config.contains("Nz") ? sim_config["Nz"] : 1u;
		nu = sim_config.contains("nu") ? sim_config["nu"] : 1.0f/6.0f;
		sigma = sim_config.contains("sigma") ? sim_config["sigma"] : 0.0f;
		alpha = sim_config.contains("alpha") ? sim_config["alpha"] : 0.0f;
		beta = sim_config.contains("beta") ? sim_config["beta"] : 0.0f;
		fx = sim_config.contains("fx") ? sim_config["fx"] : 0.0f;
		fy = sim_config.contains("fy") ? sim_config["fy"] : 0.0f;
		fz = sim_config.contains("fz") ? sim_config["fz"] : 0.0f;
		particles_N = sim_config.contains("P_n") ? sim_config["P_n"] : 0u;
		particles_rho = sim_config.contains("P_rho") ? sim_config["P_rho"] : 0.0f;
	} else {
		// Big error
	}

	lbm = new LBM(Nx, Ny, Nz, nu, fx, fy, fz, sigma, alpha, beta, particles_N, particles_rho);
}

fx3d::Scene::~Scene() {
	delete lbm;
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