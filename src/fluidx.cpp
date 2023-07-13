#include "lbm.hpp"


int main(int argc, char** argv) {

	LBM lbm(128u, 256u, 256u, 0.005f, 0.0f, 0.0f, -0.0002f, 0.0001f);
    std::cout << "Created LBM" << std::endl;
	// ###################################################################################### define geometry ######################################################################################
	const uint Nx=lbm.get_Nx(), Ny=lbm.get_Ny(), Nz=lbm.get_Nz(); 
    for(ulong n=0ull; n<lbm.get_N(); n++) { 
        uint x=0u, y=0u, z=0u; 
        lbm.coordinates(n, x, y, z);
		if(z<Nz*6u/8u && y<Ny/8u) 
            lbm.flags[n] = TYPE_F;
		if(x==0u||x==Nx-1u||y==0u||y==Ny-1u||z==0u||z==Nz-1u) 
            lbm.flags[n] = TYPE_S; // all non periodic
	} // ######################################################################### run simulation, export images and data ##########################################################################
	std::cout << "Initialized" << std::endl;
    lbm.graphics.visualization_modes = lbm.get_D()==1u ? VIS_PHI_RAYTRACE : VIS_PHI_RASTERIZE;
    std::cout << "Selected vismode" << std::endl;

    uint i = 0;
    while (i < 5000u) {
        lbm.run(1u);
        // std::cout << lbm.rho.x[0] << std::endl;
        i++;
    }

    lbm.graphics.write_frame(get_exe_path()+"export/t/");

}

/*int main(int argc, char* argv[]) {
	main_arguments = get_main_arguments(argc, argv);
	camera = Camera(GRAPHICS_FRAME_WIDTH, GRAPHICS_FRAME_HEIGHT, 60u); // width and height must be divisible by 8
	thread compute_thread(main_physics); // start main_physics() in a new thread
	while(running) {
		// main loop ################################################################
		main_label(1.0);
		sleep(0.050);
		// ##########################################################################
	}
	compute_thread.join();
	return 0;
}*/