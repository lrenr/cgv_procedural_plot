#include <cgv_gl/gl/gl.h>
#include <cgv/base/node.h>
#include <cgv/render/drawable.h>
#include <cgv/gui/provider.h>
#include <cgv_gl/rectangle_renderer.h>
#include <cgv/render/vertex_buffer.h>
#include <cgv/math/fvec.h>

/**
 * Procedural Plot Plugin
 **/
class procedural_plot :
        public cgv::base::node,
        public cgv::render::drawable,
        public cgv::gui::provider
{
private:
    struct Graph_Param
    {
        int vertex_count;
        cgv::math::fvec<float, 4> color;
        cgv::math::fvec<float, 2> scale;
    };
    const vec2 RECT_EXTEND = {4.0f, 2.0f}, RECT_POSITION = {0.0f, 0.0f};
    cgv::render::shader_program shader_program;
    cgv::render::shader_define_map current_defines;
    cgv::render::vertex_buffer vec_sbo, segment_sbo;
    std::vector<cgv::math::fvec<float, 2>> graph_data;
    std::vector<Graph_Param> graph_param;
    unsigned int graph_count = 2;
    bool is_red = false;
    bool is_hidden = false;
    bool show_grid = true;
    bool show_graph = false;
    float domain_alpha = 1.0f;
    float grid_strength = 60.0f;
    float graph_scale = 1.0f;
    float segment_width = 0.02f;

    template<typename T>
    void set_uniform(cgv::render::context &ctx, const std::basic_string<char> &name, const T &value)
    {
        shader_program.set_uniform(ctx, shader_program.get_uniform_location(ctx, name), value);
    }
    template<typename T>
    void load_data_buffer(cgv::render::context &ctx, cgv::render::vertex_buffer &buffer, T &data)
    {
        cgv::render::vertex_buffer sbo(cgv::render::VBT_STORAGE, cgv::render::VBU_STATIC_READ);
        if (!sbo.create(ctx, data)) throw;
        buffer = std::move(sbo);
    }
    void create_data_buffers(cgv::render::context &ctx)
    {
        segment_sbo.destruct(ctx);
        vec_sbo.destruct(ctx);
        load_data_buffer(ctx, segment_sbo, graph_param);
        load_data_buffer(ctx, vec_sbo, graph_data);
    }
    void gen_graph_data()
    {
        std::vector<cgv::math::fvec<float, 2>> s1;
        std::vector<cgv::math::fvec<float, 2>> s2;
        std::vector<cgv::math::fvec<float, 2>> s3;
        for(int i = 0; i<10000; i++)
        {
            float x = 0.002f*(float)i;
            float y1 = sin(x*180.0f*0.035f);
            float y2 = cos(x*180.0f*0.035f);
            float y3 = tan(x*180.0f*0.035f);
            s1.push_back(vec2(x, y1));
            s2.push_back(vec2(x, y2));
            s3.push_back(vec2(x, y3));
        }
        graph_data.insert(graph_data.end(), s1.begin(), s1.end());
        graph_data.insert(graph_data.end(), s2.begin(), s2.end());
        graph_data.insert(graph_data.end(), s3.begin(), s3.end());
        Graph_Param gp;
        gp.vertex_count = 10000;
        gp.color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
        gp.scale = vec2(3.0f, 1.0f);
        graph_param.push_back(gp);
        gp.color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
        graph_param.push_back(gp);
        gp.color = vec4(0.0f, 1.0f, 1.0f, 1.0f);
        graph_param.push_back(gp);
        //graph_count = 3;
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
    bool init(cgv::render::context &ctx) override
    {
        gen_graph_data();
        create_data_buffers(ctx);
        return true;
    }
    void draw(cgv::render::context &ctx) override
    {
        GLboolean blend = glIsEnabled(GL_BLEND);
        GLboolean cull_face = glIsEnabled(GL_CULL_FACE);
        GLenum blend_src, blend_dst, depth;
        glGetIntegerv(GL_BLEND_DST, reinterpret_cast<GLint*>(&blend_dst));
        glGetIntegerv(GL_BLEND_SRC, reinterpret_cast<GLint*>(&blend_src));
        glGetIntegerv(GL_DEPTH_FUNC, reinterpret_cast<GLint*>(&depth));

        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);

        const int vec_sbo_h = vec_sbo.handle ? (const int&)vec_sbo.handle - 1 : 0,
        segment_sbo_h = segment_sbo.handle ? (const int&)segment_sbo.handle - 1 : 0;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, segment_sbo_h);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vec_sbo_h);

        const cgv::render::shader_define_map defines = {
                {"GRAPH_COUNT", std::to_string(graph_count)},
                {"IS_RED", std::to_string(is_red)},
                {"IS_HIDDEN", std::to_string(is_hidden)},
                {"SHOW_GRID", std::to_string(show_grid)},
                {"SHOW_GRAPH", std::to_string(show_graph)}
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
        set_uniform(ctx, "graph_scale", graph_scale);
        set_uniform(ctx, "segment_width", segment_width);
        auto &rcr = cgv::render::ref_rectangle_renderer(ctx, 0);
        rcr.set_prog(shader_program);
        rcr.set_position(ctx, RECT_POSITION);
        rcr.set_extent(ctx, RECT_EXTEND);
        rcr.render(ctx, 0, 1);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

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
        add_member_control(this, "is_hidden", is_hidden, "toggle", "w=60");
        add_member_control(this, "show_grid", show_grid, "toggle", "w=60", " ");
        add_member_control(this, "show_graph", show_graph, "toggle", "w=60");
        add_member_control(this, "domain_alpha", domain_alpha, "value_slider", "min=0;max=1;step=0.001;ticks=true");
        add_member_control(this, "grid_strength", grid_strength, "value_slider", "min=10;max=200;step=1;ticks=true");
        add_member_control(this, "graph_scale", graph_scale, "value_slider", "min=0.1;max=10;step=0.1;ticks=true");
        add_member_control(this, "segment_width", segment_width, "value_slider", "min=0.001;max=0.1;step=0.001;ticks=true");
        add_member_control(this, "graph_count", graph_count, "value_slider", "min=1;max=3;step=1;ticks=true");
    }
};

#include <cgv/base/register.h>
cgv::base::factory_registration<procedural_plot> procedural_plot("New/Demo/procedural_plot", 'N');
