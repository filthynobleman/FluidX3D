#include <fx3d/lbm.hpp>
#include <fx3d/settings.hpp>
#include <fx3d/scenes.hpp>
#include <fx3d/info.hpp>


void simulation_thread() {
	fx3d::info.print_logo();

	std::ifstream Stream(main_arguments[0]);
	nlohmann::json j = nlohmann::json::parse(Stream);

    fx3d::Settings::EnableFeature(fx3d::Feature::VOLUME_FORCE);
    fx3d::Settings::EnableFeature(fx3d::Feature::SURFACE);

	fx3d::Scene scene;
	scene.configure(j);

	scene.run();

	running = false;
	exit(0); // make sure that the program stops
}


int main(int argc, char** argv) {

	if (argc < 2)
	{
		std::cerr << "No input arguments." << std::endl;
		return -1;
	}

	main_arguments = get_main_arguments(argc, argv);
	thread compute_thread(simulation_thread);
	do { // main console loop
		fx3d::info.print_update();
		sleep(0.050);
	} while(running);
	compute_thread.join();

	return 0;
}
