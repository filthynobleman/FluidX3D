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
#include <vector>


namespace fx3d
{
/*    
typedef fx3d::LBM*(*LBMInitializer)(const nlohmann::json& json);

LBMInitializer DetermineScene(const std::string& Name);

fx3d::LBM* DamBreakInit(const nlohmann::json& json);
fx3d::LBM* CollidingDropletsInit(const nlohmann::json& json);
*/


class Scene {

private:

    /* Export */
    std::string out_dir = ".\\export";
    uint fps = 30u;
    uint sim_steps = 1u;
    uint update_dt = 1u;
    float out_seconds = 0.0f;

    /* Simulation parameters */
	uint Nx = 1u, Ny = 1u, Nz = 1u;
	float nu = 1.0f/6.0f, sigma = 0.0f, alpha = 0.0f, beta = 0.0f;
	float fx = 0.0f, fy = 0.0f, fz = 0.0f;
	uint particles_N = 0u;
	float particles_rho = 0.0f;

protected:

    LBM* lbm;

    const std::string get_out_dir();
    
    Mesh* load_mesh_stl(const string& stl_path, const float3& center, const float3x3& rotation, const float3& scale, const float size) const;

    virtual void enable_features();
    virtual void config_sim_params(const nlohmann::json &config);
    virtual void config_obstacles(const nlohmann::json &config);
    virtual void config_fluid_bodies(const nlohmann::json &config);
    virtual void config_export(const nlohmann::json &config);
    virtual void config_graphics(const nlohmann::json &config);
    virtual void select_rendering_mode(const nlohmann::json &config);
    virtual void custom_grid_initialization();

    virtual void postprocess();
    
    struct Cuboid {

        float3 center;
        float3 rotation;
        float3 sides;

        Cuboid(const float3& center, const float3& rotation, const float3& sides) : 
            center(center), rotation(rotation), sides(sides) {}

    };

    struct Sphere {

        float3 center;
        float  radius;

        Sphere(const float3& center, const float radius) : 
            center(center), radius(radius) {}

    };

    std::vector<const Mesh*> mesh_obst;
    std::vector<Cuboid>      cuboid_obst;
    std::vector<Sphere>      sphere_obst;
    std::vector<Cuboid>      cuboid_fluid;
    std::vector<Sphere>      sphere_fluid;

public:

    void make_fluid(const Mesh* geometry);
    void make_obstacle(const Mesh* geometry);
    
    virtual bool is_fluid(uint x, uint y, uint z) const;
    virtual bool is_static(uint x, uint y, uint z) const;
    virtual bool is_boundary(uint x, uint y, uint z) const;

    void run();
    void step();

    virtual void export_frame();
    void render_current_frame(const std::string& out_png, const std::string &name) const;
    bool is_current_frame_output(const ulong sim_steps, const float out_len_s, const float fps) const;

    void configure(const nlohmann::json& config);
    Scene() = default;
    ~Scene();

};



} // namespace fx3d
