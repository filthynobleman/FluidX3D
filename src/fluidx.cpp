#include <fx3d/lbm.hpp>
#include <fx3d/settings.hpp>
#include <fx3d/scenes.hpp>

int main(int argc, char** argv) {

	// fx3d::Settings::EnableFeature(fx3d::Feature::VOLUME_FORCE);
	// fx3d::Settings::EnableFeature(fx3d::Feature::SURFACE);

	// fx3d::LBM lbm(128u, 256u, 256u, 0.005f, 0.0f, 0.0f, -0.0002f, 0.0001f);
    // std::cout << "Created LBM" << std::endl;
	// // ###################################################################################### define geometry ######################################################################################
	// const uint Nx=lbm.get_Nx(), Ny=lbm.get_Ny(), Nz=lbm.get_Nz(); 
    // for(ulong n=0ull; n<lbm.get_N(); n++) { 
    //     uint x=0u, y=0u, z=0u; 
    //     lbm.coordinates(n, x, y, z);
	// 	if(z<Nz*6u/8u && y<Ny/8u) 
    //         lbm.flags[n] = TYPE_F;
	// 	if(x==0u||x==Nx-1u||y==0u||y==Ny-1u||z==0u||z==Nz-1u) 
    //         lbm.flags[n] = TYPE_S; // all non periodic
	// } // ######################################################################### run simulation, export images and data ##########################################################################
	// std::cout << "Initialized" << std::endl;
    // lbm.graphics.visualization_modes = lbm.get_D()==1u ? VIS_PHI_RAYTRACE : VIS_PHI_RASTERIZE;
    // std::cout << "Selected vismode" << std::endl;

	if (argc < 2)
	{
		std::cerr << "No input arguments." << std::endl;
		return -1;
	}

	std::ifstream Stream(argv[1]);
	nlohmann::json j = nlohmann::json::parse(Stream);

	// if (!j.contains("scene"))
	// {
	// 	std::cerr << "Config file does not contain the \"scene\" attribute." << std::endl;
	// 	return -1;
	// }
	// if (!j["scene"].is_string())
	// {
	// 	std::cerr << "Attribute \"scene\" in the config file is not a string." << std::endl;
	// 	return -1;
	// }

	unsigned int NFrames = 5000u;
	if (j.contains("num_frames"))
	{
		if (!j["num_frames"].is_number_integer())
		{
			std::cerr << "Attribute \"num_frames\" must be have an integer value." << std::endl;
			std::cerr << "Using default value of " << NFrames << '.' << std::endl;
		}
		else
		{
			NFrames = j["num_frames"];
		}
	}
	else
	{
		std::cerr << "Attribute \"num_frames\" is not defined." << std::endl;
		std::cerr << "Using default value of " << NFrames << '.' << std::endl;
	}

	if (!j.contains("scene"))
	{
		std::cerr << "Attribute \"scene\" is not defined." << std::endl;
		return -1;
	}
	if (!j["scene"].is_string())
	{
		std::cerr << "Attribute \"scene\" is not a string." << std::endl;
		return -1;
	}

	fx3d::LBMInitializer LBMInit = fx3d::DetermineScene(j["scene"]);
	fx3d::LBM* lbm = LBMInit(j);
	if (lbm == nullptr)
		return -1;

    uint i = 0;
    while (i < NFrames) {
        lbm->run(1u);
        // std::cout << lbm.rho.x[0] << std::endl;
        i++;
    }

    lbm->graphics.write_frame(get_exe_path()+"export/t/");

	delete lbm;

	return 0;
}
