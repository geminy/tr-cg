#version 300 es

struct directional_light
{
	vec3 direction; // 眼睛空间内的规范化照明方向
	vec3 halfplane; // 规范化的半平面向量H,对于直射光可以预先计算,因为它不会变化
	vec4 ambient_color; // 环境光颜色
	vec4 diffuse_color; // 漫射光颜色
	vec4 specular_color; // 反射光颜色
};

struct material_properties
{
	vec4 ambient_color; // 材料的环境颜色
	vec4 diffuse_color; // 材料的漫射颜色
	vec4 specular_color; // 材料的反射颜色
	float specular_exponent; // 材料的光亮度的反光指数,用于控制反射高光的亮度
};

const float c_zero = 0.0;
const float c_one = 1.0;

uniform directional_light light;
uniform material_properties material;

// normal是一个规范化向量而且转换到了眼睛空间
// 函数中将环境光\漫射光\反射光组合为单个颜色
// 返回计算后的颜色
// 多个光源时应该为每个光源计算一次
vec4 directional_light_color(vec3 normal)
{
	vec4 computerd_color = vec4(c_zero, c_zero, c_zero, c_zero);
	float ndotl; // dot product of normal & light direction
	float ndoth; // dot product of normal & half-plane vector
	ndotl = max(c_zero, dot(normal, light.direction);
	ndoth = max(c_zero, dot(normal, light.halfplane);
	computered_color += (light.ambient_color * material.ambient_color);
	computered_color += (ndotl * light.diffuse_color * material.diffuse_color);
	if (ndoth > c_zero) {
		computered_color += (pow(ndoth, material.specular_exponent) * material.specular_color * light.specular_color);
	}
	return computered_color;
}
