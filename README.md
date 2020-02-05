# ray_tracing
Ray tracing:

Interactive Commands:<br/>
1)Left and Right arrows should rotate by 15 degrees about a vertical axis.<br/>
2)Up and Down arrows should rotate by 15 degrees about a horizontal axis.<br/>
3)'t' key should toggles through scene rotation, ball rotation and light rotation.<br/>
4)Press 'A' to toggle anti-aliasing


Lighting includes:

1)Fresnel + Blinn/Phong illumination model with diffuse, specular and ambient components.<br/>
2)Recursive depth of 4 (4 intersections), refraction and shadows.<br/>
3)Anti-aliasing, using up to 16 rays per pixel.<br/>

To Compile:<br/>
g++ main.cpp -lGL -lGLEW -lglfw -o raytracer

![Produced Image](./ss.png)
