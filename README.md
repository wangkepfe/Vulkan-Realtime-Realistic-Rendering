# Vulkan-Graphics
learning/developing/designing

<h3>Hello Triangle</h3>

* Fully control of physical device, selection of suitable queue families and logical device creation
* Window surface integrated with GLFW
* Swap Chain Creation with triple buffers
* Built Graphical Pipeline(fixed stages and shader/programmable stages)
* Created Frame buffers and command buffers
* Sychronization of graphical process and presentation process
* Validation layer for debugging

<h3>Textured Model Rendering</h3>

* created vertex buffers, staging buffers, index buffers and controled the memory usage of buffers
* created uniform buffers and descriptor sets
* created texture images and image samplers
* created depth/stencil buffers
* image library: <a href='https://github.com/nothings/stb'>stb_image.h</a>
* obj model library: <a href='https://github.com/syoyo/tinyobjloader'>tiny_obj_loader.h</a>

<h3>Physically-Based Rendering</h3>

* Diffuse and Phong specular lighting 
<img src="https://github.com/wangkepfe/Vulkan-Graphics/blob/master/Demo/basic_lighting.PNG" width="250" height="200"/>

* Diffuse and Blinn-Phong specular lighting 
<img src="https://github.com/wangkepfe/Vulkan-Graphics/blob/master/Demo/basic_lighting_Blinn_phong.PNG" width="250" height="200"/>

* Normal Mapping 
<img src="https://github.com/wangkepfe/Vulkan-Graphics/blob/master/Demo/normal_mapping.PNG" width="250" height="200"/>

* Parallax Occlusion Mapping 
<img src="https://github.com/wangkepfe/Vulkan-Graphics/blob/master/Demo/parallax_mapping.PNG" width="250" height="200"/>

* PBR with single light source 
<img src="https://github.com/wangkepfe/Vulkan-Graphics/blob/master/Demo/PBR.PNG" width="250" height="200"/>

<h3>References: </h3>

* <a href='http://vinjn.github.io/awesome-vulkan/'>Awesome Vulkan ecosystem</a>
* <a href='https://github.com/SaschaWillems/Vulkan'>Examples and demos by SaschaWillems</a>
* <a href='https://vulkan-tutorial.com/Introduction'>Vulkan Tutorial by Alexander Overvoorde</a>
