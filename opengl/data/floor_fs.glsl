#version 330
in vec3 fp;
in vec3 fnormal;
in vec2 ftex;

out vec4 fcolor;

uniform vec3 headlightPos = vec3(0.0,3.0,0.0);
uniform vec3 cameraPos;
uniform vec3 lightColor = vec3(1.0,1.0,1.0);
uniform vec3 objectColor;

uniform sampler2D texture1;

void main() {

//    vec3 ambientC = 0.1*lightColor;

//    vec3 norm = normalize(fnormal);
//    vec3 lightDir = normalize(headlightPos - fp);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = diff * lightColor;
   
//    // specular
//    float specularStrength = 0.5;
//    vec3 viewDir = normalize(cameraPos - fp);
//    vec3 reflectDir = reflect(-lightDir, norm);  
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//    vec3 specular = specularStrength * spec * lightColor;  

//    vec3 lightDir2 = normalize(cameraPos - fp);
//    float diff2 = max(dot(norm,lightDir2),0.0);
//    vec3 diffuse2 = diff2*lightColor;

//    vec3 reflectDir2 = reflect(-lightDir2,norm);
//    float spec2 = pow(max(dot(viewDir,reflectDir2),0.0),32);
//    vec3 specular2 = specularStrength * spec2 * lightColor;
      
//    vec3 result = (ambientC + diffuse + specular + diffuse2 + specular2) * objectColor;
//    fcolor = vec4(1.0,1.0,1.0, 1.0);
   fcolor = texture(texture1,ftex);
}
