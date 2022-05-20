#include <cgv_gl/gl/gl.h>
#include <cgv/base/node.h>
#include <cgv/render/drawable.h>
#include <cgv_gl/rectangle_renderer.h>

/**
 * Procedural Plot Plugin
 **/
class procedural_plot :
        public cgv::base::node,
        public cgv::render::drawable
{
    cgv::render::shader_program shaderProgram;
public:
	procedural_plot() : node("procedural_plot")
	{
		std::cout << "Hallo" << std::endl;
	}
	std::string get_type_name() const override
	{
		return "procedural_plot";
	}
    bool init(cgv::render::context &ctx) override
    {
        auto &rcr = cgv::render::ref_rectangle_renderer(ctx, 1);
        if(!shaderProgram.build_program(ctx, "procedural_plot.glpr", 1))
            throw;
        rcr.set_prog(shaderProgram);
        bool success = rcr.ref_prog().is_linked();
        return success;
    }
    void draw(cgv::render::context &ctx) override
    {
        auto &rcr = cgv::render::ref_rectangle_renderer(ctx, 0);
        rcr.set_prog(shaderProgram);
        rcr.set_position(ctx, vec2(0.0f, 0.0f));
        rcr.set_extent(ctx, vec2(4.0f, 2.0f));
        rcr.set_color(ctx, vec3(1.0f, 1.0f, 1.0f));
        rcr.render(ctx, 0, 1);
    }
};

#include <cgv/base/register.h>
cgv::base::factory_registration<procedural_plot> procedural_plot("new/demo/procedural_plot", 'N');
