#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTangent;
layout (location = 3) in vec2 vertexUV;

struct Transform
{
	vec3 position;
	vec4 orientation;
	vec3 scale;
};

vec3 quaternionRotate(vec4 q, vec3 v)
{
	vec3 t = 2.0 * cross(q.xyz, v);
	return (v + q.w * t + cross(q.xyz, t)); 
}

out VS_OUT
{
	vec3 FragPos;
	vec2 TexCoords;
    mat3 TBN;
} vs_out;

uniform mat4 camera = mat4(1.0f);
uniform mat4 model;
uniform vec3 lightPosition;
uniform vec3 viewPos;
uniform vec3 viewDir;

void main()
{
	// ModelViewProjection matrix
	mat4 MVP = camera * model;

	vec3 pos = vec3(model * vec4(vertexPosition, 1.0)); 

	// Calculate vertex position in camera space
	gl_Position = camera * model * vec4(vertexPosition, 1.0);

	vec3 N = normalize( ( model * vec4( vertexNormal, 0.0 ) ).xyz );
	vec3 T = normalize( ( model * vec4( vertexTangent, 0.0 ) ).xyz );

	// Reorthogonalize with respect to N
	T = normalize(T - dot(T, N) * N);

	// Calculate bitangent
	vec3 B = normalize( ( model * vec4( ( cross( vertexNormal, vertexTangent.xyz ) * 1.0), 0.0 )).xyz);
	mat3 TBN = mat3(T, B, N);

	// Output data
	vs_out.FragPos = vec3(model * vec4(vertexPosition, 1.0));
	vs_out.TexCoords = vec2(vertexUV.x, -vertexUV.y);	
	vs_out.TBN = TBN;
}