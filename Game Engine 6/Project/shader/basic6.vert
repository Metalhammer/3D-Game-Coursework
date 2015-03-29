#version 330

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec2 VertexTexture;
layout (location=2) in vec3 VertexNormal;

uniform vec3 lightPosition;
uniform mat4 mModel;
uniform mat4 mView;         
uniform mat4 mProjection;   
uniform mat3 mNormal;
uniform float innerConeAngle;
uniform float outerConeAngle;

out vec2 TextureCoordinates;

out vec3 CamSpaceVertexPosition;
out vec3 CamSpaceLightPosition;
out vec3 CamSpaceVertexNormal;
out vec3 ViewDirectionVector;

void main()
{
CamSpaceVertexPosition = vec3(mView * mModel * vec4(VertexPosition,1.0));
CamSpaceLightPosition = vec3(mView * mModel * vec4(lightPosition,1.0));
CamSpaceVertexNormal = normalize(mNormal * VertexNormal);
TextureCoordinates = VertexTexture * vec2(1.0, -1.0);
ViewDirectionVector = vec3(-CamSpaceVertexPosition);
vec4 vertPosition = vec4(VertexPosition,1.0);
gl_Position = mProjection * mView * mModel * vertPosition;
}
