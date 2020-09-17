#version 300 es

struct spot_light
{
	vec4 position; // 在眼睛空间内的照明方向
	vec4 ambient_color; // 环境光颜色
	vec4 diffuse_color; // 漫射光颜色
	vec4 specular_color; // 反射光颜色
	vec3 spot_direction; // 规范化的点方向向量
	vec3 attenuation_factors; // 距离衰减因子K0 K1 K2
	bool compute_distance_attenuation; // 确定距离衰减是否必须计算
	float spot_exponent; // 用于计算点截止因子的聚光灯指数
	float spot_cutoff_angle; // 聚光灯截止角度(度数)
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

uniform spot_light light;
uniform material_properties material;

// normal和position在眼睛空间
// normal是一个规范化的向量
// 返回计算后的颜色
vec4 spot_light_color(vec3 normal, vec4 position)
{
	vec4 computerd_color = vec4(c_zero, c_zero, c_zero, c_zero);
	vec3 lightdir;
	vec3 halfplane;
	float ndotl;
	float ndoth;
	float att_factor;
	att_factor = c_one;
	// 假设光源位置和顶点位置的w分量相同
	lightdir = light.position.xyz - position.xyz;
	// 计算距离衰减因子
	if (light.compute_distance_attenuation) {
		vec3 att_dist;
		att_dist.x = c_one;
		att_dist.z = dot(lightdir, lightdir);
		att_dist.y = sqrt(att_dist.z);
		att_factor = c_one / dot(att_dist, light.attenuation_factors);
	}
	// 规范化光源方向向量
	lightdir = normalize(lightdir);
	// 计算点截止因子
	if (light.spot_cutoff_angle < 180.0) {
		float spot_factor = dot(-lightdir, light.spot_direction);
		if (spot_factor >= cos(radians(light.spot_cutoff_angle))) {
			spot_factor = pow(spot_factor, light.spot_exponent);
		}
		else {
			spot_factor = c_zero;
		}
		// 计算距离和点衰减因子的组合
		att_factor *= spot_factor;
	}
	if (att_factor > c_zero) {
		// 根据光照公式计算光照颜色
		computerd_color += (light.ambient_color * material.ambient_color);
		ndotl = max(c_zero, dot(normal, lightdir);
		computered_color += (ndotl * light.diffuse_color * material.diffuse_color);
		halfplane = normalized(lightdir + vec3(c_zero, z_zero, c_one));
		ndoth = dot(normal, halfplane);
		if (ndoth > c_zero) {
			computered_color += (pow(ndoth, material.specular_exponent) * material.specular_color * light.specular_color);
		}
		// 颜色乘以衰减因子
		computerd_color *= att_factor;
	}
	
	return computered_color;
}
