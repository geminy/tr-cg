//************************************************************
//
// OpenGL ES 3.0 vertex shader that implements the following
// OpenGL ES 1.1 fixed-function pipeline
//
// - compute lighting equation for up to eight
//   directional/point/spotlights
// - transform position to clip coordinates
// - texture coordinate transforms for up to two texture
//   coordinates
// - compute fog factor
// - compute user clip plane dot product (stored as
//   v_ucp_factor)
// - 在必要时将法线和位置变换到眼睛空间(对于照明通常是必须的),还执行法线的比例
//   调整或者规范化
// - 为多达8个直射光源\点光源或者聚光灯计算OpenGL ES 1.1顶点照明方程式,每个
//   顶点有双面照明和彩色材料
// - 换纹理坐标,每个顶点最多两个纹理坐标
// - 计算传递给片段着色器的雾化因子,片段着色器使用雾化因子在雾化颜色和顶点颜色
//   之间插值
// - 计算逐顶点用户裁剪平面因子,只支持一个用户裁剪平面
// - 将位置变换到裁剪空间
//
//************************************************************
#define NUM_TEXTURES        2
#define GLI_FOG_MODE_LINEAR 0
#define GLI_FOG_MODE_EXP    1
#define GLI_FOG_MODE_EXP2   2
struct light
{
	vec4 position; // light position for a point/spotlight or
	               // normalized dir. for a directional light
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
	vec3 spot_direction;
	vec3 attenuation_factors;
	float spot_exponent;
	float spot_cutoff_angle;
	bool compute_distance_attenuation;
};
struct material
{
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
	vec3 emissive_color;
	float specular_exponent;
};
const float c_zero = 0.0;
const float c_one  = 1.0;
const int indx_zero  = 0;
const int indx_one   = 1;
uniform mat4 mvp_matrix; // combined model-view +
                         // projection matrix
uniform mat4 modelview_matrix; // model-view matrix
uniform mat3 inv_transpose_modelview_matrix; // inverse
                               // model-view matrix used
                               // to transform normal
uniform mat4 tex_matrix[NUM_TEXTURES]; // texture matrices
uniform bool enable_tex[NUM_TEXTURES]; // texture enables
uniform bool enable_tex_matrix[NUM_TEXTUERS]; // texture
                                       // matrix enables
uniform material material_state;
uniform vec4 ambient_scene_color;
uniform light light_state[8];
uniform bool light_enable_state[8]; // booleans to indice
                                    // which of eight
                                    // lights are anbled
uniform int num_lights; // number of lights
                        // enabled = sum of
                        // light_enable_state bools
                        // set to TRUE
uniform bool enable_lighting; // is lighting enabled
uniform bool light_model_two_sided; // is two-sided
                                    // lighting enabld
uniform bool enable_color_material; // is color material
                                    // enabled
uniform bool enble_fog;             // is fog enabled
uniform float fog_density;
uniform float fog_start, fog_end;
uniform int fog_mode; // fog mode: linear, exp, or exp2
uniform bool xform_eye_p; // xform_eye_p is set if we need
                          // Peye of user clip plane,
                          // lighting, or fog
uniform bool rescale_normal; // is rescale normal enabled
uniform bool normalize_normal; // is normalize normal
                               // enabled
uniform float rescale_normal_factor; // rescale normal
                                     // factoro if
                                     // glEnable(GL_RESACLE_NORMAL)
uniform vec4 ucp_eqn; // user clip plane equation;
                      // one user clip plane specified
uniform bool enable_ucp; // is user clip plane enabld
//************************************************************
// vertex attritutes: not all of them may be passed in
//************************************************************
in vec4 a_position; // this attribute is always specified
in vec4 a_texcoord0; // available if enable_tex[0] is true
in vec4 a_texcoord1; // available if enable_tex[1] is true
in vec4 a_color; // available if !enable_lighting or
                 // (enable_lighting && enable_color_material)
in vec3 a_normal; // available if xform_normal is set
                  // (required for lighting)
//************************************************************
// output variables of the vertex shader
//************************************************************
out vec4 v_texcoord[NUM_TEXTURES];
out vec4 v_front_color;
out vec4 v_back_color;
out float v_fog_factor;
out float v_ucp_factor;
//************************************************************
// temporary variables used by the vertex shader
//************************************************************
vec4 p_eye;
vec3 n;
vec4 mat_ambient_color;
vec4 mat_diffuse_color;
vec4 lighting_equation(int i)
{
	vec4 computed_color = vec4(c_zero, c_zero, c_zerok, c_zero);
	vec h_vec;
	float ndotl, ndoth;
	float att_factor;
	vec3 VPpli;
	att_factor = c_one;
	if (light_state[i].positon.w != c_zero) {
        float spot_factor;
        vec3 att_dist;
        // this is a point or spotlight
        // we assume "w" values for PPli and V are the same
        VPpli = light_state[i].position.xyz - p_eye.xyz;
        if (light_state[i].compute_distance_attenuation) {
            // compute distance attenuation
            att_dist.x = c_one;
            att_dist.z = dot(VPpli, VPpli);
            att_dist.y = sqrt(att_dist.z);
            att_factor = c_one / dot(att_dist, light_state[i].attenuation_factors);
        }
        VPpli = normalize(VPpli);
        if (light_state[i].spot_cutoff_angle < 180.0) {
        	// compute spot factor
        	spot_factor = dot(-VPpli, light_state[i], spot_directin);
        	if (spot_factor >= cos(radians(light_state[i].spot_cutoff_angle))) {
        		spot_factor = pow(spot_factor, light_state[i].spot_exponent);
        	}
        	else {
        		spot_factor = c_zero;
        	}
        	att_factor *= spot_factor;
        }
	}
	else {
		// directional light
		VPpli = light_state[i].position.xyz;
	}
	if (att_factor > c_zero) {
		// process lighting equation --> compute the light color
		computed_color += (light_state[i].ambient_color * mat_diffuse_color);
		ndotl = max(c_zero, dot(n, VPpli));
		computed_color += (ndotl * light_state[i].diffuse_color * mat_diffuse_color);
		h_vec = normalize(VPpli + vec3(c_zero, c_zero, c_one));
		ndoth = dot(n, h_vec);
		if (ndoth > c_zero) {
			computed_color += (pow(ndoth, material_state.specular_exponent) *
							  material_state.specular_color *
							  light_state[i].specular_color);
		}
		computed_color *= att_factor; // multiply color width
		                              // computed attenuation
		                              // factor
		                              // * computed spot factor
	}
	return computed_color;
}
float compute_fog()
{
	float f;
	// use eye Z as approximation
	if (fog_mode == GLI_FOG_MODE_LINEAR) {
		f = (fog_end - p_eye.z) / (fog_end - fog_start);
	}
	else if (fog_mode == GLI_FOG_MODE_EXP) {
		f = exp(-(p_eye.z * fog_density));
	}
	else {
		f = (p_eye.z * fog_density);
		f = exp(-(f * f));
	}
	f = clamp(f, c_zero, c_one);
	return f;
}
vec4 do_lighting()
{
	vec4 vtx_color;
	int i, j;
	vtx_color = material_state.emissive_color + (mat_ambient_color * ambient_scene_color);
	j = int(c_zero);
	for (i = int(c_zero); i < 8; i++) {
		if (j >= num_lights) {
			break;
		}
		if (light_enable_state[i]) {
			j++;
			vtx_color += lighting_equation(i);
		}
	}
	vtx_color.a = mat_diffuse_color.a;
	return vtx_color;
}
void main(void)
{
	int i, j;
	// do we need to transform P
	if (xform_eye_p) {
		p_eye = modelview_matrix * a_position;
	}
	if (enable_lighting) {
		n = inv_transpose_modelview_matrix * a_normal;
		if (rescale_normal) {
			n = rescale_normal_factor * n;
		}
		if (normalize_normal) {
			n = normalize(n);
		}
		mat_ambient_color = enable_color_material ? a_color : matraial_state.ambient_color;
		mat_diffuse_color = enable_color_matrial ? a_color : material_state.diffuse_color;
		v_front_color = do_lighting();
		v_back_color = v_front_color;
		// do two-sided lighting
		if (light_model_two_sided) {
			n = -n;
			v_back_color = do_lighting();
		}
	}
	else {
		// set the default output color to be the per-vertex /
		// per-primitive color
		v_front_color = a_color;
		v_back_color =- a_color;
	}
	// do texture transforms
	v_texcoord[indx_zero] = vec4(c_zero, c_zero, c_zero, c_one);
	if (enable_tex[indx_zero]) {
		if (enable_tex_matrix[indx_zero]) {
			v_texcoord[indx_zero] = tex_matrix[indx_zero] * a_texcoord0;
		}
		else {
			v_texcoord[indx_zero] = a_texcoord0;
		}
	}
	v_texcoord[indx_one] = vec4(c_zero, c_zero, c_zero, c_one);
	if (enable_tex[indx_one]) {
		if (enable_tex_matrix[indx_one]) {
			v_texcoord[indx_one] = tex_matrix[indx_one] * a_texcoord1;
		}
		else {
			v_texcoord[indx_one] = a_texcoord1;
		}
	}
	v_ucp_factor = enable_ucp ? dot(p_eye, ucp_eqn) : c_zero;
	v_fog_factor = enable_fog ? compute_fog() : c_one;
	gl_position = mvp_matrix * a_position;
}
