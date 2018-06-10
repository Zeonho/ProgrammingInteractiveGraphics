#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to calculate per-fragment shading here using the Phong shading
// model.


in vec3 Position;
in vec3 Normal;

out vec4 color;

uniform vec3 lightPosition;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;

// uniform = variables passed in from the C++ code
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

void main() {
  //    color = vec4(1,0,0,1);
  //  vec3 n = normalize(Normal);
  //
  //
  //
  //  vec3 h = normalize(Position + lightPosition);
  //  color = (ka * Ia) + (kd * Id * dot(Normal, lightPosition)) + (ks * Is * pow(dot(h,Normal), s));
  //
  //  vec4 ambient = ka * Ia;
  //
  //
  //  float t = max(dot(Normal, lightPosition, 0.0);
  //  vec4 diffuse = kd * Id * t;
  //
  //
  //  float a = max(dot(h,Normal),0.0);
  //  vec4 specular = ks * Is * pow(a, s);
  //
  //
  //  color = ambient + diffuse + specular;

  // vertex position in "eye space"
  vec3 v = normalize(Position).xyz;
  // vec3 v = (modelViewMatrix * vec4(vertex,1)).xyz;
  
  // unit vector from the vertex to the light
  vec3 l = normalize(lightPosition - v);
  
  // unit vector from the vertex to the eye point, which is at 0,0,0 in "eye space"
  vec3 e = normalize(-v); //normalize(vec3(0,0,0) - v);
  
  // normal transformed into "eye space"
  vec3 n = normalize(Normal).xyz;
  // vec3 n = (normalMatrix * vec4(normal,0)).xyz;
  
  // halfway vector
  vec3 h = normalize(l + e); // (l + e) / 2;
  
  
  // color = ka*Ia + kd*Id*(n dot l) + ks*Is*(h dot n)^s
  vec4 ambient = ka * Ia;
  vec4 diffuse = kd * Id * max(dot(n,l), 0);
  vec4 specular = ks * Is * pow(max(dot(n,h),0), s);
  
  color = ambient + diffuse + specular;
}


