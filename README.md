### SoftWareRenderer - Ji

About My Implemented Software Renderer

You could download Release.zip in repo and unzip it. And you are good to go! :)

InSpired by https://github.com/ssloy/tinyrenderer/wiki/Lesson-0:-getting-started

Development Language: C++

Development Environment: Windows 11 - Microsoft Visual Studio 2022

## Getting Started
# Prerequisites
C++
Windows 11
Microsoft Visual Studio 2022

# Installation
1. Download `Release.zip` from the repository.
2. Unzip the file.
3. Open the project in Microsoft Visual Studio 2022.
Controls
Instructions for user interaction within the software renderer.

# Scene Navigation
W or S: Switch scenes.
#Camera Control
Left Mouse Button: Rotate the camera.
Right Mouse Button: Move the camera.
Space Bar: Reset the camera.
Lighting and Shading
A or D: Rotate the direction of the light.
Menu: Switch shaders.



## Improvement 1: Real-Time Rendering and Camera Control
One major issue with TinyRenderer was that it outputs the rendering result to a TGA image, which is a form of offline rendering. Sometimes, rendering issues can only be discovered by adjusting the viewing angle, which made debugging difficult. Our first major improvement was to integrate a graphical interface for real-time rendering and to implement camera control through input signals.

![](https://github.com/phantom23333/SoftwareRenderer/blob/main/README_IMG/SoftwareRenderer_tinyrenderer_orbit_camera.gif)

For this, I referred directly to the code in zauonlok’s software rendering project. The graphical interface uses the Qt Creator Lib, and the camera control was based on another open-source library for orbit camera control. After these improvements, we achieved the control effects shown in the image above.



## Improvement 2: Optimizing Shader Process
Let’s first look at a shader code from TinyRenderer (we’re discussing shaders written in C++, not special shader languages):

```cpp
struct GouraudShader : public IShader {
    mat<3,3,float> varying_tri;
    Vec3f          varying_ity;

    virtual ~GouraudShader() {}

    virtual Vec3i vertex(int iface, int nthvert) {
        Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert));
        gl_Vertex = Projection*ModelView*gl_Vertex;
        varying_tri.set_col(nthvert, proj<3>(gl_Vertex/gl_Vertex[3]));

        varying_ity[nthvert] = CLAMP(model->normal(iface, nthvert)*light_dir, 0.f, 1.f);

        gl_Vertex = Viewport*gl_Vertex;
        return proj<3>(gl_Vertex/gl_Vertex[3]);
    }

    virtual bool fragment(Vec3f bar, TGAColor &color) {
        float intensity = varying_ity*bar;
        color = TGAColor(255, 255, 255)*intensity;
        return false;
    }
};
```
This process has some irregularities, such as the lack of separate data structures and direct model access in the vertex shader (vs), instead of just receiving a vertex data structure. The interpolation process from vs to fragment shader (fs) is handled internally, but we can improve on these aspects.

```cpp
shader_struct_v2f BlinnShader::vertex(shader_struct_a2v* a2v) {
	shader_struct_v2f v2f;
	v2f.clip_pos = ObjectToClipPos(a2v->obj_pos);
	v2f.world_pos = ObjectToWorldPos(a2v->obj_pos);
	v2f.world_normal = ObjectToWorldNormal(a2v->obj_normal);
	v2f.uv = a2v->uv;
	return v2f;
}

bool BlinnShader::fragment(shader_struct_v2f* v2f, Color& color) {
	Vector3f worldNormalDir = (v2f->world_normal).normalize();
	Color albedo = tex_diffuse(v2f->uv) * shader_data->matrial->color;
	Color ambient = shader_data->ambient * albedo;
	float n_dot_l = saturate(worldNormalDir * WorldLightDir());
	Color diffuse = shader_data->light_color * albedo * n_dot_l;
	Vector3f viewDir = WorldSpaceViewDir(v2f->world_pos).normalize();
	Vector3f halfDir = (viewDir + WorldLightDir()).normalize();
	Color spcular = shader_data->light_color * shader_data->matrial->specular * std::pow(saturate(worldNormalDir * halfDir), shader_data->matrial->gloss);

	Vector4f depth_pos = shader_data->light_vp_matrix * embed<4>(v2f->world_pos);
	int shadow = is_in_shadow(depth_pos, n_dot_l);

	color = ambient + (diffuse + spcular) * shadow;
	return false;
}
```
In my implementation, I tried to encapsulate the vertex and fragment shader stages similar to how shaders are written in Unity. This makes it easier to understand what the engine does for us outside the shader functions.




## Improvement 3: Scene and Objects
You might have noticed that TinyRenderer doesn’t discuss model transformations much. In the MVP matrix, the MV matrix is combined into one matrix called ModelView, which actually only includes the View matrix. It assumes the model's origin is at the world space origin and doesn't consider rotation or scaling. Once we deal with multiple objects or rotations and scaling of a single object, this approach becomes insufficient.

We can follow game engine rules, establish the concept of objects, and store each object's position, rotation, scaling, etc. We can also establish a scene concept, managing all objects within it.

```cpp
struct Transform {
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;
};

class GameObject {
public:
	Transform transform;
	GameObject(Vector3f position = Vector3f::Zero, Vector3f rotation = Vector3f::Zero, Vector3f scale = Vector3f::One);
	virtual void tick(float delta_time);
	Matrix4x4 GetModelMatrix();
};
```
The Transform structure includes position, rotation, and scaling information. Each GameObject must contain a Transform, with each GameObject having its own model transformation matrix.

I won’t go into detail about the Scene implementation here; it mainly involves managing the creation and destruction of objects, including models, lights, cameras, etc.




## Improvement 4: Input Signal Integration for Scene Control
![](https://github.com/phantom23333/SoftwareRenderer/blob/main/README_IMG/SoftwareRenderer_final.gif)

Referring to camera control, we can use the Win32 API to receive input signals and implement input control logic. As shown above, we implemented logic for switching shaders and scenes (switching models) through keyboard input. Additionally, we can easily implement control of object movement, light direction, and shadow toggle after integrating scene and object management.




## Improvement 5: Writing a Shader
Create a Shader Class: Inherit from the IShader class.

```cpp
#ifndef __HATCHSHADER__H__
#define __HATCHSHADER__H__

#include "../ShaderLibrary/IShader.hpp"
#include "../ShaderLibrary/ShaderCore.hpp"

class HatchShader : public IShader {
    // Class definition
};

#endif
Define Property, Attributes, and Varyings Structures: These structures should contain members of type vec4, double, or Texture. Ensure that the Attributes structure contains at least one vec4 member, representing the vertex input to the vertex shader. The order of semantics should be vertex, normal, tangent, texcoord.
cpp
Copy code
public:
    struct Properties {
        // Textures and other properties
    } properties;

private:
    struct Attributes {
        vec4 vertex;
        vec4 normal;
        vec4 texcoord;
    } attributes;

    struct Varyings {
        // Interpolated values to be used in the fragment shader
    } varyings;

```

Initialize the Shader: In the constructor, bind the Attributes and Varyings structures, set the attri_semantic flags, and add passes with vertex and fragment shader functions.

```cpp
HatchShader() {
    // Initialization code
}
Implement Vertex and Fragment Shaders for Each Pass: Ensure vertex shaders return homogenous clipping coordinates and fragment shaders return pixel colors.
cpp
Copy code
vec4 outlineVertex() {
    // Vertex shader logic
}

vec4 outlineFragment() {
    // Fragment shader logic
}
```
// Additional shading logic
Adding the Shader
Add the Shader to ShaderManager: Update ShaderManager.h to include the new shader, adjusting the count and initializing properties.

```cpp
namespace SHADERMANAGER {
    const int number = 4;
    // Other shader names
}

struct ShaderManager {
    // Initialization functions
}
```
Integrate with the Rendering Application: In rendererApp.cpp, make the shader interact with the UI and rendering pipeline. Modify functions like bindPropertiesAddress(), bindPropertiesUI(), and render() to integrate the shader.
```cpp
void rendererApp::bindPropertiesAddress() {
    // Binding logic
}

void rendererApp::bindPropertiesUI() {
    // UI binding logic
}

void rendererApp::render() {
    // Render logic
}

```
![](https://github.com/phantom23333/SoftwareRenderer/blob/main/README_IMG/softwarerender.png)

By following these steps, you can successfully write and integrate a new shader into your rendering application. The process involves careful structuring of the shader code and its properties, as well as ensuring that it is correctly integrated into the existing rendering pipeline and UI components of the application.
