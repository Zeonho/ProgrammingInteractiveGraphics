#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to calculate per-fragment shading here using a toon shading model

in vec3 Position;
in vec3 Normal;

out vec4 color;

uniform vec3 lightPosition;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;

uniform sampler2D diffuseRamp;
uniform sampler2D specularRamp;
void main() {
    // color = vec4(0,0,0,1);
    //vec4 tex_color = texture(diffuseRamp, texcoords);
    
    // vertex position in "eye space"
    vec3 v = normalize(Position).xyz;
    
    // unit vector from the vertex to the light
    vec3 l = normalize(lightPosition - v);
    
    // unit vector from the vertex to the eye point, which is at 0,0,0 in "eye space"
    vec3 e = normalize(-v); //normalize(vec3(0,0,0) - v);
    
    // normal transformed into "eye space"
    vec3 n = normalize(Normal).xyz;
    
    // halfway vector
    vec3 h = normalize(l + e); // (l + e) / 2;
    
    
    // color = ka*Ia + kd*Id*(n dot l) + ks*Is*(h dot n)^s
    vec4 ambient = ka * Ia;
    
    float diff_intensity = (dot(n,l) + 1) / 2;
    vec4 diffuse = kd * Id * texture(diffuseRamp, vec2(diff_intensity,0));
    float specular_intensity = pow(((dot(n,h)+1)/2), s);
    vec4 specular = ks * Is * texture(specularRamp, vec2(specular_intensity,0));;
   
    
    color = ambient + diffuse + specular;
}
