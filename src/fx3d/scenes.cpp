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

/*
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
*/


bool fx3d::Scene::is_boundary(uint x, uint y, uint z) const {
	return x==0u || /*x==lbm->get_Nx()-1u ||*/ y==0u || y==lbm->get_Ny()-1u || z==0u || z==lbm->get_Nz()-1u;
}

bool fx3d::Scene::is_fluid(uint x, uint y, uint z) const {
    return false;
}

bool fx3d::Scene::is_static(uint x, uint y, uint z) const {
    return false;
}

void fx3d::Scene::make_obstacle(const Mesh* geometry) {
	this->lbm->flags.write_to_device();
	this->lbm->voxelize_mesh_on_device(geometry, TYPE_S);
	this->lbm->flags.read_from_device();
}

void fx3d::Scene::make_fluid(const Mesh* geometry) {
	this->lbm->flags.write_to_device();
	this->lbm->voxelize_mesh_on_device(geometry, TYPE_F);
	this->lbm->flags.read_from_device();
}

Mesh *fx3d::Scene::load_mesh_stl(const string &stl_path, const float3 &center, const float3x3 &rotation, const float3& scale, const float size) const {
    auto out = read_stl(stl_path, this->lbm->size(), center, rotation, size);
	out->scale(scale);
	return out;
}

void fx3d::Scene::run() {
	for (uint t = 0; t < sim_steps; t++) {
		this->lbm->run(update_dt);
		if (is_current_frame_output(sim_steps, out_seconds, fps)) {
			export_frame();
		}
	}
}

void fx3d::Scene::step() {
	this->lbm->run(1u);
}

void fx3d::Scene::export_frame() {
	render_current_frame(out_dir, "frame");
}

void fx3d::Scene::render_current_frame(const std::string &out_dir, const std::string &name) const
{
    this->lbm->graphics.write_frame(out_dir, name);
}

bool fx3d::Scene::is_current_frame_output(const ulong sim_steps, const float out_len_s, const float fps) const {
    return this->lbm->graphics.next_frame(sim_steps, out_len_s, fps);
}

fx3d::Scene::Scene(const nlohmann::json &config) {

	/* Simulation parameters + create LBM */

	config_sim_params(config);

	this->lbm = new LBM(Nx, Ny, Nz, nu, fx, fy, fz, sigma, alpha, beta, particles_N, particles_rho);

	/* Obstacles and fluid bodies */

	config_obstacles(config);
	config_fluid_bodies(config);
	
	/* Ad-hoc grid definition */

	custom_grid_initialization();

	/* Graphics config and initialize */

	config_graphics(config);
	lbm->run(0u);

	/* Export configuration */

	config_export(config);

}

fx3d::Scene::~Scene() {
	delete this->lbm;
}


void fx3d::Scene::custom_grid_initialization() {
    for (ulong n=0ull; n < this->lbm->get_N(); n++) { 
        uint x=0u, y=0u, z=0u; 
		lbm->coordinates(n, x, y, z);
		if (is_fluid(x, y, z)) {
			lbm->flags[n] = TYPE_F;
		} else if (is_boundary(x, y, z) || is_static(x, y, z)) {
			lbm->flags[n] = TYPE_S;
		}
	}
}

void fx3d::Scene::config_sim_params(const nlohmann::json &config) {    
	if (config.contains("sim_params")) {
		nlohmann::json sim_config = config["sim_params"];
		Nx = sim_config.contains("Nx") ? sim_config["Nx"] : Nx;
		Ny = sim_config.contains("Ny") ? sim_config["Ny"] : Ny;
		Nz = sim_config.contains("Nz") ? sim_config["Nz"] : Nz;
		nu = sim_config.contains("nu") ? sim_config["nu"] : nu;
		sigma = sim_config.contains("sigma") ? sim_config["sigma"] : sigma;
		alpha = sim_config.contains("alpha") ? sim_config["alpha"] : alpha;
		beta = sim_config.contains("beta") ? sim_config["beta"] : beta;
		fx = sim_config.contains("fx") ? sim_config["fx"] : fx;
		fy = sim_config.contains("fy") ? sim_config["fy"] : fy;
		fz = sim_config.contains("fz") ? sim_config["fz"] : fz;
		particles_N = sim_config.contains("P_n") ? sim_config["P_n"] : particles_N;
		particles_rho = sim_config.contains("P_rho") ? sim_config["P_rho"] : particles_rho;
	} 
}

void fx3d::Scene::config_obstacles(const nlohmann::json &config) {
	if (config.contains("obstacles")) {
		nlohmann::json obstacles = config["obstacles"];
		for (auto ptr = obstacles.begin(); ptr != obstacles.end(); ptr++) {
			auto obst = *ptr;
			std::vector<float> center = obst["center"];
			std::vector<float> rotation = obst["rotation"];
			std::vector<float> scale = obst["scale"];
			const Mesh* geometry = load_mesh_stl(
				obst["mesh_path"], float3(center[0], center[1], center[2]),
				euler(float3(rotation[0], rotation[1], rotation[2])), 
				float3(scale[0], scale[1], scale[2]), (float)obst["size"]
			);
			make_obstacle(geometry);
			delete geometry;
		}
	}
}

void fx3d::Scene::config_fluid_bodies(const nlohmann::json &config) {
	if (config.contains("fluids")) {
		nlohmann::json fluids = config["fluids"];
		for (auto ptr = fluids.begin(); ptr != fluids.end(); ptr++) {
			auto fluid = *ptr;
			std::vector<float> center = fluid["center"];
			std::vector<float> rotation = fluid["rotation"];
			std::vector<float> scale = fluid["scale"];
			const Mesh* geometry = load_mesh_stl(
				fluid["mesh_path"], float3(center[0], center[1], center[2]),
				euler(float3(rotation[0], rotation[1], rotation[2])), 
				float3(scale[0], scale[1], scale[2]), (float)fluid["size"]
			);
			make_fluid(geometry);	
			delete geometry;		
		}
	}
}

void fx3d::Scene::config_export(const nlohmann::json &config) {
	if (config.contains("export")) {
		nlohmann::json exp_config = config["export"];
		out_dir = exp_config["out_dir"];
		fps = exp_config["fps"];
		sim_steps = exp_config["simulation_steps"];
		update_dt = exp_config["update_dt"];
		out_seconds = exp_config["video_seconds"];
	}
}

void fx3d::Scene::config_graphics(const nlohmann::json &config) {
	lbm->graphics.visualization_modes = VIS_PHI_RASTERIZE|VIS_FLAG_SURFACE;
	// lbm->graphics.set_camera_centered(0.0f, -0.5f);
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