顶点梦皮着色器,不检查权重是否为0
# version 300 es
#define NUM_MATRICES 32 // 32 matrices in matrix palette
const int c_zero  = 0;
const int c_one   = 1;
const int c_two   = 2;
const int c_three = 3;
// store 32 4 x 3 matrices as an array of floats representing
// each matrix in row-major order (i.e., 3 vec4s)
uniform vec4 matrix_palette[NUM_MATRICES * 3];
// vertex position and normal attributes
in vec4 a_position;
in vec3 a_normal;
// matrix weights - 4 entries / vertex
in vec4 a_matrixweights;
// matrix palette indices
in vec4 a_matrixindices
void skin_position(in vec4 position, float m_wt, int m_indx, out vec4 skinned_position)
{
	vec4 tmp;
	tmp.x = dot(position, matrix_palette[m_indx]);
	tmp.y = dot(position, matrix_palette[m_indx + c_one]);
	tmp.z = dot(position, matrix_palette[m_indx + c_two]);
	tmp.w = position.w;
	skinned_position += m_wt * tmp;
}
void skin_normal(in vec3 normal, float m_wt, int m_indx, inout vec3 skinned_normal)
{
	vec3 tmp;
	tmp.x = dot(normal, matrix_palette[m_indx].xyz);
	tmp.y = dot(normal, matrix_palette[m_indx + c_one].xyz);
	tmp.z = dot(normal, matrix_palette[m_indx + c_two].xyz);
	skinned_normal += m_wt * tmp;
}
void do_skinning(in vec4 position, in vec4 normal, out vec4 skinned_position, out vec3 skinned_normal)
{
	skinned_position = vec4(float(c_zero));
	skinned_normal = vec3(float(c_zero));
	// transform position and normal to eye space using matrix
	// palette with four matrices used to transform a vertex
	float m_wt = a_matrixweights[0];
	int m_indx = int(a_matrixindices[0]) * c_three;
	skin_position(position, m_wt, m_indx, skinned_position);
	skin_normal(normal, m_wt, m_indx, skinned_normal);
	m_wt = a_matrixweiths[1];
	m_indx = int(a_matrixindices[1]) * c_three;
	skin_position(position, m_wt, m_indx, skinned_position);
	skin_normal(normal, m_wt, m_indx, skinned_normal);
	m_wt = a_matrixweights[2];
	m_indx = int(a_matrixindices[2]) * c_three;
	skin_position(position, m_wt, m_indx, skinned_position);
	skin_normal(normal, m_wt, m_indx, skinned_normal);
	m_wt = a_matrixweights[3];
	m_indx = int(a_matrixindices[3]) * c_three;
	skin_position(position, m_wt, m_indx, skinned_position);
	skin_normal(normal, m_wt, m_indx, skinned_normal);
}
// add a main function to make this into a valid vertex shader

顶点梦皮着色器,检查权重是否为0
void do_skinning(in vec4 position, in vec4 normal, out vec4 skinned_position, out vec3 skinned_normal)
{
	skinned_position = vec4(float(c_zero));
	skinned_normal = vec3(float(c_zero));
	// transform position and normal to eye space using matrix
	// palette with four matrices used to transform a vertex
	int m_indx = 0;
	float m_wt = a_matrixweights[0];
	if (m_wt > 0.0) {
		int m_indx = int(a_matrixindices[0]) * c_three;
		skin_position(position, m_wt, m_indx, skinned_position);
		skin_normal(normal, m_wt, m_indx, skinned_normal);
	}
	m_wt = a_matrixweiths[1];
	if (m_wt > 0.0) {
		m_indx = int(a_matrixindices[1]) * c_three;
		skin_position(position, m_wt, m_indx, skinned_position);
		skin_normal(normal, m_wt, m_indx, skinned_normal);
	}
	m_wt = a_matrixweights[2];
	if (m_wt > 0.0) {
		m_indx = int(a_matrixindices[2]) * c_three;
		skin_position(position, m_wt, m_indx, skinned_position);
		skin_normal(normal, m_wt, m_indx, skinned_normal);
	}
	m_wt = a_matrixweights[3];
	if (m_wt > 0.0) {
		m_indx = int(a_matrixindices[3]) * c_three;
		skin_position(position, m_wt, m_indx, skinned_position);
		skin_normal(normal, m_wt, m_indx, skinned_normal);
	}
}
