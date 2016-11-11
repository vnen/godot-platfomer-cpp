# Godot Engine platformer demo as module

This is a reimplementation of the Godot Engine [platformer demo](https://github.com/godotengine/godot-demo-projects/tree/master/2d/platformer)
as a C++ module.

## Method

Each scripted node in the original demo was converted to a new node type in the
C++ module. Then the nodes were replaced in-editor by those new types and the
GDScripts were removed.

## Running

Put the `platformer_module` folder inside the Godot's source `modules` folder.
Then build it as usual.

Open the editor and import the project `platformer` from this repository. You can
then run it directly from there.

## License

MIT
