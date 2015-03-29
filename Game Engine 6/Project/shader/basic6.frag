#version 330

uniform sampler2D tex;

uniform vec3 AmbientReflectivity;
uniform vec3 DiffuseReflectivity;
uniform vec3 SpecularReflectivity;
uniform vec3 AmbientIntensity;
uniform vec3 DiffuseIntensity;
uniform vec3 SpecularIntensity;
uniform int SpecularExponent;

in vec3 CamSpaceLightPosition;
in vec3 CamSpaceVertexPosition;
in vec3 CamSpaceVertexNormal;
in vec2 TextureCoordinates;
in vec3 ViewDirectionVector;

out vec4 FragColour;

vec4 PhongLighting(){
	//Light direction vector
	vec3 LightDirectionVector = normalize(CamSpaceLightPosition - CamSpaceVertexPosition);
	
	//Angle between normal vector and light direction
	float NormalLightVector = max(dot(CamSpaceVertexNormal,LightDirectionVector), 0.0);
	
	//Ambient light
	vec4 AmbientLight = vec4(AmbientIntensity,1.0) * vec4(AmbientReflectivity,1.0);
	
	//Diffuse Light
	vec4 DiffuseLight = vec4(DiffuseReflectivity,1.0) * clamp(vec4(DiffuseIntensity,1.0) * NormalLightVector,0.0,1.0);
	
	//Specular Light
	vec3 ReflectionVector = (2 * CamSpaceVertexNormal * NormalLightVector) - LightDirectionVector;
	float RV = clamp(dot(ReflectionVector,ViewDirectionVector),0.0,1.0);
	vec4 SpecularLight = clamp(vec4(SpecularIntensity,1.0) * vec4(SpecularReflectivity,1.0) * pow(RV,SpecularExponent),0.0,1.0);
	
	return AmbientLight+DiffuseLight;
	
}

void main() {

   FragColour = PhongLighting() * texture(tex,TextureCoordinates);
}