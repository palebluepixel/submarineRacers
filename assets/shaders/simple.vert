uniform mat4 MVP;
uniform mat4 SECOND;
attribute vec3 vCol;
attribute vec3 vPos;
varying vec3 color;
void main(){
    gl_Position = MVP * SECOND * vec4(vPos, 1.0);
    color = vCol;
}
