# Scene example
## Introduction
This is an explanation of the file format used for our scene storage. Then there is a basic example of what a YAML scene looks like. For more example you can look at all the scenes in the "rendering" folder.

**NOTE : The order of the parameters in the yaml do matter.**

## Properties
- **RaytracingType** (Mandatory) : this parameter defines the technique used for rendering. All possible rendering types are:
    * Phong = 0
    * Normals (Debug) = 1
    * Z-Buffer (Debug) = 2
    * UV-coords (Debug) = 3
    * Gooch = 4
    * Blinn-Phong = 5
    * Cel = 6

- **Camera** (Mandatory): this defines all the properties of the camera:
   * *eye*: defines the position of the camera in 3D space.
   * *center*: defines the point which the camera is looking at.
   * *up*: defines the orientation of the camera (where is "up" for the camera?). NOTE : if this vector is not normalized, it will deform the scene.
   * *viewSize*: defines the size (in pixel) of the output image.

- **Shadows** (Mandatory): do we compute shadows for the scene? (values : 0 false, 1 true)

- **MaxRecursionDepth** (Mandatory): How many additional times does a ray bounces? (expensive in resources). Useful mostly for reflection.

- **SuperSampling** (Mandatory): Everything related to super sampling
    * *factor*: In how many subpixel each pixel is divided (for SSAA technique). Must be a power of 2.

- **Lights** (Mandatory): this defines all the light sources (each light source is defined by a - and must have all the following properties):
   * *position*: Position of the light source in 3D space
   * *color*: color of the light source

- **Objects** (Mandatory): this defines all the visible objects (each object is defined by a - and must have all the following properties):
    * *type*: what kind of primitive is used? (authorized values: triangle, sphere, quad, model, plane)
    * *primitive specifics data* : data to define the primitive. It depends of the primitive:
        * For triangle: p1,p2,p3 the 3 points defining the triangle
        * For plane:  p1,p2,p3 the 3 points defining the plane
        * For quad:  p1,p2,p3,p4 the 4 points defining the quad
        * For sphere: 
            * *position*: position in space of the 3D model
            * *radius* : radius of the sphere
            * *rotation-axis* (Optional) : defining the rotation axis of the sphere
            * *angle* (Optional) : rotation angle
        * For model: 
            * *path*: the path to the .obj file
            * *scaleFactor*: scale the size of the model
            * *position*: position in space of the 3D model

    * *material*: a material is defined by:
       * texture (Optional): path to the texture
       * bumpMap (Optional): path to the bumpmap
       * ambient (Optional): path to the ambiant occlusion map
       * color : the base color of the material (if no texture)
       * ka : ambiant light coefficient (Phong model)
       * kd : diffuse light coefficient (Phong model)
       * ks : specular light coefficient (Phong model)
       * n : specular exponent (Phong model)

## Scene example
```yaml
#scene01-plane-AmbOcc.yaml

#---
#  This is an example scene description for the raytracer framework.
#
#  The scene description format we use is based on YAML, which is a human friendly 
#  data serialization standard. This gives us a flexible format which should be
#  fairly easy to make both backward and forward compatible (i.e., by ignoring
#  unknown directives). In addition parsers are available for many languages.
#  See http://www.yaml.org/ for more information on YAML.

# It should be clear that the #-character can be used to insert comments.

#---

#All possible rendering types : Phong = 0, Normals = 1, Z-Buffer = 2, UV-coords = 3, Gooch = 4, Blinn-Phong = 5, Cel = 6
RaytracingType: 0

Camera:
    eye: [0,110,3100]
    center: [0,110,0]
    up: [0,1,0]
    viewSize: [1000,1000]  #aspectRatio * baseLine 

Shadows: 0
MaxRecursionDepth: 0
SuperSampling:
  factor: 4

Lights:
- position: [200,600,1500]
  color: [1,1,1]

Objects:
- type: plane
  p1: [-50,-50,-50]
  p2: [50,-50,-50]
  p3: [-50,50,-50]
  material:
    texture: bluegrid.png
    color: [1,1,1]
    ka: 0.2
    kd: 0.8
    ks: 0
    n: 1
- type: sphere
  position: [-200,350,550]
  radius: 100
  rotation-axis : [1,0,0]
  angle : 90
  material:
    texture: texture.png
    color: [0.0,0.0,1.0]
    ka: 0.1
    kd: 0.7
    ks: 0.1
    n: 1
- type: sphere
  position: [0,350,550]
  radius: 100
  rotation-axis : [1,0,0]
  angle : 90
  material:
    texture: textureBump.png
    color: [0.0,0.0,1.0]
    ka: 0.1
    kd: 0.7
    ks: 0.1
    n: 1
- type: sphere
  position: [200,350,550]
  radius: 100
  rotation-axis : [1,0,0]
  angle : 90
  material:
    texture: textureAmbOcc.png
    color: [0.0,0.0,1.0]
    ka: 0.1
    kd: 0.7
    ks: 0.1
    n: 1
- type: sphere
  position: [-200,100,550]
  radius: 100
  rotation-axis : [1,0,0]
  angle : 90
  material:
    texture: texture.png
    color: [0.0,0.0,1.0]
    ka: 0.1
    kd: 0.7
    ks: 0.1
    n: 1
- type: sphere
  position: [0,100,550]
  radius: 100
  rotation-axis : [1,0,0]
  angle : 90
  material:
    texture: texture.png
    bumpMap: textureBump.png
    color: [0.0,0.0,1.0]
    ka: 0.1
    kd: 0.7
    ks: 0.1
    n: 1
- type: sphere
  position: [200,100,550]
  radius: 100
  rotation-axis : [1,0,0]
  angle : 90
  material:
    texture: texture.png
    ambient: textureAmbOcc.png
    color: [0.0,0.0,1.0]
    ka: 0.1
    kd: 0.7
    ks: 0.1
    n: 1
- type: sphere
  position: [0,-150,550]
  radius: 100
  rotation-axis : [1,0,0]
  angle : 90
  material:
    texture: texture.png
    bumpMap: textureBump.png
    ambient: textureAmbOcc.png
    color: [0.0,0.0,1.0]
    ka: 0.1
    kd: 0.7
    ks: 0.1
    n: 1
```

