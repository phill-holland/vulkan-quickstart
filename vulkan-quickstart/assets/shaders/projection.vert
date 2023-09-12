#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform camera 
{
    mat4 world;
    mat4 object;
} cameraData; 

layout(push_constant) uniform constants
{
    mat4 render_matrix;
} PushConstants;

void main() {
    gl_Position = PushConstants.render_matrix * (cameraData.world * (cameraData.object * vec4(inPosition, 1.0)));
    fragColor = inColor;
}
