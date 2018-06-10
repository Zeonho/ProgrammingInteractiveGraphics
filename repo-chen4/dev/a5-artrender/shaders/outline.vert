#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to modify this vertex shader to move the edge vertex along
// the normal away from the mesh surface IF you determine that the vertex
// belongs to a silhouette edge.


uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 projectionMatrix;
uniform float thickness;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 leftNormal;
layout(location = 3) in vec3 rightNormal;

void main() {
  // vertex position in "eye space"
  vec3 v = (modelViewMatrix * vec4(vertex,1)).xyz;
  
  // unit vector from the vertex to the eye point, which is at 0,0,0 in "eye space"
  vec3 e = normalize(-v); //normalize(vec3(0,0,0) - v);
  
  vec3 left_norm = (normalMatrix * vec4(leftNormal, 0)).xyz;
  vec3 right_norm = (normalMatrix * vec4(rightNormal, 0)).xyz;
  vec3 thick = thickness * normal;
  // if this vertex is on a silhouette edge
  if (dot(e, left_norm) * dot(e, right_norm) < 0.0) {
    // expand the “quad fin” by moving the top vertices in the
    // normal direction.
    // vertex = vertex + thickness;
    v = v + thick;
  }
  gl_Position = projectionMatrix * vec4(v,1);
}
