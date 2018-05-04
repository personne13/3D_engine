// #version 330
//
// void main(){
//   //gl_Position = vec4(0, 0, 0, 1);
// }

#version 150 core
in vec2 in_Vertex;

void main(){
    gl_Position = vec4(in_Vertex, 0.0, 1.0);
}
