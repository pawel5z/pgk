# pgk
Chosen solutions of assignments of the Basics of Computer Graphics course @ Computer Science studies @ University of Wrocław.

## [Assignment 2](https://github.com/pawel5z/pgk/tree/assignment2)
![pgk_assignment2](https://user-images.githubusercontent.com/45500957/145473757-74d4955b-1a24-4698-9103-59da1716c9eb.gif)

## [Assignment 3](https://github.com/pawel5z/pgk/tree/assignment3)
![pgk_assignment3](https://user-images.githubusercontent.com/45500957/145473925-ac634c49-aaeb-4f8e-8f8f-8c5dbf02e42b.gif)

## [Assignment 4](https://github.com/pawel5z/pgk/tree/assignment4)
![pgk_assignment4](https://user-images.githubusercontent.com/45500957/145475456-f2035ad6-69b4-470c-85b2-abb826ecda76.png)

## [Assignment 5](https://github.com/pawel5z/pgk/tree/assignment5)
![pgk_assignment5](https://user-images.githubusercontent.com/45500957/145475521-7a20ae4c-7810-49b9-9a26-13a0b984c478.png)

## [Assignment 6](https://github.com/pawel5z/pgk/tree/assignment6)

Visualizer of `.hgt` files.

### Controls
- tab - toggle between map view and 3d view;
- moving mouse while holding RMB - map panning, camera rotating;
- W / S - map zoom in / out, move forward / backward in 3D view;
- A / D - move left / right in 3D view;
- Q / E - decrease / increase distance to Earth's center in 3D view;
- 0..5 - change LOD; 0 - auto mode, 1 - the most details, 5 - the least details;
- left shift (hold) - slower movement.

### How to run it

```
./lab6 path/to/directory/with/hgt/files
```

Each `.hgt` file name must be in format `AXXOYYY.hgt` where `A` (latitude) is eiter `N` or `S`, `O` (longitude) is either `E` or `W` and degrees (`XX`, `YYY`) are written with leading zeros. E.g. `N45E007.hgt`.

Program handles files which contain 1201 x 1201 twobyte big endian heights.
More on this here https://wiki.openstreetmap.org/wiki/SRTM#Format.

![pgk_assignment6_map](https://user-images.githubusercontent.com/45500957/149538060-303722ce-86dd-4822-ac23-0717ba44e294.png)
![pgk_assignment6_3d](https://user-images.githubusercontent.com/45500957/149538182-f89f4fba-924c-483d-ad43-0b20f6cf12d5.png)

## [Assignment 7](https://github.com/pawel5z/pgk/tree/assignment7)

`.obj` file viewer.

![image](https://user-images.githubusercontent.com/45500957/188507082-6e55b5d9-89c9-40b6-a80e-24f21b36c209.png)

### Usage

```
./lab7 path/to/obj [path/to/texture]
```

### Controls

- hold left mouse button and move - rotate camera
- hold right mouse button and move - pan camera
- scroll - zoom in/out
- left shift (hold) - slower panning

## Build instructions
From repo root dir: \
`$ make` \
Executable's name is `labN`, where `N` is the number of the assignment.

### Debug build
Compiles to executable with debug symbols. \
Delete any files without debug symbols. \
`$ make distclean` \
`$ make debug`
