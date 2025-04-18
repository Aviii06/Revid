# Revid
A simple deferred renderer written in vulkan.

# Example
Instanced render of 1000 bunny, with phong shading.
![Screenshot 2025-01-25 at 2 07 25 PM](https://github.com/user-attachments/assets/f3f32b77-2d8f-4a81-9079-af61f2855dca)


## GBuffers
- position, normals, color (set to red).
![Screenshot 2025-01-25 at 2 09 04 PM](https://github.com/user-attachments/assets/fb157f40-9e96-4e5a-b2b5-21f3e46b1397)

## Pipeline
It uses subpasses for effeciency. First subpass writes gbuffer and the second subpass does all the lighting calculations.
Since the example is only a directional light source, we don't need positions for the lighting subpass.
![Screenshot 2025-01-25 at 2 09 34 PM (1)](https://github.com/user-attachments/assets/b5d34063-d4df-46f8-a8e0-6cdc34ddb9c3)

## Profiling
![Screenshot 2025-01-25 at 2 10 24 PM](https://github.com/user-attachments/assets/734ee58b-0911-4504-b617-6a7f11626c01)
