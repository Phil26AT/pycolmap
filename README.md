# Python bindings for COLMAP estimators

At the moment, we provide bindings for essential and fundamental matrix estimation as well as absolute pose estimation.

# Getting started

Clone the repository and its submodules by running:
```
git clone --recursive git@github.com:mihaidusmanu/pycolmap.git
```

## Unix
**Preferred:** `pycolmap` is now available for Python 3.7 and Python 3.8 via [pypi](https://pypi.org/project/pycolmap/0.0.1/):
```bash
pip install pycolmap
```
These wheels were built using [John Lambert's wheel-builder](https://github.com/johnwlambert/pycolmap-wheels).

If you instead prefer to build the wheels from source, COLMAP should be installed as a library before proceeding. Please refer to the official website for installation instructions. PyCOLMAP can then be installed using `pip`: 
```
pip install ./
```

## Windows
To install PyCOLMAP on Windows, we recommend to install COLMAP with [vcpkg](https://github.com/microsoft/vcpkg).
From your vcpkg directory, run
```
.\vcpkg.exe install colmap --triplet=x64-windows --head
```

Then set the `CMAKE_TOOLCHAIN_FILE` environment variable to your `vcpkg\scripts\buildsystems\vcpkg.cmake` path.
Example (powershell):
```
$env:CMAKE_TOOLCHAIN_FILE='C:\Workspace\vcpkg\scripts\buildsystems\vcpkg.cmake'
```

Finally go to the PyCOLMAP folder and run
```
py -m pip install ./
```

## Remark regarding building the wheels from source

PyCOLMAP build is generally tested with the latest commit of the COLMAP [`dev` branch](https://github.com/colmap/colmap/tree/dev).
Using a previous COLMAP build might not work.

# Usage

## Camera parameters

The current bindings are compatible with numpy arrays for both 2D and 3D points. The camera parameters should be sent as a Python dictionary with the following template:
```python
{
    'model': COLMAP_CAMERA_MODEL_NAME,
    'width': IMAGE_WIDTH,
    'height': IMAGE_HEIGHT,
    'params': EXTRA_CAMERA_PARAMETERS_LIST
}
```

Alternatively, you can create a Camera object using these parameters:

```python
camera = pycolmap.Camera(
        COLMAP_CAMERA_MODEL_NAME, IMAGE_WIDTH, 
        IMAGE_HEIGHT, EXTRA_CAMERA_PARAMETERS_LIST
)
```

All functions can be invoked with either a python dictionary or a Camera object.

Please refer to [colmap - src/base/camera_models.h](https://github.com/colmap/colmap/blob/master/src/base/camera_models.h) for more details regarding camera models and parameters.

## Absolute pose estimation

For instance, the following snippet runs absolute pose estimation for a pinhole camera given 2D-3D correspondences:
```python
import pycolmap

# Parameters:
# - points2D: Nx2 array; pixel coordinates
# - points3D: Nx3 array; world coordinates
# - camera_dict: dictionary
# Named parameters
# - max_error_px: float; RANSAC inlier threshold in pixels
answer = pycolmap.absolute_pose_estimation(
    points2D, points3D,
    {
        'model': 'SIMPLE_PINHOLE',
        'width': width,
        'height': height,
        'params': [focal_length, cx, cy]
    }
)
# Returns:
# - dictionary containing the RANSAC output
```

## Standalone Pose Refinement

```python
import pycolmap
# Parameters:
# - tvec: List of 3 floats, translation component of the pose (world to camera)
# - qvec: List of 4 floats, quaternion component of the pose (world to camera)
# - points2D: Nx2 array; pixel coordinates
# - points3D: Nx3 array; world coordinates
# - inlier_mask: array of N bool; true -> corresponding value in points2D/points3D is an inlier
# - camera_dict: dictionary
answer = pycolmap.pose_refinement(
    tvec, qvec, points2D, points3D, inlier_mask,
    {
        'model': 'SIMPLE_PINHOLE',
        'width': width,
        'height': height,
        'params': [focal_length, cx, cy]
    }
)
# Returns:
# - dictionary containing the RANSAC output
```

## SIFT feature extraction

```python
import numpy as np

import pycolmap

from PIL import Image, ImageOps

# Input should be grayscale image with range [0, 1].
with open('image.jpg', 'rb') as f:
    img = Image.open(f)
    img = img.convert('RGB')
    img = ImageOps.grayscale(img)
    img = np.array(img).astype(np.float) / 255.

# Parameters:
# - image: HxW float array
# Named parameters:
# - num_octaves: int (4)
# - octave_resolution: int (3)
# - first_octave: int (0)
# - edge_thresh: float (10)
# - peak_thresh: float (0.01)
# - upright: bool (False)
keypoints, scores, descriptors = pycolmap.extract_sift(img)
# Returns:
# - keypoints: Nx4 array; format: x (j), y (i), sigma, angle
# - scores: N array; DoG scores
# - descriptors: Nx128 array; L2-normalized descriptors
```


## COLMAP Reconstruction
We provide bindings to interface with the standard reconstruction objects of COLMAP.

```python
import numpy as np

import pycolmap

reconstruction = pycolmap.Reconstruction("path/to/my/reconstruction/")
print(reconstruction.summary())

for image_id, image in reconstruction.images.items():
    print(image_id, image)

for point3D_id, point3D in reconstruction.points3D.items():
    print(point3D_id, point3D)

for camera_id, camera in reconstruction.cameras.items():
    print(camera_id, camera)

reconstruction.write("path/to/new/reconstruction/")
reconstruction.export_PLY("path/to/new/reconstruction/reconstruction.ply")
```

World-to-Image projection is inherently supported:

```python
xy = camera.world_to_image(image.project(point3D.xyz))
```

For backwards compatibility, functions requiring a Camera object argument can also be called with a camera dict, and will be implicitly converted when the function is called.

# TODO

- [ ] Add documentation
- [ ] Add more detailed examples
- [ ] Add examples for reconstruction bindings
