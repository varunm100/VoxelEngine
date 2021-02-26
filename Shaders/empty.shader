#SHADER VERTEX
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 _tex_coord;
layout(location = 2) in float _texture_index;

layout (std140, binding = 0) uniform mats
{
    mat4 projection;
    mat4 view;
};

out vec2 tex_coord;
out float texture_index;

uniform mat4 model;

void main() {
     tex_coord = _tex_coord;
     texture_index = _texture_index;
}

#SHADER FRAGMENT
#version 460 core

in vec2 tex_coord;
in float texture_index;
out vec4 frag_color;

uniform sampler2DArray diffuse;

void main() {
//    frag_color = vec4(1, 0, 0, 1);
    frag_color = texture(diffuse, vec3(tex_coord, texture_index-1));
}