// position is the normalized position coordinate in eye space.
// normal is the normalized normal coordinate in eye space.
// this function returns a vec2 texture coordinate.
vec2 sphere_map(vec3 position, vec3 normal)
{
	reflection = reflect(position, normal);
	m = 2.0 * sqrt(reflection.x * reflection.x + reflection.y * reflection.y);
	return vec2(reflection.x / m + 0.5, reflection.y / m + 0.5);
}

// position is the normalized position coordinate in eye space.
// normal is the normalized normal coordinate in eye space.
// this function returns the reflection vector as a vec3 texture coordinate.
vec3 cube_map(vec3 position, vec3 normal)
{
	return reflect(position, normal);
}