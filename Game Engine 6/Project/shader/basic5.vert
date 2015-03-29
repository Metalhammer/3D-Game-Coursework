#version 330

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec2 fragTexCoord;

uniform mat4 mModel;
uniform mat4 mView;         
uniform mat4 mProjection;   

out vec2 texCoord;

void main()
{
vec4 vertPosition = vec4(VertexPosition,1.0);
texCoord = fragTexCoord * vec2(1.0, -1.0);
gl_Position = vertPosition;
}
