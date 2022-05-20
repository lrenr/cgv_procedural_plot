#include <cgv_gl/gl/gl.h>
#include <cgv/base/node.h>
#include <cgv/render/drawable.h>
#include <cgv/gui/provider.h>
#include <cgv_gl/rectangle_renderer.h>

/**
 * Procedural Plot Plugin
 **/
class procedural_plot :
        public cgv::base::node,
        public cgv::render::drawable,
        public cgv::gui::provider
{
    cgv::render::shader_program shaderProgram;
    bool is_red = false;
    cgv::render::shader_define_map current_defines;
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
        const cgv::render::shader_define_map defines = {{"IS_RED", std::to_string(is_red)}};
        if(!shaderProgram.build_program(ctx, "procedural_plot.glpr", 1, defines))
            throw;
        current_defines = defines;
        rcr.set_prog(shaderProgram);
        bool success = rcr.ref_prog().is_linked();
        return success;
    }
    void draw(cgv::render::context &ctx) override
    {
        const cgv::render::shader_define_map defines = {{"IS_RED", std::to_string(is_red)}};
        if(defines != current_defines)
        {
            shaderProgram.destruct(ctx);
            shaderProgram.build_program(ctx, "procedural_plot.glpr", 1, defines);
            current_defines = defines;
        }
        auto &rcr = cgv::render::ref_rectangle_renderer(ctx, 0);
        rcr.set_prog(shaderProgram);
        rcr.set_position(ctx, vec2(0.0f, 0.0f));
        rcr.set_extent(ctx, vec2(4.0f, 2.0f));
        rcr.set_color(ctx, vec3(1.0f, 1.0f, 1.0f));
        rcr.render(ctx, 0, 1);
    }
    void on_set(void* member_ptr) override
    {
        post_redraw();
        update_member(member_ptr);
    }
    void create_gui() override
    {
        add_decorator("procedural_plot", "heading");
        add_member_control(this, "is_red", is_red, "toggle");
    }
};

#include <cgv/base/register.h>
cgv::base::factory_registration<procedural_plot> procedural_plot("new/demo/procedural_plot", 'N');
