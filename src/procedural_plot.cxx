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
private:
    const vec2 RECT_EXTEND = {4.0f, 2.0f}, RECT_POSITION = {0.0f, 0.0f};
    cgv::render::shader_program shader_program;
    cgv::render::shader_define_map current_defines;
    bool is_red = false;
    bool is_hidden = false;
    bool show_grid = true;
    float domain_alpha = 1.0f;
    float grid_strength = 60.0f;
    float grid_scale = 1.0f;
    float grid_filter = 0.0f;

    template<typename T>
    void set_uniform(cgv::render::context &ctx, const std::basic_string<char> &name, const T &value)
    {
        shader_program.set_uniform(ctx, shader_program.get_uniform_location(ctx, name), value);
    }
public:
	procedural_plot() : node("procedural_plot")
	{
		std::cout << "Hallo" << std::endl;
	}
	std::string get_type_name() const override
	{
		return "procedural_plot";
	}
    void draw(cgv::render::context &ctx) override
    {
        GLboolean line_smooth = glIsEnabled(GL_LINE_SMOOTH);
        GLboolean blend = glIsEnabled(GL_BLEND);
        GLboolean cull_face = glIsEnabled(GL_CULL_FACE);
        GLenum blend_src, blend_dst, depth;
        glGetIntegerv(GL_BLEND_DST, reinterpret_cast<GLint*>(&blend_dst));
        glGetIntegerv(GL_BLEND_SRC, reinterpret_cast<GLint*>(&blend_src));
        glGetIntegerv(GL_DEPTH_FUNC, reinterpret_cast<GLint*>(&depth));

        glEnable(GL_LINE_SMOOTH);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);

        const cgv::render::shader_define_map defines = {
                {"IS_RED", std::to_string(is_red)},
                {"IS_HIDDEN", std::to_string(is_hidden)},
                {"SHOW_GRID", std::to_string(show_grid)}
        };
        if(defines != current_defines)
        {
            shader_program.destruct(ctx);
            shader_program.build_program(ctx, "procedural_plot.glpr", true, defines);
            current_defines = defines;
            shader_program.allow_context_to_set_color(false);
        }
        set_uniform(ctx, "alpha_test_val", domain_alpha);
        set_uniform(ctx, "domain_extent", RECT_EXTEND);
        set_uniform(ctx, "grid_strength", grid_strength);
        set_uniform(ctx, "grid_scale", grid_scale);
        set_uniform(ctx, "grid_filter", grid_filter);
        auto &rcr = cgv::render::ref_rectangle_renderer(ctx, 0);
        rcr.set_prog(shader_program);
        rcr.set_position(ctx, RECT_POSITION);
        rcr.set_extent(ctx, RECT_EXTEND);
        rcr.render(ctx, 0, 1);

        if (!line_smooth)
            glDisable(GL_LINE_SMOOTH);
        if (!blend)
            glDisable(GL_BLEND);
        if (cull_face)
            glEnable(GL_CULL_FACE);
        glDepthFunc(depth);
        glBlendFunc(blend_src, blend_dst);
    }
    void on_set(void* member_ptr) override
    {
        post_redraw();
        update_member(member_ptr);
    }
    void create_gui() override
    {
        add_decorator("procedural_plot", "heading");
        add_member_control(this, "is_red", is_red, "toggle", "w=60", " ");
        add_member_control(this, "is_hidden", is_hidden, "toggle", "w=60", " ");
        add_member_control(this, "show_grid", show_grid, "toggle", "w=60");
        add_member_control(this, "domain_alpha", domain_alpha, "value_slider", "min=0;max=1;step=0.001;ticks=true");
        add_member_control(this, "grid_strength", grid_strength, "value_slider", "min=10;max=200;step=1;ticks=true");
        add_member_control(this, "grid_scale", grid_scale, "value_slider", "min=0.1;max=10;step=0.1;ticks=true");
        add_member_control(this, "grid_filter", grid_filter, "value_slider", "min=0;max=0.01;step=0.0001;ticks=true");
    }
};

#include <cgv/base/register.h>
cgv::base::factory_registration<procedural_plot> procedural_plot("new/demo/procedural_plot", 'N');
