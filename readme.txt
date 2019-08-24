Basic 3D graphics engine

Initialization:
1. inputManager.h
2. main.cpp
3. class which inherits from scene   

Virtual function description 
1.

Add attribute, type vec2 or vec3
1. In shader.h add attribute name in enum Attributes
2. In shader.cpp bind your new attribute in the constructor by "glBindAttribLocation"
3. In MeshConstructor.h add VEC2_ATTRIB_NUM or VEC3_ATTRIB_NUM 1
4. In Mesh.h in GetData function add attribute in the appropriate location (indieces must remain the last one)
5. In Mesh.h add Vertex or LineVertex the new attribute 
6. If you want to add attribute to the simple objects you can do so in Mesh.cpp


